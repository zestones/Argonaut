# Tests

Everything runs through `make check` (current configuration); the project rules are in the *Development* section of the README.

| Directory | Content | Run alone |
|---|---|---|
| `unit/` | C unit tests (`test.h` harness, self-registration, `TEST_XFAIL`) | `make check-unit` or `build/debug/unit_tests [filter]` |
| `lang/sanity/` | programs that work and must keep working | `python3 tests/run_tests.py tests/lang/sanity` |
| `lang/bugs/` | bug reproductions; the header describes the *correct* behaviour | `python3 tests/run_tests.py --only b29` |
| `lang/limitations/` | missing features, to be decided in `SPEC.md` | |
| `ir/` | corrupted IR files that `argov` must reject cleanly | `python3 tests/run_tests.py tests/ir` |
| `../examples/interpretation/` | real programs with golden stdin/stdout or runtime diagnostics | `python3 tests/run_tests.py examples/interpretation` |
| `xfail/` | expected failures per configuration (`debug`, `release`, `sanitize`, `msan`, `leaks`, `valgrind`) | |
| `regression/` | exact text of the compiler diagnostics (historical suite) | `make check-regression` |
| `bench/` | micro-benchmarks, their literal Python and C transcriptions (`ref/`) and the argov baseline | `make bench` |
| `fuzz/` | AFL++ dictionary and IR corpus generation | `make fuzz`, `make fuzz-vm` |
| `build/doctests/` (generated) | wiki/README examples executed as tests | `make doctests` |

Header directives of a `.arn` case:

```
// CATEGORY: sanity | bug | limitation | docs   (informational)
// DESC: description
// IN: line sent to stdin                        (repeatable)
// OUT: expected stdout line                     (repeatable)
// COMPILE: fail                                  argoc must report an error
// RC: nonzero                                    … and exit with a non-zero code
// RUN: fail                                      argov must exit with a non-zero code
// ERR: text                                      failing argoc/argov output must contain this text
```

Statuses: `PASS`, `FAIL`, `XFAIL` (expected failure, listed in `xfail/<config>.txt`), `XPASS` (a listed test that passes: remove the line — it is an error until you do).

The `xfail` lists are a ratchet: they can only shrink. `--write-xfail` exists for the initial bootstrap of a new configuration only, never to hide a regression.
