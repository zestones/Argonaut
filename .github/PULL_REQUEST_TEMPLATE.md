## What and why

<!-- The problem solved or the feature added, in two sentences. Link the issue: Fixes #… -->

## Checklist

- [ ] A test that failed before this change now passes (`tests/lang/…`, `tests/unit/…`), and its entries were removed from `tests/xfail/*.txt`
- [ ] `make check` is green in `debug`, `release` and `sanitize`
- [ ] `make lint` is green: formatted code, no new warning and no new banned-function call
- [ ] `SPEC.md` / wiki updated if the language semantics change
