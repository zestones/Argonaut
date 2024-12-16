# CPYRR Compiler Project

This project is a compiler for the CPYRR programming language, which is a simple language designed for educational purposes. The compiler is implemented in C and uses the YACC parser generator to parse the language grammar. It generates an abstract syntax tree (AST) from the parsed program and interprets the AST using a virtual machine.

## Notes

- The project is still under development and is not yet complete.
- The project is structured into different components, each handling a specific part of the compilation process.
- A Makefile is provided to build the project and run the compiler.

## Folder Structure

```bash
/compiler
│
├── /example
│   ├── /compilation                 # Examples and test cases for the compilation process
│   │   ├── /errors                  # Compilation error examples
│   │   ├── /to-clean                # Samples to validate and clean compilation phases
│   │   ├── /todo                    # Examples of incomplete or pending tasks
│   ├── /interpretation              # Examples for the interpretation process
│
├── /lib                             # Common libraries
│   ├── colors.h                     # Color utilities for terminal output
│   ├── table_printer.h              # Utilities for table printing
│
├── /report                          # Archive of development logs and templates
│   ├── Archive*.txt                 # Archived logs for project development
│   ├── template.txt                 # Template file for report creation
│
├── /src
│   ├── /ast
│   │   ├── ast.c                    # Abstract syntax tree (AST) creation and manipulation
│   │   ├── ast.h                    # Header file for AST data structures and functions
│   │   ├── lcrs.c                   # Left-Child Right-Sibling (LCRS) tree implementation
│   │   ├── lcrs.h                   # Header file for LCRS tree functions
│   │   ├── node_type.h              # Header file for defining AST node types
│   │
│   ├── /bin
│   │   ├── lex.yy.c                 # Generated lexical analyzer (by flex)
│   │   ├── y.tab.c                  # Generated parser (by yacc)
│   │   ├── y.tab.h                  # Header file for parser definitions
│   │
│   ├── /data
│   │   ├── region_stack.c           # Region stack management
│   │   ├── region_stack.h           # Header file for region stack functions
│   │   ├── region_table.c           # Region table implementation (region size, nesting, AST pointers)
│   │   ├── region_table.h           # Header file for region table definitions
│   │
│   ├── /lexer
│   │   ├── interpreter_lexer.l      # Lexical analyzer for the interpreter
│   │   ├── lexer.l                  # Lexical analysis (token generation and lexeme identification)
│   │
│   ├── /parser
│   │   ├── grammar.y                # YACC grammar file
│   │   ├── interpreter_grammar.y    # Grammar file for interpretation phase
│   │   ├── parser.c                 # Parser implementation (calls lexing functions and constructs AST)
│   │   ├── parser.h                 # Header file for parser declarations
│   │
│   ├── /semantic_analysis           # Semantic analysis and validation phase
│   │   ├── /assignment_checks       # Assignment validation utilities
│   │   ├── /condition_checks        # Condition validation utilities
│   │   ├── /function_checks         # Function and procedure validation utilities
│   │   ├── /scope_checks            # Scope validation utilities
│   │   ├── /type_checks             # Type validation utilities
│   │   ├── /variable_checks         # Variable validation utilities
│   │   ├── validation_utils.h       # Common utilities for semantic checks
│   │   ├── semantic_checks.h        # Aggregator header for semantic checks
│   │
│   ├── /symbol_table
│   │   ├── declaration              # Declaration table management
│   │   ├── hash                     # Hash table utilities for symbol storage
│   │   ├── lexeme                   # Lexeme table management
│   │   ├── representation           # Type and structure representation table
│   │   ├── utility.c                # Common utilities for symbol table
│   │   ├── utility.h                # Header file for common utilities
│   │
│   ├── /table_management
│   │   ├── array_manager.c          # Array-specific management utilities
│   │   ├── func_proc_manager.c      # Function and procedure management utilities
│   │   ├── structure_manager.c      # Structure-specific utilities
│   │   ├── variable_manager.c       # Variable-specific utilities
│   │
│   ├── /type_system
│   │   ├── /format                  # Format utilities for messages and output
│   │   ├── /type_inference          # Type inference for expressions and structures
│   │
│   └── /utils
│       ├── errors.c                 # Error handling utilities
│       ├── scope_tracker.c          # Scope tracking utilities
│       ├── stack.c                  # Stack utilities
│       ├── utils.h                  # General utility macros and functions
│   
│
├── /tests
│   ├── regression                   # Regression tests for validating components
│
├── Makefile                         # Main Makefile to build the project
├── README.md                        # Project documentation
└── requirements.txt                 # Project dependencies
```

## Key Components

1. **`/example`**: Test cases and example programs for both compilation and interpretation phases.
2. **`/lib`**: Helper libraries for enhanced output and table visualization.
3. **`/src`**: Core implementation files, including lexer, parser, semantic analysis, AST, and symbol table.
4. **`/tests`**: Regression tests to ensure the correctness of implemented features.
5. **Makefile**: Build and automation for the compiler and virtual machine.

---

## Error Messages

- **Title Line**: Error details with a reference to the source code line.
- **Context**: Highlights the issue and its surrounding code snippet.
- **Advice**: Provides suggestions or steps to resolve the error.

## AST Node Format in Intermediate Code

```
Node(<type>, <lex-index>, <decl-index>) [Child: <child>] [Sibling: <sibling>]
```

This format ensures consistency when interpreting or debugging the AST representation.