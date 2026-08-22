#!/usr/bin/env python3
"""Compile every example and language test into an IR corpus for fuzzing argov (make fuzz-vm)."""
import argparse, glob, os, subprocess, sys
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
ap = argparse.ArgumentParser()
ap.add_argument("--argoc", default=os.path.join(ROOT, "bin", "argoc"))
ap.add_argument("--out", required=True)
args = ap.parse_args()
os.makedirs(args.out, exist_ok=True)
sources = sorted(glob.glob(os.path.join(ROOT, "examples", "interpretation", "**", "*.arn"), recursive=True)
                 + glob.glob(os.path.join(ROOT, "tests", "lang", "sanity", "*.arn")))
n = 0
for src in sources:
    ir = os.path.join(args.out, os.path.splitext(os.path.basename(src))[0] + ".ir")
    p = subprocess.run([args.argoc, "-a", src, "-o", ir], capture_output=True)
    if p.returncode == 0 and os.path.exists(ir) and os.path.getsize(ir) > 0:
        n += 1
print("IR corpus: %d file(s) in %s" % (n, args.out))
sys.exit(0 if n else 1)
