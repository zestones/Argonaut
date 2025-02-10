<div align="center">
  <img src="./extensions/custom-icons/icons/icon.png" alt="Argonaut">
  
  [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
  [![Build Status](https://img.shields.io/github/actions/workflow/status/zestones/argonaut/build.yml?branch=main)](https://github.com/zestones/argonaut/actions)
  [![Dependencies](https://img.shields.io/badge/dependencies-GCC%2C%20Flex%2C%20Bison-orange)](https://gcc.gnu.org/)

  <p>Argonaut is a procedural programming language designed for educational purposes, featuring a compiler and virtual machine implemented in C. This project aims to provide a comprehensive understanding of how compilers and interpreters function.
  </p>
</div>

## 🚀 Key Features

### Compiler Architecture

- **Three-phase compilation process**  
  ```mermaid
  graph LR
  subgraph "Compiler Pipeline"
    direction LR
    Frontend["Frontend (Lexer/Parser)"] --> Middleend["Middleend (Semantic Analysis)"]
    Middleend --> Backend["Backend (Code Generation)"]
  end
  
  Frontend -.-> SymbolTable["Symbol Tables"]
  Middleend -.-> TypeSystem["Type System"]
  Backend -.-> IR["Intermediate Representation"]
  IR --> VM["Virtual Machine Execution"]
  ```

- Modular design with clean separation between:
  - Frontend (Lexer/Parser)
  - Middleend (Semantic Analysis)
  - Backend (Code Generation)
  
## 📜 Language Overview

### Sample Program
```js
PROG fibonacci;

func int fib(int n) {
    if n <= 1 {
        return n;
    }
    return fib(n-1) + fib(n-2);
}

VAR x: int = 10;

MAIN {
    print("Fibonacci sequence:");
    for var i = 0; i < x; i++ {
        print(fib(i));
    }
}
```

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
git clone https://github.com/zestones/argonaut.git
cd argonaut

# Build compiler and VM
make
```

## 📂 Project Structure

```bash
.
├── bin/                    # Compiled binaries
├── docs/                   # Generated documentation
├── examples/               # Sample programs
│   ├── hello.ag           # Basic example
│   └── algorithms/        # Complex programs
├── include/                # Header files
├── lib/                    # Utility libraries
├── src/                    # Core implementation
│   ├── frontend/          # Lexer/Parser
│   ├── middleend/         # Semantic analysis
│   └── backend/           # Code generation
└── tests/                  # Test suite
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
  <a href="https://github.com/zestones/argonaut/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=zestones/argonaut" />
  </a>
</div>
