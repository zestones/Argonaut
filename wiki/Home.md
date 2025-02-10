# Argonaut

Welcome to the documentation for the Argonaut Compiler and Virtual Machine. This repository contains everything you need to understand how the compiler is structured, how it works, and how to use it. The goal of this project is to offer a comprehensive and detailed guide for those looking to gain a deeper understanding of how compilers and interpreters function, using the Argonaut language as an educational example.

## What is Argonaut?

Argonaut is a procedural programming language designed for efficient block-structured programming. It supports data structures such as arrays and structures and allows the declaration of types, variables, functions, and procedures. The language is compiled into an intermediate form and executed on a virtual machine, which is the heart of the Argonaut runtime system.

## Key Features

| Feature                     | Description                                                                 |
|-----------------------------|-----------------------------------------------------------------------------|
| **Procedural Language**      | Argonaut uses a traditional block-structured, procedural paradigm.           |
| **Static Scoping**           | Supports variable scoping with region-based rules.                           |
| **Syntactic Flexibility**    | Allows the declaration of types, variables, functions, and procedures, supporting overloading and name masking. |
| **Intermediate Representation** | Programs are compiled into an intermediate format, which is then interpreted by a custom-built virtual machine. |
| **Manual Memory Management** | Explicit handling of the stack and regions during runtime.                  |
