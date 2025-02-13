# Abstract Syntax Tree (AST)

The Abstract Syntax Tree (AST) is a pivotal data structure in the Argonaut Compiler, representing the syntactic and semantic structure of a source program. It is constructed during the parsing phase and is subsequently used for semantic analysis and code generation. Below, we provide a comprehensive guide on how the AST is constructed and managed in the Argonaut Compiler.

## Overview

The AST is a hierarchical tree structure where each node represents a construct occurring in the source code, such as a variable declaration, a function call, or an operator. It provides a higher-level abstraction than the raw token stream produced by the lexical analysis.

### Core Components

1. **Nodes:** Each node in the AST corresponds to a syntactic construct.
2. **Node Types:** Each node has a specific type (e.g., `A_VARIABLE_DECLARATION`, `A_FUNCTION_CALL`) indicating its role.
3. **Child-Sibling Representation:** The AST uses the Left-Child Right-Sibling (LCRS) representation to allow arbitrary branching while maintaining efficient traversal.
4. **Symbol Tables:** Nodes may reference entries in the symbol tables (e.g., declaration table, lexeme table) for semantic information.

---

## Key Data Structures

### Node Structure

Each node in the AST is defined as follows:

```c
typedef struct Node {
    struct Node* child;            // Pointer to the left child
    struct Node* sibling;          // Pointer to the right sibling
    
    NodeType type;                 // Node type (operator, variable, literal, etc.)
    int index_lexicographic;       // Index in the lexicographic table
    int index_declaration;         // Index in the declaration table
} Node;
```

### Node Types

We defined a **flexible mechanism** for managing and enumerating **node types** in a syntax tree, commonly used in compilers or interpreters. Here's a breakdown of its functionality:

---

#### **1. Defining Node Types with Macros**

The `FOREACH_NODE_TYPE` macro lists all node types. Each node type represents a specific construct in a programming language (e.g., literals, operations, control flow, declarations).

Example:

```c
NODE(A_INTEGER_LITERAL) 
NODE(A_ADD_OP)
NODE(A_IF)
```

This single macro acts as a central repository for all node types, avoiding repetitive definitions and making it easy to add or modify types.

---

#### **2. Generating Enum Values**

The `FOREACH_NODE_TYPE` macro is used to generate the values of the `NodeType` enum:

```c
#define GENERATE_ENUM(ENUM) ENUM,
typedef enum {
    FOREACH_NODE_TYPE(GENERATE_ENUM)
} NodeType;
```

This translates to:

```c
typedef enum {
    A_INTEGER_LITERAL,
    A_FLOAT_LITERAL,
    A_BOOLEAN_LITERAL,
    ...
} NodeType;
```

The benefit:

- Adding or removing a node type requires editing only the `FOREACH_NODE_TYPE` macro.
- No need to manually update the `NodeType` enum.

---

#### **3. Generating String Representations**

The `NodeTypeStrings` array is generated using another macro:

```c
#define GENERATE_STRING(STRING) #STRING,
static const char *NodeTypeStrings[] = {
    FOREACH_NODE_TYPE(GENERATE_STRING)
};
```

This converts node types like `A_INTEGER_LITERAL` into readable strings (`"A_INTEGER_LITERAL"`) for debugging or visualization.

Example:

```c
NodeTypeStrings[A_INTEGER_LITERAL] == "A_INTEGER_LITERAL"
```

---

## AST Construction Process

The construction of the AST is an essential part of the compilation process. It starts during the parsing phase, where the source code is analyzed according to the grammar rules of the language. The primary goal is to transform the linear sequence of tokens (produced during lexical analysis) into a hierarchical structure (the AST) that captures both the syntax and semantics of the program.

---

### Initialization of the AST

When the parsing process begins, the AST is initially empty. Think of it as a blank canvas waiting for nodes to be added. Each node represents a syntactic construct, and together they form a tree that reflects the nested, hierarchical structure of the source code.

For example:

- A program may consist of functions.
- Functions may consist of statements.
- Statements may include expressions, loops, or assignments.

At the start, the AST has no nodes because the parser has not yet encountered any constructs.

---

### Child-Sibling Representation

The AST uses the Left-Child Right-Sibling (LCRS) representation, which simplifies the structure by allowing each node to have at most two pointers:

1. A pointer to its first child (representing the first element in a nested structure).
2. A pointer to its next sibling (representing adjacent constructs).

This structure makes it possible to represent constructs with arbitrary branching (e.g., a function with many parameters) while maintaining a simple and efficient memory layout.

---

### Constructing Nodes

Nodes are created dynamically as the parser encounters constructs in the source code. For each syntactic rule in the grammar:

- A new node is created.
- The node is populated with information like its type and links to symbol tables.
- Child and sibling pointers are adjusted to reflect the tree's structure.

In the Argonaut code a node can be created using the ```construct_node``` function :

```c
Node* construct_node(NodeType type, int index_lexicographic, int index_declaration) {
    Node* new_node = (Node*) malloc(sizeof(Node));
    if (!new_node) {
        perror("Failed to allocate memory for AST node");
        exit(EXIT_FAILURE);
    }

    new_node->type = type;
    new_node->index_lexicographic = index_lexicographic;
    new_node->index_declaration = index_declaration;
    
    new_node->child = NULL;
    new_node->sibling = NULL;

    return new_node;
}
```

Default nodes can be created without specific indices:

```c
Node* construct_node_default(NodeType type) {
    return construct_node(type, NULL_VALUE, NULL_VALUE);
}
```

#### Integration in Grammar

For example, when the parser encounters a variable declaration, it creates a node of type ``A_VARIABLE_DECLARATION`` and links it to the corresponding identifier and type information from the symbol tables.

```c
variable_declaration: VARIABLE IDENTIFIER TWO_POINTS type SEMICOLON {
                        declaration_variable_start($2, $4);
                        $$ = construct_node(A_VARIABLE_DECLARATION, $2, find_declaration_index($4));
                    }
;
```

This rule:

1. Starts the variable declaration process.
2. Creates an `A_VARIABLE_DECLARATION` node with the identifier and type indices.
3. Links the node appropriately in the AST.

---

### Building the Tree Structure

The AST grows as nodes are linked together to form subtrees. This happens recursively:

1. Adding Children: When a construct has nested components, child nodes are added to represent them. For example:
    - An if statement node may have a child node for its condition and another for its body.

2. Adding Siblings: When constructs occur sequentially, sibling nodes are added to represent them. For example:
    - In a block of statements, each statement is added as a sibling of the previous one.

The tree structure captures the nesting and ordering of constructs in the program.

We use the defined functions to add children and siblings:

- **Add a Child:**

```c
void add_child(Node* parent, Node* child) {
    if (!parent || !child) return;
    child->sibling = parent->child;
    parent->child = child;
}
```

- **Add a Sibling:**

```c
void add_sibling(Node* node, Node* sibling) {
    if (!node || !sibling) return;
    while (node->sibling) {
        node = node->sibling;
    }
    node->sibling = sibling;
}
```

- **Append a Child or Sibling:**

For scenarios requiring sequential additions:

```c
void append_child(Node* parent, Node* child);
void append_sibling(Node* node, Node* sibling);
```

### Subtree Construction

Complex constructs often require constructing subtrees before integrating them into the larger AST (e.g. the ```function_declaration```
and ```procedure_declaration``` rules). The subtree construction process ensures that even deeply nested constructs are represented correctly and efficiently.

Subtrees can be constructed by chaining nodes using `add_chain`. For example:

```c
void add_chain(Node* parent, Node* nodes[], int count) {
    Node* current = NULL;
    for (int i = 0; i < count; i++) {
        if (!is_node_null(nodes[i])) {
            if (current == NULL) {
                add_child(parent, nodes[i]);
                current = nodes[i];
            } else {
                add_sibling(current, nodes[i]);
                current = nodes[i];
            }
        }
    }
}
```

---

## Integration with the Parser

The AST construction is tightly integrated with the parser, which is generated from the grammar file (``src/parser/grammar.y``). Each rule in the grammar corresponds to a specific syntactic construct, and the associated code creates and links the necessary nodes.

For example:

- When the parser encounters a rule for a function declaration, it creates a node for the function and links nodes for its parameters, body, and return type.

The parser essentially "walks" through the source code, building the AST piece by piece according to the language's grammar.

### **Example: Function Declaration**

When the parser encounters a function declaration, it creates a node for the function and links nodes representing its parameters, body, and return type. Below is the grammar rule for a function declaration with detailed comments explaining each step:

```c
function_declaration:
    // Step 1: Recognize the 'FUNCTION' keyword and the function identifier
    FUNCTION IDENTIFIER {
        construct_func_proc_manager_context($2);   // Initialize context for the function
        declaration_func_start();                 // Mark the start of a function declaration
    }

    // Step 2: Parse the function's parameter list
    OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS RETURN_TYPE type {
        update_declaration_func_return_type($8);  // Set the return type for the function in the symbol table
    }

    // Step 3: Parse the function's body, including declarations and statements
    START declaration_list statement_list END {
        // Create a new node for the function declaration
        $$ = construct_node(A_FUNCTION_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_FUNC));

        // Create an array of child nodes: parameters, declarations, and statements
        Node *nodes[] = { $5, $11, $12 };

        // Link the child nodes to the function node
        add_chain($$, nodes, 3);

        // Update the region's AST with the constructed function node
        update_region_ast(peek_region(), $$);

        // Finalize the function declaration context
        declaration_func_proc_end();

        // Reconstruct the function declaration node for the region (if needed)
        $$ = construct_node(A_FUNCTION_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_FUNC));
    }
;
```

---

#### **Step-by-Step Explanation**

1. **Step 1: Recognizing the Function Start**
   - The parser identifies the `FUNCTION` keyword followed by the function identifier (`IDENTIFIER`).
   - The semantic action initializes a **function/procedure context** (`construct_func_proc_manager_context`) to manage attributes like parameters and return types.
   - The `declaration_func_start()` function marks the beginning of a new function declaration in the symbol table.

   ```c
   construct_func_proc_manager_context($2);   // Prepare function-specific data
   declaration_func_start();                 // Initialize function declaration process
   ```

2. **Step 2: Parsing the Parameters and Return Type**
   - The function's parameter list is parsed between `OPEN_PARENTHESIS` and `CLOSE_PARENTHESIS`.
   - After parsing `RETURN_TYPE` and the `type` keyword, the semantic action updates the function's return type in the symbol table (`update_declaration_func_return_type`).

   ```c
   update_declaration_func_return_type($8);  // Record the return type in the function's symbol table entry
   ```

3. **Step 3: Parsing the Function Body**
   - The function body starts with `START` and ends with `END`, containing a list of declarations (`declaration_list`) and executable statements (`statement_list`).
   - A new node (`A_FUNCTION_DECLARATION`) is created to represent the function in the AST:
     - `$2` is the **identifier** for the function name.
     - `find_declaration_index_by_nature` fetches the declaration index for the function.

   ```c
   $$ = construct_node(A_FUNCTION_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_FUNC));
   ```

   - An array of child nodes is created to represent the function's parameters, declarations, and statements:
     - `$5`: Represents the parameter list.
     - `$11`: Represents declarations inside the function body.
     - `$12`: Represents the statements inside the function body.

   ```c
   Node *nodes[] = { $5, $11, $12 };
   ```

   - The `add_chain` function links these child nodes to the function node.

   ```c
   add_chain($$, nodes, 3);  // Link the child nodes to the parent function node
   ```

   - The constructed function node is added to the current **region's AST**.

   ```c
   update_region_ast(peek_region(), $$);  // Update the region's AST with this function node
   ```

4. **Finalization**
   - The `declaration_func_proc_end()` function finalizes the function declaration context, ensuring all necessary data is stored and indexed correctly.
   - If required, the function declaration node is reconstructed for further usage in the region.

   ```c
   declaration_func_proc_end();  // Finalize the function declaration
   $$ = construct_node(A_FUNCTION_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_FUNC));
   ```

---

#### **AST Representation for a Function**

For a function `foo` with the following structure:

```c
func foo(x: int) -> int {
    var y: int;
    y := x + 1;

    return y;
}
```

The constructed AST might look like this (simplified):

```text
├── A_FUNCTION_DECLARATION, Lexico Idx: 4, Decl Idx: 4 -- Lexeme: 'foo'
│   ├── A_PARAMETER_LIST, Lexico Idx: -1, Decl Idx: -1
│   │   ├── A_PARAMETER, Lexico Idx: 5, Decl Idx: 5 -- Lexeme: 'x'
│   └── A_DECLARATION_LIST, Lexico Idx: -1, Decl Idx: -1
│   │   ├── A_VARIABLE_DECLARATION, Lexico Idx: 6, Decl Idx: 6 -- Lexeme: 'y'
│   └── A_STATEMENT_LIST, Lexico Idx: -1, Decl Idx: -1
│   │   ├── A_ASSIGNMENT_STATEMENT, Lexico Idx: -1, Decl Idx: -1
│   │   │   ├── A_VARIABLE_ASSIGNMENT, Lexico Idx: 6, Decl Idx: 6 -- Lexeme: 'y'
│   │   │   │   ├── A_ADD_OP, Lexico Idx: -1, Decl Idx: -1
│   │   │   │   │   ├── A_IDENTIFIER, Lexico Idx: 5, Decl Idx: 5 -- Lexeme: 'x'
│   │   │   │   │   └── A_INTEGER_LITERAL, Lexico Idx: 7, Decl Idx: -1 -- Lexeme: '1'
│   │   └── A_RETURN_STATEMENT, Lexico Idx: -1, Decl Idx: -1
│   │   │   ├── A_IDENTIFIER, Lexico Idx: 6, Decl Idx: 6 -- Lexeme: 'y'
```

---

By tightly coupling AST construction with the parser, the compiler efficiently builds a hierarchical structure representing the program. This ensures that each syntactic construct is accurately captured and linked for subsequent compilation phases.
