#!/usr/bin/env python3
"""Fast end-to-end contracts for the argoc and argov command-line adapters."""

import argparse
import difflib
import os
import re
import subprocess
import sys
import tempfile


HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))
ANSI = re.compile(r"\x1b\[[0-9;]*[mK]")


def run(command):
    try:
        completed = subprocess.run(command, capture_output=True, text=True, timeout=10, errors="replace")
    except subprocess.TimeoutExpired as error:
        return None, error.stdout or "", error.stderr or "", "timed out"
    return completed.returncode, ANSI.sub("", completed.stdout), ANSI.sub("", completed.stderr), ""


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--argoc", required=True)
    parser.add_argument("--argov", required=True)
    args = parser.parse_args()

    argoc = os.path.abspath(args.argoc)
    argov = os.path.abspath(args.argov)
    failures = []
    tests_run = 0

    def check(name, condition, detail):
        nonlocal tests_run
        tests_run += 1
        if condition:
            print("[PASS] ", name)
            return
        print("[FAIL] ", name)
        failures.append("%s: %s" % (name, detail))

    for binary in (argoc, argov):
        if not os.access(binary, os.X_OK):
            sys.exit("binary not found or not executable: %s" % binary)

    rc, out, err, problem = run([argoc, "--help"])
    check("argoc_help", not problem and rc == 0 and "Usage:" in out + err and "--output" in out + err,
          problem or "exit %s, output: %s" % (rc, (out + err).strip()[:160]))

    rc, out, err, problem = run([argov, "--help"])
    check("argov_help", not problem and rc == 0 and "Usage:" in out + err and "--verbose" in out + err,
          problem or "exit %s, output: %s" % (rc, (out + err).strip()[:160]))

    rc, out, err, problem = run([argoc])
    check("argoc_requires_input", not problem and rc != 0 and "Missing required" in out + err,
          problem or "exit %s, output: %s" % (rc, (out + err).strip()[:160]))

    rc, out, err, problem = run([argov])
    check("argov_requires_input", not problem and rc != 0 and "No input file specified" in out + err,
          problem or "exit %s, output: %s" % (rc, (out + err).strip()[:160]))

    rc, out, err, problem = run([argoc, "--unknown"])
    check("argoc_rejects_unknown_options", not problem and rc != 0 and "Usage:" in out + err,
          problem or "exit %s, output: %s" % (rc, (out + err).strip()[:160]))

    rc, out, err, problem = run([argov, "--output", "unused.ir"])
    check("argov_rejects_compiler_options", not problem and rc != 0 and "Usage:" in out + err,
          problem or "exit %s, output: %s" % (rc, (out + err).strip()[:160]))

    source = os.path.join(ROOT, "examples", "interpretation", "functional", "01_basics", "hello_world.arn")
    reference = os.path.join(ROOT, "tests", "ir", "hello_valid_reference.txt")

    with tempfile.TemporaryDirectory(prefix="argonaut-cli-tests-") as temp:
        missing = os.path.join(temp, "missing.input")

        rc, out, err, problem = run([argoc, "-a", missing])
        check("argoc_rejects_missing_files", not problem and rc != 0 and "Could not open file" in out + err,
              problem or "exit %s, output: %s" % (rc, (out + err).strip()[:160]))

        rc, out, err, problem = run([argov, "-a", missing])
        check("argov_rejects_missing_files", not problem and rc != 0 and "Could not open file" in out + err,
              problem or "exit %s, output: %s" % (rc, (out + err).strip()[:160]))

        generated = os.path.join(temp, "hello.ir")
        rc, out, err, problem = run([argoc, "-a", source, "-o", generated])
        generated_text = ""
        if os.path.exists(generated):
            with open(generated, encoding="utf-8") as stream:
                generated_text = stream.read()
        with open(reference, encoding="utf-8") as stream:
            reference_text = stream.read()
        difference = "".join(difflib.unified_diff(reference_text.splitlines(True), generated_text.splitlines(True),
                                                  fromfile="reference", tofile="generated"))
        check("argoc_serializes_the_reference_ir", not problem and rc == 0 and generated_text == reference_text,
              problem or "exit %s\n%s%s" % (rc, (out + err), difference))

        rc, out, err, problem = run([argov, "-a", reference])
        check("argov_executes_the_reference_ir", not problem and rc == 0 and out == "Hello, World!\n",
              problem or "exit %s, stdout: %r, stderr: %r" % (rc, out, err))

    print("\n%d tests: %d passed, %d failed" % (tests_run, tests_run - len(failures), len(failures)))
    for failure in failures:
        print("  " + failure)
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
