#!/usr/bin/env python3
"""
Line-coverage summary from gcov data (GCC --coverage builds).

  python3 tools/coverage_report.py --build build/coverage --src src [--min 0]

Runs `gcov --json-format` on every .gcda found under the build directory, aggregates the
per-line execution counts of the files under --src, prints a table and writes
<build>/coverage.json. With --min N the script exits 1 when the total line coverage is below N%.
If `gcovr` is installed, an HTML report is also produced in <build>/html.
"""
import argparse
import glob
import gzip
import json
import os
import shutil
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))


def run_gcov(gcda):
    objdir = os.path.dirname(gcda)
    cmd = ["gcov", "--json-format", "--stdout", "-o", objdir, gcda]
    p = subprocess.run(cmd, capture_output=True, text=True)
    if p.returncode != 0 or not p.stdout.strip():
        return None
    try:
        return json.loads(p.stdout)
    except json.JSONDecodeError:
        # Some gcov versions gzip the JSON even with --stdout
        try:
            return json.loads(gzip.decompress(p.stdout.encode("latin-1")).decode())
        except Exception:  # noqa: BLE001 - best effort
            return None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--build", required=True, help="build directory of the coverage configuration")
    ap.add_argument("--src", default="src", help="source directory to report on")
    ap.add_argument("--min", type=float, default=0.0, help="minimum total line coverage in percent")
    args = ap.parse_args()

    gcdas = sorted(glob.glob(os.path.join(args.build, "**", "*.gcda"), recursive=True))
    if not gcdas:
        sys.exit("no .gcda file under %s — run `make CONFIG=coverage check` first" % args.build)

    src_abs = os.path.abspath(args.src)
    lines = {}  # file -> {line: count}
    for gcda in gcdas:
        data = run_gcov(gcda)
        if not data:
            continue
        for f in data.get("files", []):
            path = os.path.normpath(os.path.abspath(f["file"]))
            if not path.startswith(src_abs + os.sep):
                continue
            rel = os.path.relpath(path, ROOT)
            d = lines.setdefault(rel, {})
            for l in f.get("lines", []):
                n = l["line_number"]
                d[n] = max(d.get(n, 0), l["count"])

    total_lines = total_hit = 0
    rows = []
    for rel in sorted(lines):
        d = lines[rel]
        n, hit = len(d), sum(1 for c in d.values() if c > 0)
        total_lines += n
        total_hit += hit
        rows.append((rel, n, hit, 100.0 * hit / n if n else 0.0))

    print("%-70s %7s %7s %7s" % ("file", "lines", "hit", "cov%"))
    for rel, n, hit, pct in rows:
        print("%-70s %7d %7d %6.1f%%" % (rel, n, hit, pct))
    total_pct = 100.0 * total_hit / total_lines if total_lines else 0.0
    print("%-70s %7d %7d %6.1f%%" % ("TOTAL", total_lines, total_hit, total_pct))

    with open(os.path.join(args.build, "coverage.json"), "w", encoding="utf-8") as f:
        json.dump({"total_lines": total_lines, "total_hit": total_hit, "percent": round(total_pct, 2),
                   "files": [{"file": r, "lines": n, "hit": h, "percent": round(p, 2)} for r, n, h, p in rows]},
                  f, indent=1)

    if shutil.which("gcovr"):
        html = os.path.join(args.build, "html")
        os.makedirs(html, exist_ok=True)
        subprocess.run(["gcovr", "-r", ROOT, "--object-directory", args.build, "-f", args.src,
                        "--html-details", os.path.join(html, "index.html")], check=False)
        print("HTML report:", os.path.join(html, "index.html"))

    if total_pct < args.min:
        print("coverage %.1f%% is below the required %.1f%%" % (total_pct, args.min))
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
