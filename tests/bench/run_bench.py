#!/usr/bin/env python3
"""
Argonaut micro-benchmarks: argov against the same programs written in Python and C.

  python3 tests/bench/run_bench.py --argoc BIN --argov BIN            # measure and print
  python3 tests/bench/run_bench.py ... --check [--tolerance 0.30]     # fail if argov is slower than the baseline
  python3 tests/bench/run_bench.py ... --update                       # rewrite tests/bench/baseline.json
  python3 tests/bench/run_bench.py ... --no-ref                       # skip the Python and C references

Every tests/bench/X.arn has a literal transcription in ref/X.py and ref/X.c: same variables, same
loops, same calls (C is compiled with -O2 plus an optimisation barrier so the loops are not folded
away). The three run on the same machine in the same session and must print the same output.

argov is measured end to end (wall time of the process, min of --runs; its start-up is a few ms
out of about a second). A reference is too fast for that, so it times its own body in-process,
repeated enough times to last ~0.2 s, and reports the time per repetition (min of --runs).
The seconds only mean something on this host; the ratios (arg/py, arg/c) are comparable across
machines. The baseline (argov only) is per-host: --check is a guard against large regressions.
"""
import argparse
import json
import math
import os
import platform
import re
import shutil
import subprocess
import sys
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))
REF = os.path.join(HERE, "ref")
BASELINE = os.path.join(HERE, "baseline.json")


def measure(cmd, runs):
    """Minimum wall time, maximum RSS (KB) and stdout over `runs` executions (wait4 per child)."""
    best, max_rss, out = None, 0, b""
    for _ in range(runs):
        with tempfile.TemporaryFile() as stdout, tempfile.TemporaryFile() as stderr:
            t0 = time.perf_counter()
            p = subprocess.Popen(cmd, stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr)
            _pid, status, ru = os.wait4(p.pid, 0)
            dt = time.perf_counter() - t0
            stdout.seek(0)
            out = stdout.read()
            if not os.WIFEXITED(status) or os.WEXITSTATUS(status) != 0:
                stderr.seek(0)
                sys.exit("%s failed: %s" % (" ".join(cmd), stderr.read().decode(errors="replace").strip()[:200]))
        rss_kb = ru.ru_maxrss if sys.platform != "darwin" else ru.ru_maxrss // 1024
        max_rss = max(max_rss, rss_kb)
        best = dt if best is None else min(best, dt)
    return best, max_rss, out


def run_ref(cmd):
    """One run of a reference program: (body seconds per repetition, stdout)."""
    p = subprocess.run(cmd, stdin=subprocess.DEVNULL, capture_output=True)
    m = re.search(rb"^time=([0-9.]+)$", p.stderr, re.M)
    if p.returncode != 0 or not m:
        sys.exit("%s failed: %s" % (" ".join(cmd), p.stderr.decode(errors="replace").strip()[:200]))
    return float(m.group(1)), p.stdout


def measure_ref(cmd, runs, target=0.2):
    """Minimum body time per repetition over `runs` executions, repetitions calibrated to ~target s."""
    t1, out = run_ref(cmd + ["1"])
    reps = max(1, min(1000000, int(target / t1))) if t1 > 0 else 1
    best = None
    for _ in range(runs):
        t, _out = run_ref(cmd + [str(reps)])
        best = t if best is None else min(best, t)
    return best, out, reps


def ratio_str(r):
    if r is None:
        return "-"
    return "x%.2f" % r if r < 10 else "x%.0f" % r


def geomean(values):
    values = [v for v in values if v]
    return math.exp(sum(math.log(v) for v in values) / len(values)) if values else None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--argoc", default=os.path.join(ROOT, "bin", "argoc"))
    ap.add_argument("--argov", default=os.path.join(ROOT, "bin", "argov"))
    ap.add_argument("--python", default=sys.executable, help="interpreter for the Python references")
    ap.add_argument("--cc", default="cc", help="compiler for the C references (-O2)")
    ap.add_argument("--no-ref", action="store_true", help="do not run the Python and C references")
    ap.add_argument("--runs", type=int, default=5)
    ap.add_argument("--check", action="store_true", help="compare argov with the baseline, exit 1 on regression")
    ap.add_argument("--update", action="store_true", help="rewrite the baseline")
    ap.add_argument("--tolerance", type=float, default=0.30, help="allowed relative slowdown (default 0.30)")
    ap.add_argument("--json", help="also write the measurements to this file")
    args = ap.parse_args()

    programs = sorted(f[:-4] for f in os.listdir(HERE) if f.endswith(".arn"))
    if not programs:
        sys.exit("no benchmark program in " + HERE)

    baseline = {}
    if os.path.exists(BASELINE):
        with open(BASELINE, encoding="utf-8") as f:
            baseline = json.load(f).get("programs", {})

    use_py = use_c = not args.no_ref
    if use_c and shutil.which(args.cc) is None:
        print("note: %s not found, the C references are skipped" % args.cc)
        use_c = False

    results, regressions = {}, []
    print("%-16s %11s %8s %6s %10s %7s %9s %8s %8s"
          % ("program", "argonaut(s)", "base(s)", "ratio", "python(s)", "arg/py", "c -O2(s)", "arg/c", "rss(MB)"))
    with tempfile.TemporaryDirectory(prefix="argonaut-bench-") as tmp:
        for name in programs:
            arn = os.path.join(HERE, name + ".arn")
            ir = os.path.join(tmp, name + ".ir")
            c = subprocess.run([args.argoc, "-a", arn, "-o", ir], capture_output=True, text=True)
            if c.returncode != 0 or not os.path.exists(ir):
                sys.exit("compilation failed for %s: %s" % (name, (c.stderr or c.stdout).strip()[:200]))
            t, rss, out = measure([args.argov, "-a", ir], args.runs)
            res = {"seconds": round(t, 4), "max_rss_kb": rss}

            py_src, c_src = os.path.join(REF, name + ".py"), os.path.join(REF, name + ".c")
            t_py = t_c = None
            if use_py and os.path.exists(py_src):
                t_py, out_py, _ = measure_ref([args.python, py_src], args.runs)
                if out_py != out:
                    sys.exit("ref/%s.py prints %r but %s.arn prints %r" % (name, out_py, name, out))
                res["python_seconds"] = round(t_py, 4)
                res["ratio_python"] = round(t / t_py, 1)
            if use_c and os.path.exists(c_src):
                exe = os.path.join(tmp, name + ".ref")
                cc = subprocess.run([args.cc, "-O2", "-o", exe, c_src], capture_output=True, text=True)
                if cc.returncode != 0:
                    sys.exit("compilation failed for ref/%s.c: %s" % (name, cc.stderr.strip()[:200]))
                t_c, out_c, _ = measure_ref([exe], args.runs)
                if out_c != out:
                    sys.exit("ref/%s.c prints %r but %s.arn prints %r" % (name, out_c, name, out))
                res["c_seconds"] = round(t_c, 7)
                res["ratio_c"] = round(t / t_c, 1)
            results[name] = res

            base = baseline.get(name + ".arn", {}).get("seconds")
            ratio = (t / base) if base else None
            flag = ""
            if args.check and base and ratio > 1 + args.tolerance:
                flag = "  <-- SLOWER"
                regressions.append(name)
            print("%-16s %11.3f %8s %6s %10s %7s %9s %8s %8.1f%s"
                  % (name, t, "%.3f" % base if base else "-", ratio_str(ratio),
                     "%.4f" % t_py if t_py is not None else "-", ratio_str(t / t_py if t_py else None),
                     "%.6f" % t_c if t_c is not None else "-", ratio_str(t / t_c if t_c else None),
                     rss / 1024.0, flag))

    g_py = geomean([r.get("ratio_python") for r in results.values()])
    g_c = geomean([r.get("ratio_c") for r in results.values()])
    if g_py or g_c:
        print("\nargonaut: wall time of argov; python / C: body time per repetition, measured in-process")
        print("geometric mean: argonaut is %s slower than python, %s slower than C -O2" % (ratio_str(g_py), ratio_str(g_c)))

    if args.json:
        with open(args.json, "w", encoding="utf-8") as f:
            json.dump(results, f, indent=1)
    if args.update:
        with open(BASELINE, "w", encoding="utf-8") as f:
            json.dump({"host": platform.node(), "machine": platform.machine(), "runs": args.runs,
                       "programs": {n + ".arn": {"seconds": r["seconds"], "max_rss_kb": r["max_rss_kb"]}
                                    for n, r in results.items()}}, f, indent=1)
            f.write("\n")
        print("baseline written to", os.path.relpath(BASELINE, ROOT))
    if regressions:
        print("\nperformance regression (> +%d%%) on: %s" % (args.tolerance * 100, ", ".join(regressions)))
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
