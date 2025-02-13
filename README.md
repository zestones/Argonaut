<div align="center">
  <img src="./extensions/custom-icons/icons/icon.png" alt="Argonaut">
  
  <br />
  <br />
  
  [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
  [![Regression Tests](https://github.com/zestones/Argonaut/actions/workflows/regression.yml/badge.svg?branch=main)](https://github.com/zestones/Argonaut/actions/workflows/regression.yml)
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

# Build compiler and VM
make
```

The executable files will be generated in the `bin` directory. ``argoc`` stand for Argonaut compiler and ``argov`` stand for Argonaut virtual machine.

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
  Use ./``./bin/argov --help`` for more options.
- **Virtual Machine:** Run :
  ```bash
  ./bin/argov –a <intermediate-file>
  ```
  Use ``./bin/argov –h`` for more options.

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
├── bin/                    # Compiled binaries
├── docs/                   # Generated documentation
├── extensions/             # Custom extensions (for Argonaut syntax highlighting)
├── examples/               # Sample programs
│   ├── compilation/        # Compiler examples
│   └── interpretation/     # Interpreter examples        
├── lib/                    # Utility libraries (colors, and table formatting)
├── src/                    # Core implementation
└── tests/                  # Regression tests
```

## 🤝 Contributing

We welcome contributions from the community! To contribute:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them with descriptive messages.
4. Push your branch to your fork.
5. Submit a pull request to the main repository.

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