# Fuzzing

Argonaut's front-ends are fuzzed with [AFL++](https://github.com/AFLplusplus/AFLplusplus): the compiler (`argoc`) reads a source file and the VM (`argov`) reads an IR file, and both call `exit()` on errors — a fork-server fuzzer such as AFL++ handles that naturally, whereas libFuzzer requires an in-process harness that never exits.

```bash
sudo apt install afl++                 # or build from source
make fuzz                              # compiler: corpus = examples/, dictionary = argonaut.dict
make fuzz-vm                           # VM: corpus = IR of the examples and sanity tests
make fuzz FUZZ_TIME=3600 FUZZ_CC=afl-gcc-fast
```

Crashes and hangs land in `build/fuzz/out/default/{crashes,hangs}`. Every finding becomes a regular test: minimise it with `afl-tmin`, add the program to `tests/lang/bugs/` (or the IR to `tests/ir/`) with the expected behaviour in the header, and list it in `tests/xfail/` until it is fixed.

The weekly CI workflow runs a short session (10 minutes) and uploads the findings as an artifact.
