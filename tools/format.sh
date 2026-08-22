#!/usr/bin/env bash
# Formatting gate (clang-format, style in .clang-format).
#
#   tools/format.sh --check        verify the files changed since the merge base with main (ALL=1: every file)
#   tools/format.sh --apply        reformat them in place
#
# Like LLVM's `git clang-format`, only the files touched by a change must be formatted, so the
# gate can be adopted without reformatting the whole history at once. A one-time full reformat
# (ALL=1 tools/format.sh --apply) can be done later and its commit listed in .git-blame-ignore-revs.
set -euo pipefail
cd "$(dirname "$0")/.."

mode="${1:---check}"
if ! command -v clang-format >/dev/null 2>&1; then
    echo "format: clang-format not installed — skipped (install it: apt install clang-format, or pip install clang-format)"
    exit 0
fi

if [ "${ALL:-0}" = "1" ]; then
    mapfile -t files < <(git ls-files 'src/*.c' 'src/*.h' 'tests/unit/*.c' 'tests/unit/*.h' 'lib/*.h')
else
    base="$(git merge-base HEAD "${BASE_REF:-origin/main}" 2>/dev/null || git merge-base HEAD main 2>/dev/null || echo "")"
    if [ -n "$base" ]; then
        mapfile -t files < <( { git diff --name-only --diff-filter=ACMR "$base" -- 'src/*.c' 'src/*.h' 'tests/unit/*.c' 'tests/unit/*.h' 'lib/*.h'; \
                                git ls-files --others --exclude-standard -- 'src/*.c' 'src/*.h' 'tests/unit/*.c' 'tests/unit/*.h' 'lib/*.h'; } | sort -u)
    else
        mapfile -t files < <(git ls-files 'src/*.c' 'src/*.h' 'tests/unit/*.c' 'tests/unit/*.h' 'lib/*.h')
    fi
fi

if [ "${#files[@]}" -eq 0 ]; then
    echo "format: no C file to check"
    exit 0
fi

case "$mode" in
    --apply)
        clang-format -i --style=file "${files[@]}"
        echo "format: ${#files[@]} file(s) formatted"
        ;;
    --check)
        status=0
        for f in "${files[@]}"; do
            if ! clang-format --style=file --dry-run -Werror "$f" >/dev/null 2>&1; then
                echo "format: $f is not formatted (run: make format)"
                status=1
            fi
        done
        [ "$status" -eq 0 ] && echo "format: ${#files[@]} file(s) OK"
        exit "$status"
        ;;
    *)
        echo "usage: $0 --check | --apply" >&2
        exit 2
        ;;
esac
