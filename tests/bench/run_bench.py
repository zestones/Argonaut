#!/usr/bin/env python3
"""
Argonaut micro-benchmarks: wall time and peak memory of argov on the programs in tests/bench/.

  python3 tests/bench/run_bench.py --argoc BIN --argov BIN            # measure and print
  python3 tests/bench/run_bench.py ... --check [--tolerance 0.30]     # fail if slower/bigger than the baseline
  python3 tests/bench/run_bench.py ... --update                       # rewrite tests/bench/baseline.json

Each program is run --runs times (default 5); the minimum wall time and the maximum RSS are kept
(min time is the most stable estimator on a noisy machine). The baseline is per-host by nature:
treat --check as a guard against large regressions (x1.3 by default), not as a precise oracle.
"""
import argparse
import json
import os
import platform
import subprocess
import sys
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))
BASELINE = os.path.join(HERE, "baseline.json")


def measure(argov, ir, runs):
    """Minimum wall time and maximum RSS (KB) over `runs` executions, measured per child with wait4."""
    best, max_rss = None, 0
    for _ in range(runs):
        t0 = time.perf_counter()
        p = subprocess.Popen([argov, "-a", ir], stdin=subprocess.DEVNULL,
                             stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
        _pid, status, ru = os.wait4(p.pid, 0)
        dt = time.perf_counter() - t0
        err = p.stderr.read().decode(errors="replace") if p.stderr else ""
        if not os.WIFEXITED(status) or os.WEXITSTATUS(status) != 0:
            sys.exit("%s failed on %s: %s" % (argov, ir, err.strip()[:200]))
        rss_kb = ru.ru_maxrss if sys.platform != "darwin" else ru.ru_maxrss // 1024
        max_rss = max(max_rss, rss_kb)
        best = dt if best is None else min(best, dt)
    return best, max_rss


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--argoc", default=os.path.join(ROOT, "bin", "argoc"))
    ap.add_argument("--argov", default=os.path.join(ROOT, "bin", "argov"))
    ap.add_argument("--runs", type=int, default=5)
    ap.add_argument("--check", action="store_true", help="compare with the baseline, exit 1 on regression")
    ap.add_argument("--update", action="store_true", help="rewrite the baseline")
    ap.add_argument("--tolerance", type=float, default=0.30, help="allowed relative slowdown (default 0.30)")
    ap.add_argument("--json", help="also write the measurements to this file")
    args = ap.parse_args()

    programs = sorted(f for f in os.listdir(HERE) if f.endswith(".arn"))
    if not programs:
        sys.exit("no benchmark program in " + HERE)

    baseline = {}
    if os.path.exists(BASELINE):
        with open(BASELINE, encoding="utf-8") as f:
            baseline = json.load(f).get("programs", {})

    results = {}
    regressions = []
    print("%-28s %10s %10s %12s %10s" % ("program", "time(s)", "base(s)", "ratio", "rss(MB)"))
    with tempfile.TemporaryDirectory(prefix="argonaut-bench-") as tmp:
        for prog in programs:
            ir = os.path.join(tmp, prog + ".ir")
            c = subprocess.run([args.argoc, "-a", os.path.join(HERE, prog), "-o", ir], capture_output=True, text=True)
            if c.returncode != 0 or not os.path.exists(ir):
                sys.exit("compilation failed for %s: %s" % (prog, (c.stderr or c.stdout).strip()[:200]))
            t, rss = measure(args.argov, ir, args.runs)
            results[prog] = {"seconds": round(t, 4), "max_rss_kb": rss}
            base = baseline.get(prog, {}).get("seconds")
            ratio = (t / base) if base else None
            flag = ""
            if args.check and base and ratio > 1 + args.tolerance:
                flag = "  <-- SLOWER"
                regressions.append(prog)
            print("%-28s %10.3f %10s %12s %10.1f%s" % (prog, t, "%.3f" % base if base else "-",
                                                       "x%.2f" % ratio if ratio else "-", rss / 1024.0, flag))

    if args.json:
        with open(args.json, "w", encoding="utf-8") as f:
            json.dump(results, f, indent=1)
    if args.update:
        with open(BASELINE, "w", encoding="utf-8") as f:
            json.dump({"host": platform.node(), "machine": platform.machine(), "runs": args.runs,
                       "programs": results}, f, indent=1)
            f.write("\n")
        print("baseline written to", os.path.relpath(BASELINE, ROOT))
    if regressions:
        print("\nperformance regression (> +%d%%) on: %s" % (args.tolerance * 100, ", ".join(regressions)))
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
