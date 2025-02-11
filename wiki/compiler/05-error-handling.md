# Error Handling in the Argonaut Compiler

## Introduction

In the Argonaut compiler, robust error handling is essential to provide meaningful feedback to the user when the compiler encounters issues in the source code. This document provides a detailed overview of the error handling mechanism implemented in the compiler, covering the structure of error messages, the types of errors handled, and the functions involved in error reporting.

---

## Table of Contents

- [Error Handling in the Argonaut Compiler](#error-handling-in-the-argonaut-compiler)
  - [Introduction](#introduction)
  - [Table of Contents](#table-of-contents)
  - [Overview of Error Handling](#overview-of-error-handling)
  - [Error Message Structure](#error-message-structure)
    - [Title Line](#title-line)
    - [Context](#context)
    - [Advice](#advice)
  - [Error Types](#error-types)
  - [The `Error` Structure](#the-error-structure)
  - [Error Handling Functions](#error-handling-functions)
    - [`construct_error`](#construct_error)
    - [`yywarn`](#yywarn)
    - [`yerror`](#yerror)
    - [`get_formatted_location`](#get_formatted_location)
    - [`set_error_message`](#set_error_message)
    - [`set_error_type`](#set_error_type)
  - [Usage Examples](#usage-examples)
    - [Syntax Error Example](#syntax-error-example)
    - [Semantic Warning Example](#semantic-warning-example)
  - [Conclusion](#conclusion)

---

## Overview of Error Handling

The error handling module in the Argonaut compiler is designed to:

- Detect and report errors encountered during compilation.
- Provide clear and informative messages to help users identify and fix issues.
- Differentiate between errors (which typically halt compilation) and warnings (which allow compilation to continue).

**Key Components:**

- **Error Types:** Enumerated categories of errors (e.g., syntax, semantic, type errors).
- **Error Messages:** Structured messages providing details about the error.
- **Error Functions:** Functions that create, format, and display error messages.

---

## Error Message Structure

Effective error messages in the Argonaut compiler consist of three main components:

### Title Line

- **Purpose:** Provides a concise summary of the error, including its type and location.
- **Format:** `[Error Type] Error message`
- **Example:**

  ```
  [Syntax Error] Unexpected token ';' at line 42, column 15.
  ```

### Context

- **Purpose:** Highlights the exact location in the code where the error occurred, often including a snippet of the code around the error.
- **Note:** In the provided code, context management is implied but not explicitly implemented.

### Advice

- **Purpose:** Offers suggestions or steps the user can take to resolve the error.
- **Note:** Advice is generally included in the error message as additional information.

---

## Error Types

The compiler handles several types of errors, defined in the `ErrorType` enumeration:

```c
typedef enum {
    NO_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    TYPE_ERROR,
    LEXICAL_ERROR,
    RUN_TIME_ERROR,
} ErrorType;
```

- **NO_ERROR:** Indicates no error occurred.
- **SYNTAX_ERROR:** Errors related to incorrect syntax in the source code.
- **SEMANTIC_ERROR:** Errors where code is syntactically correct but semantically invalid.
- **TYPE_ERROR:** Errors due to type mismatches or incorrect type usage.
- **LEXICAL_ERROR:** Errors during lexical analysis (tokenization).
- **RUN_TIME_ERROR:** Errors that occur during program execution (not typically handled at compile time).

---

## The `Error` Structure

The `Error` structure holds detailed information about an error:

```c
typedef struct {
    ErrorType type;      // The type of the error.
    int line;            // The line number where the error occurred.
    int column;          // The column number where the error occurred.
    char message[500];   // The error message.
    char *filename;      // The name of the file where the error occurred.
} Error;
```

- **type:** Categorizes the error.
- **line & column:** Identify the exact location of the error in the source code.
- **message:** Contains the formatted error message.
- **filename:** Specifies the source file where the error occurred.

**Global Error Variable:**

An instance of `Error`, typically named `error`, is used globally to store and access error information throughout the compiler.

---

## Error Handling Functions

### `construct_error`

**Purpose:**

Creates an `Error` instance with the provided details.

**Prototype:**

```c
Error construct_error(ErrorType type, int line, int column, const char *format, ...);
```

**Parameters:**

- `type`: The type of error (`ErrorType`).
- `line`: Line number where the error occurred.
- `column`: Column number where the error occurred.
- `format`: A format string for the error message.
- `...`: Additional arguments for the format string.

**Usage:**

```c
Error error = construct_error(SYNTAX_ERROR, 42, 15, "Unexpected token '%s'.", token);
```

**Implementation Highlights:**

- Uses `vsnprintf` to safely format the error message with variable arguments.
- Returns a fully constructed `Error` object.

### `yywarn`

**Purpose:**

Displays a formatted warning message to `stderr`.

**Prototype:**

```c
void yywarn(const Error error);
```

**Behavior:**

- Prints the warning in yellow color (using ANSI color codes for terminal highlighting).
- Does not halt the compiler; compilation continues after a warning.

**Example Output:**

```
[Semantic Warning] Variable 'x' is unused.
```

**Implementation Highlights:**

- Switches on `error.type` to display the appropriate warning type.
- Formats and prints the message with color coding.

### `yerror`

**Purpose:**

Displays a formatted error message to `stderr` and can halt the compiler.

**Prototype:**

```c
void yerror(const Error error);
```

**Behavior:**

- Prints the error in red color (using ANSI color codes for terminal highlighting).
- Can terminate the compiler execution (exit call is commented out in the code).

**Example Output:**

```
[Type Error] Incompatible types in assignment at line 10, column 5.
```

**Implementation Highlights:**

- Similar to `yywarn`, but intended for errors rather than warnings.
- The `exit` function call is present but commented out, giving flexibility in whether the compiler halts on errors.

### `get_formatted_location`

**Purpose:**

Generates a string with the formatted location of the error (filename, line, column).

**Prototype:**

```c
char* get_formatted_location();
```

**Behavior:**

- Returns a string like `"filename.c:42:15"`.
- Useful for including in error messages to indicate where the error occurred.

**Implementation Highlights:**

- Uses `snprintf` to format the location string.
- Stores the result in a static `char` array to persist after the function returns.

### `set_error_message`

**Purpose:**

Sets the error message within an `Error` struct using a formatted string.

**Prototype:**

```c
void set_error_message(Error *error, const char *format, ...);
```

**Usage:**

```c
set_error_message(&error, "Undefined variable '%s'.", var_name);
```

**Implementation Highlights:**

- Similar to `construct_error`, but sets the message on an existing `Error` instance.
- Uses variable arguments to format the message.

### `set_error_type`

**Purpose:**

Updates the error type of an existing `Error` struct.

**Prototype:**

```c
void set_error_type(Error *error, ErrorType type);
```

**Usage:**

```c
set_error_type(&error, TYPE_ERROR);
```

---

## Usage Examples

### Syntax Error Example

**Scenario:**

An unexpected token is encountered during parsing.

**Code Snippet:**

```c
if (unexpected_token) {
    Error error = construct_error(SYNTAX_ERROR, line_number, column_number,
                                  "Unexpected token '%s'.", token);
    yerror(error);
}
```

**Output:**

```
[Syntax Error] Unexpected token ';'.
```

### Semantic Warning Example

**Scenario:**

A variable is declared but not used.

**Code Snippet:**

```c
if (variable_unused) {
    Error error = construct_error(SEMANTIC_ERROR, line_number, column_number,
                                  "Variable '%s' is declared but never used.", var_name);
    yywarn(error);
}
```

**Output:**

```
[Semantic Warning] Variable 'temp' is declared but never used.
```

---

## Conclusion

The error handling module in the Argonaut compiler provides a structured and robust way to report errors and warnings to the user. By categorizing errors, providing detailed messages, and pinpointing their locations, developers can efficiently diagnose and correct issues in their code.

**Key Features:**

- **Structured Error Reporting:** Clear separation of error types aids in understanding the nature of issues.
- **Detailed Messages:** Format strings and variable arguments allow for informative and customizable messages.
- **Integration with Compiler Phases:** Can be used throughout different compilation stages (lexical, syntax, semantic analysis).

---

> [!NOTE]
> **Disclaimer:**
> This documentation is intended to provide a comprehensive understanding of the error handling mechanisms within the Argonaut compiler. For further details, refer to the source code repository.
