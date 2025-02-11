<div align="center">
  <img src="./extensions/custom-icons/icons/icon.png" alt="Argonaut">
  
  <br />
  <br />
  
  [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
  [![Build Status](https://img.shields.io/github/actions/workflow/status/zestones/Argonaut/build.yml?branch=main)](https://github.com/zestones/Argonaut/actions)
  [![Dependencies](https://img.shields.io/badge/dependencies-GCC%2C%20Flex%2C%20Bison-orange)](https://gcc.gnu.org/)

  <p>Argonaut is a procedural programming language designed for educational purposes, featuring a compiler and virtual machine implemented in C. This project aims to provide a comprehensive understanding of how compilers and interpreters function.
  </p>
</div>

> [!IMPORTANT]
> Checkout **[wiki](#link)** documentation to understand how the Argonaut compiler and vm works.

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

Checkout the **[Argonaut Language Guide](#link)** documentation to write your own programs in Argonaut.

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
> Example programs are provided in the `examples/interpretation` directory.

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

For detailed documentation, including language syntax, compiler architecture, and virtual machine specifications, please visit our [Wiki](link-to-your-wiki).

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

<div align="center">
  <p>Made with ❤️ by the Argonaut Contributors</p>
  <a href="https://github.com/zestones/Argonaut/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=zestones/Argonaut" />
  </a>
</div>
