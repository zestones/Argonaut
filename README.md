<div align="center">
  <img src="./extensions/custom-icons/icons/icon.png" alt="Argonaut">
  
  <br />
  <br />
  
  [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
  [![CI](https://github.com/zestones/Argonaut/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/zestones/Argonaut/actions/workflows/ci.yml)
  [![Weekly](https://github.com/zestones/Argonaut/actions/workflows/weekly.yml/badge.svg)](https://github.com/zestones/Argonaut/actions/workflows/weekly.yml)
  [![CodeQL](https://github.com/zestones/Argonaut/actions/workflows/codeql.yml/badge.svg)](https://github.com/zestones/Argonaut/actions/workflows/codeql.yml)
  [![pages-build-deployment](https://github.com/zestones/Argonaut/actions/workflows/pages/pages-build-deployment/badge.svg)](https://github.com/zestones/Argonaut/actions/workflows/pages/pages-build-deployment)
  [![Dependencies](https://img.shields.io/badge/dependencies-GCC%2C%20Flex%2C%20Bison-orange)](https://gcc.gnu.org/)
  [![Argonaut Wiki](https://img.shields.io/badge/Wiki-Argonaut-blue?style=flat)](https://github.com/zestones/Argonaut/wiki)

  <p>Argonaut is a procedural programming language designed for educational purposes, featuring a compiler and virtual machine implemented in C. This project aims to provide a comprehensive understanding of how compilers and interpreters function.
  </p>
</div>

> [!IMPORTANT]
> Checkout **[wiki documentation](https://github.com/zestones/Argonaut/wiki)** to understand how the Argonaut compiler and vm works.

## 🛠️ Getting Started

### Prerequisites

Before you begin, ensure you have the following installed:

- **C Compiler**: gcc
- **Lexical Analyzer**: flex
- **Parser Generator**: bison
- **Documentation Generator**: doxygen (optional)
- **Debugger**: gdb (optional)

### Installation

```bash
git clone https://github.com/zestones/Argonaut.git
cd Argonaut

make            # build the compiler and the VM (debug configuration) into bin/
make check      # unit tests, language tests, IR tests, doc examples, diagnostics suite
make help       # every target: CONFIG=release|sanitize|msan|coverage, lint, coverage, bench, fuzz…
```

The executable files are generated in the `bin` directory (`build/<config>/` keeps one copy per configuration). ``argoc`` stands for Argonaut compiler and ``argov`` for Argonaut virtual machine. The language itself is described in [SPEC.md](SPEC.md); the engineering rules are in the [Development](#-development) section below.

You can also install the VSCode extension for Argonaut to higlight the Argonaut language.

```bash
make extension-install
```

To delete the VSCode extension, run ``make extension-uninstall``.

## 🚀 Usage

- **Compiler:** Run :
  ```bash
  ./bin/argoc -a <source-file> -o <output-file>
  ```
  Use ``./bin/argoc --help`` for more options.
- **Virtual Machine:** Run :
  ```bash
  ./bin/argov -a <intermediate-file>
  ```
  Use ``./bin/argov -h`` for more options.

## 📜 Language Overview

Checkout the **[Argonaut Language Guide](https://github.com/zestones/Argonaut/wiki/argonaut-language-guide)** documentation to write your own programs in Argonaut.

### Sample Program

```js
func fibonacci(num : int) -> int {
    if (num <= 1) { return num; }

    return fibonacci(num - 1) + fibonacci(num - 2);
}

proc main()  {
    var num : int;
    print("Enter a number: ");
    input("%d", num);

    print("fibonacci(%d) = %d\n", num, fibonacci(num));
}

main();
```

> [!NOTE]
> Example programs are provided in the `examples/interpretation/functional/` directory.

## 📚 Language Features

I'll organize both the implemented and missing features by categories:

### Data Types & Variables

- [x] Primitive types (int, float, char, bool)
- [x] Basic string support (print-only)
- [x] Arrays with custom ranges
- [x] Multidimensional arrays
- [x] Struct types
- [x] Variable declaration with explicit typing
- [ ] String manipulation functions
- [ ] Constants/immutable variables
- [ ] Pointers/references

### Operators

- [x] Arithmetic (+, -, *, /, %)
- [x] Comparison (==, !=, <, >, <=, >=)
- [x] Logical (&&, ||, !)
- [x] Assignment (:=)
- [x] Increment/decrement operators (++, --)
- [ ] Compound assignment operators (+=, -=, etc.)
- [ ] Ternary operator

### Control Flow

- [x] If-else, else-if conditionals
- [x] While loops
- [x] For loops
- [x] Break and continue
- [x] Return statement
- [ ] Switch/case statements
- [ ] Do-while loops

### Functions & Procedures

- [x] Functions with return values (only basic return types)
- [x] Procedures (void functions)
- [x] Parameters and arguments
- [x] Nested functions
- [x] Scope rules
- [ ] Function overloading

### I/O & System Integration

- [x] Basic console output (print)
- [x] Basic input reading
- [x] Format specifiers (%d, %f, %c, %s)
- [ ] File I/O

### Documentation & Comments

- [x] Single-line comments (//)
- [ ] Multi-line comments

## 📂 Project Structure

```bash
.
├── bin/                    # Compiled binaries (generated, not versioned)
├── build/                  # Generated parsers, objects, per-configuration binaries (generated)
├── docs/                   # Generated documentation
├── extensions/             # Custom extensions (for Argonaut syntax highlighting)
├── examples/               # Sample programs
│   ├── compilation/        # Compiler examples (diagnostics)
│   └── interpretation/     # Interpreter examples
├── lib/                    # Utility libraries (colors, and table formatting)
├── src/                    # Core implementation
├── tests/                  # Unit, language, IR, regression, bench and fuzz suites (see tests/README.md)
├── tools/                  # Quality gates: warnings/banned-functions ratchets, coverage, doctests, format
└── SPEC.md                 # Language specification and open design decisions
```

## 🧪 Development

The project is kept honest by a set of automated gates (inspired by how Go, LLVM, CPython and SQLite test their toolchains). One rule: **`main` is always green** — nothing is merged unless the `CI` workflow passes, and every fixed bug comes with a test that failed before the fix.

```bash
make                          # debug build → bin/argoc, bin/argov (build/debug/)
make CONFIG=release           # -O2, _FORTIFY_SOURCE=2, -fstack-protector-strong
make CONFIG=sanitize          # ASan + UBSan (-fno-sanitize-recover=all)
make CONFIG=msan CC=clang     # MemorySanitizer (clang only)
make check                    # unit + language + IR + doc examples + diagnostics regression
make CONFIG=sanitize check    # the same suites under the sanitizers
make CONFIG=sanitize check-leaks   # LeakSanitizer ratchet
make lint                     # format-check, banned-check, warnings-check, cppcheck, tidy, analyze
make coverage | bench | fuzz  # line coverage (gcov), micro-benchmarks, AFL++ fuzzing
make help                     # the full list
```

Tools: `gcc`/`clang`, `flex`, `bison`, `python3` (standard library only). Optional: `clang-format`, `clang-tidy`, `cppcheck`, `valgrind`, `gcovr`, `afl++`, `doxygen` (`make deps-apt` on Debian/Ubuntu). Missing tools are reported and skipped locally; the CI installs all of them.

**Strict flags and ratchets.** The strict warning set (`-Wall -Wextra -Wpedantic -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wshadow -Wvla -Wformat=2 -Wundef -Wwrite-strings -Wpointer-arith -Wcast-qual`) is always on. The legacy code does not satisfy every rule yet, so each rule has a *baseline* that can only go down — a new violation fails the build, an old one is a known debt:

| Ratchet | Baseline | Check | Update (downwards only) |
|---|---|---|---|
| Expected test failures | `tests/xfail/<config>.txt` | `make check` — a listed test that fails is `XFAIL`; one that passes is `XPASS` (**error**: remove the line) | edit the file |
| Compiler warnings | `tools/warnings_baseline.json` | `make warnings-check` | `make warnings-baseline` |
| Banned C functions (`strcpy`, `sprintf`, `atoi`, `scanf`, …) | `tools/banned_baseline.json` | `make banned-check` | `python3 tools/check_banned.py --update` |
| Performance | `tests/bench/baseline.json` | `make bench` (+30 % = regression) | `python3 tests/bench/run_bench.py --update` |

Two CI jobs describe the *target state* and stay red until the debt is paid: `warnings-as-errors` (`make WERROR=1`) and `static-analysis` (cppcheck, clang-tidy, `gcc -fanalyzer`). The `weekly` workflow runs the slow checks (valgrind, MemorySanitizer, fuzzing, benchmarks).

**Fixing a bug, step by step.** `python3 tests/run_tests.py --only b29` → `FAIL` → fix → `make check` reports `XPASS` → remove the line from `tests/xfail/debug.txt`, `release.txt`, `sanitize.txt` (and `msan.txt`/`leaks.txt` if present) → `make CONFIG=sanitize check && make lint` → pull request. Test conventions are described in [tests/README.md](tests/README.md); the known bugs are the `b*` cases of `tests/lang/bugs/` (each header states the expected behaviour) and the expected-failure lists in `tests/xfail/`.

## 📚 Documentation

For detailed documentation, including language syntax, compiler architecture, and virtual machine specifications, please visit the [Wiki](https://github.com/zestones/Argonaut/wiki).

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
<div align="center">
  <p>Made with ❤️ by the Argonaut Contributors</p>
  <p>
    <a href="https://github.com/zestones/Argonaut">🔗 Project Repository</a> |
    <a href="https://github.com/zestones/Argonaut/wiki">📖 Documentation</a> |
    <a href="https://github.com/zestones/Argonaut/issues">🐛 Report a Bug</a>
  </p>
  <p>
    <img src="https://img.shields.io/github/stars/zestones/Argonaut?style=social" />
    <img src="https://img.shields.io/github/forks/zestones/Argonaut?style=social" />
  </p>
</div>