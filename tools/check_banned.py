#!/usr/bin/env python3
"""
Banned C functions ratchet (inspired by git's banned.h).

Counts calls to unsafe libc functions in src/ and compares them with tools/banned_baseline.json:
the count of each function may only go *down*. New code must use the safe alternative; existing
uses are paid off over time. Once a counter reaches 0 it can never come back.

  python3 tools/check_banned.py            # check (exit 1 if any count increased)
  python3 tools/check_banned.py --update   # rewrite the baseline with the current counts
"""
import argparse
import json
import os
import re
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
BASELINE = os.path.join(ROOT, "tools", "banned_baseline.json")
SRC = os.path.join(ROOT, "src")

BANNED = {
    "strcpy": "memcpy with an explicit bound, or snprintf",
    "strcat": "snprintf into a bounded buffer",
    "sprintf": "snprintf",
    "vsprintf": "vsnprintf",
    "gets": "fgets",
    "strncpy": "memcpy + explicit NUL terminator (strncpy does not always terminate)",
    "strtok": "strtok_r",
    "atoi": "strtol with overflow and garbage checks",
    "atol": "strtol with overflow and garbage checks",
    "atof": "strtod with error checks",
    "scanf": "fgets + strtol/strtod, and always check the return value",
}
CALL = re.compile(r"(?<![\w.>])(%s)\s*\(" % "|".join(BANNED))
EXT = (".c", ".h", ".l", ".y")


def source_files():
    for root, dirs, names in os.walk(SRC):
        dirs[:] = [d for d in dirs if d != "bin"]
        for n in sorted(names):
            if n.endswith(EXT):
                yield os.path.join(root, n)


def count():
    counts = {name: 0 for name in BANNED}
    sites = {name: [] for name in BANNED}
    for path in source_files():
        with open(path, encoding="utf-8", errors="replace") as f:
            for lineno, line in enumerate(f, 1):
                code = line.split("//", 1)[0]
                for m in CALL.finditer(code):
                    counts[m.group(1)] += 1
                    sites[m.group(1)].append("%s:%d" % (os.path.relpath(path, ROOT), lineno))
    return counts, sites


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--update", action="store_true", help="rewrite the baseline with the current counts")
    ap.add_argument("-v", "--verbose", action="store_true", help="list every call site")
    args = ap.parse_args()

    counts, sites = count()
    if args.update:
        with open(BASELINE, "w", encoding="utf-8") as f:
            json.dump(counts, f, indent=2, sort_keys=True)
            f.write("\n")
        print("banned-functions baseline written:", {k: v for k, v in counts.items() if v})
        return 0

    baseline = {}
    if os.path.exists(BASELINE):
        with open(BASELINE, encoding="utf-8") as f:
            baseline = json.load(f)

    failed = False
    print("%-10s %8s %8s   %s" % ("function", "now", "baseline", "use instead"))
    for name in sorted(BANNED):
        now, base = counts[name], baseline.get(name, 0)
        flag = ""
        if now > base:
            flag = "  <-- INCREASED"
            failed = True
        if now or base:
            print("%-10s %8d %8d   %s%s" % (name, now, base, BANNED[name], flag))
        if args.verbose or now > base:
            for s in sites[name]:
                print("           %s" % s)
    total = sum(counts.values())
    print("total banned calls: %d (baseline %d)" % (total, sum(baseline.get(k, 0) for k in BANNED)))
    if failed:
        print("\nNew calls to banned functions were added. Use the safe alternative, or — only when "
              "reducing the total — refresh the baseline with `python3 tools/check_banned.py --update`.")
        return 1
    if any(counts[k] < baseline.get(k, 0) for k in BANNED):
        print("Some counters went down: run `python3 tools/check_banned.py --update` to lock the progress in.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
