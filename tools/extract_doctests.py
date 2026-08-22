#!/usr/bin/env python3
"""
Turn the Argonaut examples of the documentation into executable tests ("doctests").

A fenced ```argonaut block becomes a test when:
  * it is followed (within a few lines) by an "Output" label and a fenced block — the block's
    lines become the expected stdout; or
  * it is immediately preceded by an HTML comment `<!-- doctest -->` (the program must compile
    and run without error; an Output block may still follow).
Optional directives, as HTML comments right before the code block:
  <!-- doctest-stdin: 42 -->    a stdin line (repeatable)
  <!-- doctest: skip -->        never extract this block

Generated files carry the usual header directives understood by tests/run_tests.py.
Usage: extract_doctests.py --out DIR FILE_OR_DIR...
"""
import argparse
import os
import re
import sys

FENCE = re.compile(r"^\s*```\s*([A-Za-z0-9_+-]*)\s*$")
OUTPUT_LABEL = re.compile(r"^\s*(\*\*|__)?\s*(Sample\s+)?Output\s*:?\s*(\*\*|__)?\s*:?\s*$", re.IGNORECASE)
HEADING = re.compile(r"^(#+)\s+(.*)$")
STDIN = re.compile(r"<!--\s*doctest-stdin:\s*(.*?)\s*-->")
MARK = re.compile(r"<!--\s*doctest\s*-->")
SKIP = re.compile(r"<!--\s*doctest:\s*skip\s*-->")


def slug(text):
    s = re.sub(r"[^a-z0-9]+", "_", text.lower()).strip("_")
    return s[:40] or "block"


def markdown_files(paths):
    for p in paths:
        if os.path.isdir(p):
            for root, _dirs, names in os.walk(p):
                for n in sorted(names):
                    if n.lower().endswith(".md"):
                        yield os.path.join(root, n)
        elif p.lower().endswith(".md"):
            yield p


def extract(path):
    with open(path, encoding="utf-8") as f:
        lines = f.read().splitlines()
    tests = []
    heading = os.path.splitext(os.path.basename(path))[0]
    i = 0
    while i < len(lines):
        m = HEADING.match(lines[i])
        if m:
            heading = m.group(2).strip()
        fence = FENCE.match(lines[i])
        if fence and fence.group(1).lower() == "argonaut":
            start = i
            i += 1
            code = []
            while i < len(lines) and not FENCE.match(lines[i]):
                code.append(lines[i])
                i += 1
            end = i  # closing fence
            # directives in the comment lines right before the block
            stdin, marked, skip = [], False, False
            j = start - 1
            while j >= 0 and (lines[j].strip() == "" or "<!--" in lines[j]):
                stdin = [STDIN.match(x).group(1) for x in [lines[j].strip()] if STDIN.match(x)] + stdin
                marked = marked or bool(MARK.search(lines[j]))
                skip = skip or bool(SKIP.search(lines[j]))
                j -= 1
            # expected output: an "Output" label followed by a fenced block
            out = None
            k = end + 1
            limit = min(len(lines), end + 6)
            while k < limit and not OUTPUT_LABEL.match(lines[k]):
                if lines[k].strip() and not lines[k].lstrip().startswith("<!--"):
                    break
                k += 1
            if k < limit and OUTPUT_LABEL.match(lines[k]):
                k += 1
                while k < len(lines) and lines[k].strip() == "":
                    k += 1
                if k < len(lines) and FENCE.match(lines[k]):
                    k += 1
                    out = []
                    while k < len(lines) and not FENCE.match(lines[k]):
                        out.append(lines[k])
                        k += 1
            if not skip and (out is not None or marked):
                tests.append({"line": start + 1, "heading": heading, "code": code,
                              "stdin": stdin, "out": out})
        i += 1
    return tests


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", required=True, help="directory receiving the generated .arn files")
    ap.add_argument("paths", nargs="+", help="markdown files or directories")
    args = ap.parse_args()

    os.makedirs(args.out, exist_ok=True)
    for old in os.listdir(args.out):
        if old.endswith(".arn"):
            os.remove(os.path.join(args.out, old))

    count = 0
    for md in markdown_files(args.paths):
        for n, t in enumerate(extract(md), 1):
            stem = os.path.splitext(os.path.basename(md))[0]
            name = "%s__%02d__%s.arn" % (slug(stem), n, slug(t["heading"]))
            header = ["// CATEGORY: docs",
                      "// DESC: %s:%d (%s)" % (md.replace(os.sep, "/"), t["line"], t["heading"])]
            header += ["// IN: " + s for s in t["stdin"]]
            if t["out"] is not None:
                header += ["// OUT: " + o.rstrip() for o in t["out"]]
            with open(os.path.join(args.out, name), "w", encoding="utf-8") as f:
                f.write("\n".join(header) + "\n" + "\n".join(t["code"]).rstrip("\n") + "\n")
            count += 1
    print("doctests: %d program(s) extracted into %s" % (count, args.out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
