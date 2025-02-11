# Intermediate Code Generation in the Argonaut Compiler

## Introduction

The **Intermediate Representation (IR)** serves as a means to understand how compilers translate high-level language constructs into a form that can be further processed or analyzed. Unlike production compilers, the Argonaut compiler does not perform optimizations on the IR. Instead, it focuses on exporting the symbol tables and the Abstract Syntax Tree (AST) as they are, to provide insights into the compilation process.

This document provides an in-depth look at how the Argonaut compiler generates the Intermediate Code, focusing on:

- The structure and format of Abstract Syntax Tree (AST) nodes in the IR.
- The exportation of symbol tables and other compiler data structures.
- Interpretation of the exported IR code.
- Examples illustrating how source code translates into IR.

---

## Table of Contents

- [Intermediate Code Generation in the Argonaut Compiler](#intermediate-code-generation-in-the-argonaut-compiler)
  - [Introduction](#introduction)
  - [Table of Contents](#table-of-contents)
  - [AST Node Format in Intermediate Code](#ast-node-format-in-intermediate-code)
  - [Exporting Compiler Data Structures](#exporting-compiler-data-structures)
    - [Common Export Function](#common-export-function)
    - [Exporting the Lexeme Table](#exporting-the-lexeme-table)
    - [Exporting the Declaration Table](#exporting-the-declaration-table)
    - [Exporting the Representation Table](#exporting-the-representation-table)
    - [Exporting the Region Table and AST](#exporting-the-region-table-and-ast)
  - [Formatting the AST in the IR](#formatting-the-ast-in-the-ir)
  - [Sample Exported IR Code](#sample-exported-ir-code)
    - [Original Source Code](#original-source-code)
    - [Exported IR Code](#exported-ir-code)
  - [Understanding the Exported IR Code](#understanding-the-exported-ir-code)
    - [Lexeme Table Interpretation](#lexeme-table-interpretation)
    - [Declaration Table Interpretation](#declaration-table-interpretation)
    - [Region Table and AST Interpretation](#region-table-and-ast-interpretation)

---

## AST Node Format in Intermediate Code

In the Intermediate Representation (IR), each node of the Abstract Syntax Tree (AST) is formatted consistently to ensure clarity and ease of debugging. The format used for representing AST nodes is:

```plaintext
Node(<type>, <index_lexicographic>, <index_declaration>) [Child: <child>] [Sibling: <sibling>]
```

- `<type>`: The type of the AST node (e.g., identifier, operation).
- `<index_lexicographic>`: Index into the lexeme table for the node's lexeme.
- `<index_declaration>`: Index into the declaration table if the node is associated with a declaration.
- `[Child: <child>]`: (Optional) Represents the child node(s) of the current node.
- `[Sibling: <sibling>]`: (Optional) Represents the sibling node(s) at the same level.

**Example:**

```plaintext
Node(6, -1, -1) [Child: Node(23, -1, -1) [Sibling: Node(33, -1, -1)]]
```

This representation provides a hierarchical view of the AST, showing parent-child relationships and sibling connections.

---

## Exporting Compiler Data Structures

The compiler maintains several key data structures during compilation:

- **Lexeme Table**: Stores lexemes (tokens) extracted from the source code.
- **Declaration Table**: Holds information about declarations (variables, functions, types).
- **Representation Table**: Contains representations for various language constructs.
- **Region Table**: Manages scopes and regions within the source code.
- **Abstract Syntax Tree (AST)**: Represents the syntactic structure of the source code.

To generate the IR, these data structures are exported into a specific format that encapsulates the necessary information.

### Common Export Function

The `export_table` function is a generic utility used to export different tables:

```c
void export_table(const char* filename, 
                  void* table, 
                  size_t table_size, 
                  size_t element_size, 
                  char* (*format_row)(void*), 
                  const char* begin_marker, 
                  const char* end_marker) {
    // Function implementation
}
```

- `filename`: The file to which the table will be appended.
- `table`: Pointer to the table data.
- `table_size`: Number of elements in the table.
- `element_size`: Size of each element in the table.
- `format_row`: Function pointer to format a table row.
- `begin_marker` and `end_marker`: Markers indicating the start and end of the table in the file.

---

### Exporting the Lexeme Table

The lexeme table stores tokens from the source code. Each entry contains the lexeme string and associated metadata.

**Code Snippet:**

```c
static char* format_lexeme_row(void* data) {
    Lexeme* row = (Lexeme*)data;
    char* formatted_row = (char*)malloc(512 * sizeof(char));
    sprintf(formatted_row, "%s|%d|%d", row->lexeme, row->length, row->next);
    return formatted_row;
}

void export_lexeme_table(const char* filename) {
    export_table(filename, 
                 lexeme_table, 
                 lexeme_table_size, 
                 sizeof(Lexeme), 
                 format_lexeme_row, 
                 "BEGIN_LEXEME_TABLE", 
                 "END_LEXEME_TABLE");
}
```

- **Fields:**
  - `lexeme`: The string representation of the token.
  - `length`: The length of the lexeme.
  - `next`: Index to the next lexeme if applicable.

**Export Format Example:**

```plaintext
BEGIN_LEXEME_TABLE
|0|int|3|-1|
|1|float|5|-1|
END_LEXEME_TABLE
```

---

### Exporting the Declaration Table

The declaration table holds information about identifiers declared in the source code.

**Code Snippet:**

```c
static char* format_declaration_row(void* data) {
    Declaration* declaration = (Declaration*)data;
    char* formatted_row = (char*)malloc(256 * sizeof(char));
    if (declaration->nature == (Nature)NULL_VALUE) return NULL;
    sprintf(formatted_row, "%d|%d|%d|%d|%d", 
            declaration->nature, 
            declaration->next, 
            declaration->region, 
            declaration->description, 
            declaration->execution);
    return formatted_row;
}

void export_declaration_table(const char* filename) {
    export_table(filename, 
                 declaration_table, 
                 MAX_DECLARATION_COUNT, 
                 sizeof(Declaration), 
                 format_declaration_row, 
                 "BEGIN_DECLARATION_TABLE", 
                 "END_DECLARATION_TABLE");
}
```

- **Fields:**
  - `nature`: The nature of the declaration (e.g., variable, function).
  - `next`: Index to the next declaration in the scope.
  - `region`: The scope or region index where the declaration is valid.
  - `description`: Index to the representation in the representation table.
  - `execution`: Execution-related metadata.

**Export Format Example:**

```plaintext
BEGIN_DECLARATION_TABLE
|0|0|-1|0|0|1|
|1|0|-1|0|1|1|
END_DECLARATION_TABLE
```

---

### Exporting the Representation Table

The representation table holds additional information about types and structures.

**Code Snippet:**

```c
static char* format_representation_row(void* element) {
    int* representation = (int*)element;
    char* formatted_row = (char*)malloc(10 * sizeof(char));
    sprintf(formatted_row, "%d", *representation);
    return formatted_row;
}

void export_representation_table(const char* filename) {
    export_table(filename, 
                 representation_table, 
                 representation_table_size, 
                 sizeof(int), 
                 format_representation_row, 
                 "BEGIN_REPRESENTATION_TABLE", 
                 "END_REPRESENTATION_TABLE");
}
```

- **Fields:** Each row is a single integer value representing a type or structure.

**Export Format Example:**

```plaintext
BEGIN_REPRESENTATION_TABLE
|0|42|
|1|43|
END_REPRESENTATION_TABLE
```

---

### Exporting the Region Table and AST

Regions represent scopes within the source code, and the AST captures the syntactic structure.

**Code Snippet:**

```c
static char* format_region_row(void* element) {
    Region* region = (Region*)element;
    char* formatted_row = (char*)malloc(512 * sizeof(char));
    sprintf(formatted_row, "%d|%d|", region->size, region->nis);
    char* ast_data = format_ast(region->ast);
    if (ast_data) {
        formatted_row = realloc(formatted_row, strlen(formatted_row) + strlen(ast_data) + 10);
        strcat(formatted_row, ast_data);
        free(ast_data);
    }
    return formatted_row;
}

void export_region_table(const char* filename) {
    export_table(filename, 
                 region_table, 
                 region_table_size, 
                 sizeof(Region), 
                 format_region_row, 
                 "BEGIN_REGION_TABLE", 
                 "END_REGION_TABLE");
}
```

- **Fields:**
  - `size`: The size of the region.
  - `nis`: The nesting level.
  - `ast`: The AST associated with the region.

---

## Formatting the AST in the IR

The AST is recursively traversed and formatted using the `format_ast` function:

- **Node Formatting:** Each node is formatted as `Node(type, index_lexicographic, index_declaration)`.
- **Child and Sibling Traversal:** The function recursively formats child and sibling nodes, appending them to the current node's representation.

---

## Sample Exported IR Code

### Original Source Code

```argonaut
print("Hello, World!\n");
```

### Exported IR Code

```plaintext
BEGIN_LEXEME_TABLE
|0|int|3|-1|
|1|float|5|-1|
|2|bool|4|-1|
|3|char|4|-1|
|4|"Hello, World!\n"|17|-1|
END_LEXEME_TABLE
BEGIN_DECLARATION_TABLE
|0|0|-1|0|0|1|
|1|0|-1|0|1|1|
|2|0|-1|0|2|1|
|3|0|-1|0|3|1|
END_DECLARATION_TABLE
BEGIN_REPRESENTATION_TABLE
END_REPRESENTATION_TABLE
BEGIN_REGION_TABLE
|0|0|0|Node(6, -1, -1) [Child: Node(23, -1, -1) [Sibling: Node(33, -1, -1) [Child: Node(52, -1, -1) [Child: Node(5, 4, -1)]]]]|
END_REGION_TABLE
```

---

## Understanding the Exported IR Code

Let's break down the exported IR code and understand how it represents the original source code.

### Lexeme Table Interpretation

The lexeme table contains the tokens extracted from the source code.

**Entries:**

```plaintext
|0|int|3|-1|
|1|float|5|-1|
|2|bool|4|-1|
|3|char|4|-1|
|4|"Hello, World!\n"|17|-1|
```

- **Index 0:** Lexeme `int`, length `3`.
- **Index 1:** Lexeme `float`, length `5`.
- **Index 2:** Lexeme `bool`, length `4`.
- **Index 3:** Lexeme `char`, length `4`.
- **Index 4:** Lexeme `"Hello, World!\n"`, length `17`.

**Interpretation:**

- These entries seem to include some default or built-in types (`int`, `float`, `bool`, `char`).
- Entry at index `4` corresponds to the string literal in the source code.

### Declaration Table Interpretation

The declaration table lists declarations found in the source code.

**Entries:**

```plaintext
|0|0|-1|0|0|1|
|1|0|-1|0|1|1|
|2|0|-1|0|2|1|
|3|0|-1|0|3|1|
```

**Fields:**

- **Index:** Position in the declaration table.
- **nature:** Nature of the declaration (e.g., variable, function).
- **next:** Next declaration index in the scope.
- **region:** Region index (scope level).
- **description:** Index to representation or additional info.
- **execution:** Execution-related metadata.

**Interpretation:**

- Since nature is `0` for all entries and region is `-1`, these may represent global or built-in declarations.
- The `description` field matches the indices of built-in types or lexemes.

### Region Table and AST Interpretation

**Region Table Entry:**

```plaintext
|0|0|0|Node(6, -1, -1) [Child: Node(23, -1, -1) [Sibling: Node(33, -1, -1) [Child: Node(52, -1, -1) [Child: Node(5, 4, -1)]]]]|
```

- **Fields:**
  - `0`: Region index.
  - `0`: Size of the region.
  - `0`: Nesting level (NIS).
  - AST representation starting with `Node(6, -1, -1)`.

**AST Interpretation:**

Let's interpret the AST nodes:

1. **Node(6, -1, -1):** Root node.
   - **Type `6`**: Could represent a statement list or program root.
   - No lexeme or declaration index associated.

2. **Child: Node(23, -1, -1):**
   - **Type `23`**: Possibly a statement or print function call.
   - Again, no lexeme or declaration index.

3. **Sibling: Node(33, -1, -1):**
   - **Type `33`**: Could represent an expression or argument list.
   - Child nodes continue down the tree.

4. **Further down: Node(5, 4, -1):**
   - **Type `5`**: Likely represents a string literal.
   - **Lexeme index `4`**: Refers to `"Hello, World!\n"` in the lexeme table.

**Connecting to the Source Code:**

- The AST represents the `print("Hello, World!\n");` statement.
- The string literal is correctly referenced in the lexeme table.
- The hierarchy indicates a print statement with an argument list containing the string.
