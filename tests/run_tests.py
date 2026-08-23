#!/usr/bin/env python3
"""
Argonaut end-to-end test runner: compiles each program with argoc, runs the IR with argov,
and compares the outcome with the expectations declared in the file header.

Test kinds
  *.arn   language tests. Header directives (comment lines at the top of the file):
            // CATEGORY: sanity | bug | limitation     (informational)
            // DESC: short description                 (informational)
            // IN: line                                stdin line for argov (repeatable)
            // OUT: line                               expected stdout line (repeatable, rstrip'ed)
            // COMPILE: fail                           argoc must report an error
            // RC: nonzero                             argoc must also exit with a non-zero code
            // RUN: fail                               argov must exit non-zero (no crash, no hang)
            // ERR: text                               failing argoc/argov output must contain this text
  *.ir    robustness tests: argov must reject the file with a clean non-zero exit
          (no signal, no timeout).

Expected failures (the "ratchet")
  --xfail FILE lists test ids (one per line, `#` comments) that currently fail because of a
  known bug. Such a test is reported XFAIL and does not break the run; when it starts
  passing it is reported XPASS and *does* break the run, so the entry must be removed.
  Test ids are paths relative to the repository root (e.g. tests/lang/bugs/b01_minus_literal.arn).

Exit code: 1 if any test FAILs or XPASSes, 0 otherwise. Only the Python standard library is used.
"""
import argparse
import concurrent.futures
import json
import os
import re
import shlex
import subprocess
import sys
import tempfile
import time
from xml.sax.saxutils import escape

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, ".."))
ANSI = re.compile(r"\x1b\[[0-9;]*[mK]")
ERROR_TAG = re.compile(r"\[(Lexical|Syntax|Semantic|Type|Run Time) Error\]")


# ----------------------------------------------------------------------------- parsing
def parse_header(path):
    spec = {"category": "sanity", "desc": "", "stdin": [], "out": [], "out_declared": False,
            "compile_fail": False, "rc_nonzero": False, "run_fail": False, "err": None}
    with open(path, encoding="utf-8") as f:
        for line in f:
            if not line.startswith("//"):
                break
            body = line[2:].strip()
            key, sep, val = body.partition(":")
            if not sep:
                continue
            key = key.strip().upper()
            val = val.strip()
            if key == "CATEGORY":
                spec["category"] = val
            elif key == "DESC":
                spec["desc"] = val
            elif key == "IN":
                spec["stdin"].append(val)
            elif key == "OUT":
                spec["out_declared"] = True
                raw = line[2:].partition(":")[2]
                spec["out"].append(raw[1:].rstrip("\n") if raw.startswith(" ") else val)
            elif key == "COMPILE":
                spec["compile_fail"] = val.lower() == "fail"
            elif key == "RC":
                spec["rc_nonzero"] = val.lower() == "nonzero"
            elif key == "RUN":
                spec["run_fail"] = val.lower() == "fail"
            elif key == "ERR":
                spec["err"] = val
    return spec


def read_xfail(path):
    ids = set()
    if not path or not os.path.exists(path):
        return ids
    with open(path, encoding="utf-8") as f:
        for line in f:
            line = line.split("#", 1)[0].strip()
            if line:
                ids.add(line.replace("\\", "/"))
    return ids


def test_id(path):
    rel = os.path.relpath(os.path.abspath(path), ROOT)
    return rel.replace(os.sep, "/")


# ----------------------------------------------------------------------------- execution
def run(cmd, stdin_text, timeout):
    """Returns (returncode or None on timeout, stdout, stderr, timed_out)."""
    try:
        p = subprocess.run(cmd, input=stdin_text, capture_output=True, text=True,
                           timeout=timeout, errors="replace")
        return p.returncode, ANSI.sub("", p.stdout), ANSI.sub("", p.stderr), False
    except subprocess.TimeoutExpired as e:
        def text(x):
            if x is None:
                return ""
            return x.decode(errors="replace") if isinstance(x, bytes) else x
        return None, ANSI.sub("", text(e.stdout)), ANSI.sub("", text(e.stderr)), True


def first_line(text, limit=120):
    for l in text.splitlines():
        if l.strip():
            return l.strip()[:limit]
    return ""


def is_crash(rc):
    return rc is not None and (rc < 0 or rc >= 128)


def run_ir_case(path, argov, wrapper, timeout):
    """argov must reject a corrupted IR file cleanly."""
    rc, out, err, to = run(wrapper + [argov, "-a", path], "", timeout)
    if to:
        return "argov: TIMEOUT (hang on corrupted IR)"
    if is_crash(rc):
        return "argov: crash (signal %s) on corrupted IR — %s" % (rc, first_line(err))
    if rc == 0:
        return "argov accepted a corrupted IR file (exit 0)"
    return ""


def run_lang_case(path, argoc, argov, wrapper, tmpdir, timeout, anomalies):
    spec = parse_header(path)
    name = os.path.splitext(os.path.basename(path))[0]
    ir = os.path.join(tmpdir, name + ".ir")
    if os.path.exists(ir):
        os.remove(ir)

    rc, out, err, to = run(wrapper + [argoc, "-a", path, "-o", ir], "", timeout)
    merged = out + err
    reported = bool(ERROR_TAG.search(merged))
    compile_failed = to or (rc is not None and rc != 0) or reported
    if to:
        return "argoc: TIMEOUT (infinite loop?)"
    if is_crash(rc):
        return "argoc: crash (signal %s) %s" % (rc, first_line(merged))
    if reported and rc == 0:
        anomalies.append("rc0")
    if reported and os.path.exists(ir):
        anomalies.append("ir")

    if spec["compile_fail"]:
        if not compile_failed:
            return "argoc accepted the program (no error reported)"
        if spec["err"] and spec["err"] not in merged:
            return "expected message missing (%r) — got: %s" % (spec["err"], first_line(merged))
        if spec["rc_nonzero"] and rc == 0:
            return "error reported but argoc exited with 0"
        return ""

    if compile_failed:
        return "unexpected compile error: " + first_line(merged)
    if not os.path.exists(ir):
        return "no IR produced"

    stdin_text = "".join(l + "\n" for l in spec["stdin"])
    rc, out, err, to = run(wrapper + [argov, "-a", ir], stdin_text, timeout)
    if to:
        return "argov: TIMEOUT (hang / infinite loop)"

    def output_difference():
        if not spec["out_declared"]:
            return ""
        got = [line.rstrip() for line in out.splitlines()]
        expected = [line.rstrip() for line in spec["out"]]
        while got and got[-1] == "":
            got.pop()
        while expected and expected[-1] == "":
            expected.pop()
        if got == expected:
            return ""

        def show(lines):
            text = " | ".join(lines[:3])
            return text if len(text) <= 70 else text[:70] + "…"

        return "output differs — expected: [%s] got: [%s]" % (show(expected), show(got))

    if spec["run_fail"]:
        if rc == 0:
            return "argov exited with 0 (runtime error expected) — output: " + first_line(out)
        if is_crash(rc):
            return "argov: crash (signal %s) instead of a clean runtime error" % rc
        if spec["err"] and spec["err"] not in out + err:
            return "expected runtime message missing (%r) — got: %s" % (
                spec["err"], first_line(out + err))
        difference = output_difference()
        if difference:
            return difference
        return ""
    if is_crash(rc):
        return "argov: crash (code %s) %s" % (rc, first_line(err))
    if rc != 0:
        return "argov: exit %s — %s" % (rc, first_line(err + out))
    return output_difference()


def run_one(path, args, wrapper, tmpdir):
    start = time.time()
    anomalies = []
    if path.endswith(".ir"):
        why = run_ir_case(path, args.argov, wrapper, args.timeout)
        category = "ir"
    else:
        why = run_lang_case(path, args.argoc, args.argov, wrapper, tmpdir, args.timeout, anomalies)
        category = parse_header(path)["category"]
    return {"id": test_id(path), "category": category, "why": why,
            "anomalies": anomalies, "seconds": time.time() - start}


# ----------------------------------------------------------------------------- reporting
def write_junit(path, results):
    failures = sum(1 for r in results if r["status"] in ("FAIL", "XPASS"))
    skipped = sum(1 for r in results if r["status"] == "XFAIL")
    total_time = sum(r["seconds"] for r in results)
    lines = ['<?xml version="1.0" encoding="UTF-8"?>',
             '<testsuite name="argonaut" tests="%d" failures="%d" skipped="%d" time="%.3f">'
             % (len(results), failures, skipped, total_time)]
    for r in results:
        classname = r["id"].rsplit("/", 1)[0].replace("/", ".")
        lines.append('  <testcase classname="%s" name="%s" time="%.3f">'
                     % (escape(classname), escape(r["id"].rsplit("/", 1)[-1]), r["seconds"]))
        if r["status"] == "FAIL":
            lines.append('    <failure message="%s"/>' % escape(r["why"], {'"': "&quot;"}))
        elif r["status"] == "XPASS":
            lines.append('    <failure message="XPASS: unexpectedly passed, remove it from the xfail list"/>')
        elif r["status"] == "XFAIL":
            lines.append('    <skipped message="XFAIL: %s"/>' % escape(r["why"], {'"': "&quot;"}))
        lines.append("  </testcase>")
    lines.append("</testsuite>")
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")


def discover(paths):
    files = []
    for p in paths:
        if os.path.isfile(p):
            files.append(p)
            continue
        for root, _dirs, names in os.walk(p):
            for n in names:
                if n.endswith(".arn") or n.endswith(".ir"):
                    files.append(os.path.join(root, n))
    return sorted(set(files))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("paths", nargs="*", default=[os.path.join(HERE, "lang"), os.path.join(HERE, "ir")],
                    help="files or directories to scan (default: tests/lang tests/ir)")
    ap.add_argument("--argoc", default=os.path.join(ROOT, "bin", "argoc"))
    ap.add_argument("--argov", default=os.path.join(ROOT, "bin", "argov"))
    ap.add_argument("--xfail", action="append", default=[], help="expected-failure list (repeatable)")
    ap.add_argument("--junit", help="write a JUnit XML report to this file")
    ap.add_argument("--json", help="write detailed results to this JSON file")
    ap.add_argument("--wrapper", default="", help="command prefix for argoc/argov, e.g. a valgrind invocation")
    ap.add_argument("--timeout", type=float, default=20.0, help="seconds per process (default 20)")
    ap.add_argument("--jobs", "-j", type=int, default=max(1, (os.cpu_count() or 2) - 1))
    ap.add_argument("--only", help="run only the tests whose id contains this text")
    ap.add_argument("-v", "--verbose", action="store_true", help="also print passing tests")
    ap.add_argument("--list", action="store_true", help="list discovered test ids and exit")
    ap.add_argument("--write-xfail", metavar="FILE",
                    help="bootstrap helper: write the ids of the failing tests to FILE (never use it to hide a regression)")
    args = ap.parse_args()

    for b in (args.argoc, args.argov):
        if not os.access(b, os.X_OK):
            sys.exit("binary not found or not executable: %s (run `make` first)" % b)

    files = [f for f in discover([p for p in args.paths if os.path.exists(p)])]
    if args.only:
        files = [f for f in files if args.only in test_id(f)]
    if args.list:
        for f in files:
            print(test_id(f))
        return 0
    if not files:
        sys.exit("no test found")

    expected_fail = set()
    for x in args.xfail:
        expected_fail |= read_xfail(x)
    wrapper = shlex.split(args.wrapper) if args.wrapper else []

    results = []
    with tempfile.TemporaryDirectory(prefix="argonaut-tests-") as tmp:
        with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
            futures = {pool.submit(run_one, f, args, wrapper, tmp): f for f in files}
            for fut in concurrent.futures.as_completed(futures):
                r = fut.result()
                failed = bool(r["why"])
                if r["id"] in expected_fail:
                    r["status"] = "XFAIL" if failed else "XPASS"
                else:
                    r["status"] = "FAIL" if failed else "PASS"
                results.append(r)
    results.sort(key=lambda r: r["id"])

    for r in results:
        if r["status"] in ("FAIL", "XPASS") or args.verbose:
            print("[%s] %s%s" % (r["status"], r["id"], (" — " + r["why"]) if r["why"] else ""))

    counts = {s: sum(1 for r in results if r["status"] == s) for s in ("PASS", "FAIL", "XFAIL", "XPASS")}
    rc0 = sum("rc0" in r["anomalies"] for r in results)
    ir = sum("ir" in r["anomalies"] for r in results)
    print("\n%d tests: %d passed, %d failed, %d expected failures, %d unexpected passes"
          % (len(results), counts["PASS"], counts["FAIL"], counts["XFAIL"], counts["XPASS"]))
    print("anomalies: argoc exited 0 despite an error in %d tests; IR written despite an error in %d tests"
          % (rc0, ir))
    stale = sorted(x for x in expected_fail if not any(r["id"] == x for r in results))
    if stale and not args.only:
        print("warning: %d xfail entries match no test: %s" % (len(stale), ", ".join(stale[:5])))

    if args.junit:
        write_junit(args.junit, results)
    if args.json:
        with open(args.json, "w", encoding="utf-8") as f:
            json.dump(results, f, ensure_ascii=False, indent=1)
    if args.write_xfail:
        with open(args.write_xfail, "w", encoding="utf-8") as f:
            f.write("# Tests that currently fail because of known bugs (see tests/README.md, 'ratchet').\n")
            f.write("# Remove an entry when the bug is fixed; the runner flags it as XPASS until you do.\n")
            for r in results:
                if r["why"]:
                    f.write("%s  # %s\n" % (r["id"], r["why"][:90].replace("\n", " ")))
        print("xfail list written to", args.write_xfail)

    return 1 if (counts["FAIL"] or counts["XPASS"]) else 0


if __name__ == "__main__":
    sys.exit(main())
