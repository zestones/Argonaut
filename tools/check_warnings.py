#!/usr/bin/env python3
"""
Compiler warnings ratchet.

Reads a full build log on stdin (`make -B V=1 ... 2>&1 | tools/check_warnings.py`), deduplicates
the warnings by (file, line, column, flag) and compares the per-flag counts with
tools/warnings_baseline.json. Any flag whose count grew fails the check; a new flag fails too.
The strict warning set stays enabled in the Makefile on purpose: the baseline only prevents
*new* warnings while the existing ones are being fixed. Refresh it with --update when counts drop.

  --analyzer   only consider -Wanalyzer-* diagnostics (gcc -fanalyzer) and fail if any is present
"""
import argparse
import os
import re
import sys

from ratchet import load_baseline, write_baseline

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
BASELINE = os.path.join(ROOT, "tools", "warnings_baseline.json")
WARNING = re.compile(r"^(?P<file>[^:\s][^:]*):(?P<line>\d+):(?P<col>\d+): warning: (?P<msg>.*?)(?: \[(?P<flag>-W[^\]]+)\])?\s*$")


def parse(stream):
    seen = set()
    for raw in stream:
        m = WARNING.match(raw.rstrip("\n"))
        if not m:
            continue
        path = os.path.normpath(m.group("file"))
        if path.startswith("build" + os.sep):  # generated flex/bison code is not ours
            continue
        flag = m.group("flag") or "-Wother"
        key = (path, int(m.group("line")), int(m.group("col")), flag)
        if key in seen:
            continue
        seen.add(key)
        yield key, m.group("msg")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--update", action="store_true", help="rewrite the baseline with the current counts")
    ap.add_argument("--analyzer", action="store_true", help="gcc -fanalyzer mode: any finding fails")
    ap.add_argument("-v", "--verbose", action="store_true", help="list every warning site")
    args = ap.parse_args()

    counts, sites = {}, {}
    for (path, line, col, flag), msg in parse(sys.stdin):
        if args.analyzer != flag.startswith("-Wanalyzer-"):
            continue
        counts[flag] = counts.get(flag, 0) + 1
        sites.setdefault(flag, []).append("%s:%d:%d: %s" % (path, line, col, msg))

    if args.analyzer:
        total = sum(counts.values())
        for flag in sorted(counts):
            print("%-45s %5d" % (flag, counts[flag]))
            for s in sites[flag]:
                print("    " + s)
        print("gcc -fanalyzer findings: %d" % total)
        return 1 if total else 0

    if args.update:
        write_baseline(BASELINE, counts)
        print("warnings baseline written (%d unique warnings)" % sum(counts.values()))
        return 0

    baseline = load_baseline(BASELINE)

    failed = False
    print("%-32s %6s %9s" % ("flag", "now", "baseline"))
    for flag in sorted(set(counts) | set(baseline)):
        now, base = counts.get(flag, 0), baseline.get(flag, 0)
        mark = "  <-- INCREASED" if now > base else ""
        failed = failed or now > base
        print("%-32s %6d %9d%s" % (flag, now, base, mark))
        if args.verbose or now > base:
            for s in sites.get(flag, []):
                print("    " + s)
    print("unique warnings: %d (baseline %d)" % (sum(counts.values()), sum(baseline.values())))
    if failed:
        print("\nThe build produces new warnings. Fix them (the strict flags are part of the contract), "
              "or — only when the total goes down — refresh the baseline with `make warnings-baseline`.")
        return 1
    if sum(counts.values()) < sum(baseline.values()):
        print("Fewer warnings than the baseline: run `make warnings-baseline` to lock the progress in.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
