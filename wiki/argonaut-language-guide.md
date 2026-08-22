# Argonaut Language Guide

## Introduction

Argonaut is a statically-typed, procedural programming language designed for simplicity and clarity. It emphasizes strict syntax rules, structured programming constructs, and clear code organization. This documentation provides a comprehensive overview of the Argonaut language, including its syntax, data types, control flow statements, functions and procedures, arrays, structures, and input/output operations.

> [!IMPORTANT]
> Examples can be found inside the ``examples/interpretation/functional/`` directory.

---

## Table of Contents

- [Argonaut Language Guide](#argonaut-language-guide)
  - [Introduction](#introduction)
  - [Table of Contents](#table-of-contents)
  - [Basic Syntax](#basic-syntax)
  - [Program Structure](#program-structure)
  - [Data Types](#data-types)
  - [Variables](#variables)
    - [Variable Declaration](#variable-declaration)
    - [Variable Initialization](#variable-initialization)
  - [Operators and Expressions](#operators-and-expressions)
    - [Arithmetic Expressions](#arithmetic-expressions)
    - [Boolean Expressions](#boolean-expressions)
  - [Control Flow Statements](#control-flow-statements)
    - [Conditional Statements](#conditional-statements)
      - [If Statement](#if-statement)
      - [If-Else Statement](#if-else-statement)
      - [If-Else If-Else Statement](#if-else-if-else-statement)
    - [Loop Statements](#loop-statements)
      - [While Loop](#while-loop)
      - [For Loop](#for-loop)
      - [Break and Continue Statements](#break-and-continue-statements)
  - [Functions and Procedures](#functions-and-procedures)
    - [Function Declaration](#function-declaration)
    - [Procedure Declaration](#procedure-declaration)
    - [Parameters and Arguments](#parameters-and-arguments)
    - [Return Statement](#return-statement)
    - [Nested Functions](#nested-functions)
    - [Function and Procedure Calls](#function-and-procedure-calls)
  - [Arrays](#arrays)
    - [Array Declaration](#array-declaration)
    - [Array Access](#array-access)
    - [Multidimensional Arrays](#multidimensional-arrays)
  - [Structures](#structures)
    - [Structure Declaration](#structure-declaration)
    - [Structure Access](#structure-access)
    - [Nested Structures](#nested-structures)
  - [Input and Output](#input-and-output)
    - [Printing to Console](#printing-to-console)
    - [Reading Input](#reading-input)
  - [Examples](#examples)
    - [Factorial Function](#factorial-function)
    - [Nested Structures Example](#nested-structures-example)
    - [Array Handling](#array-handling)
    - [Input and Output Example](#input-and-output-example)

---

## Basic Syntax

- **Statements** end with a semicolon `;`.
- **Comments** start with `//` for single-line comments. There is no support for multi-line comments.
- **Blocks** are enclosed within curly braces `{` and `}`.
- **Identifiers** must start with a letter and can contain letters, digits, and underscores.
- **Keywords** are reserved words and cannot be used as identifiers (e.g. `if`, `else`, `while`, `for`, etc.).
- **Indentation** is not syntactically significant but should be used for readability.

---

## Program Structure

An Argonaut program consists of a sequence of declarations followed by a sequence of statements.

```argonaut
program: declaration_list statement_list
```

**Example Structure:**

```argonaut
// Declarations
var x : int;
func add(a : int, b : int) -> int {
    return a + b;
}

// Statements
x := add(5, 3);
print("Result: %d\n", x);
```

---

## Data Types

Argonaut supports the following data types:

- **Primitive Types:**
  - `int` : Integer numbers
  - `float` : Floating-point numbers
  - `char` : Single characters enclosed in single quotes (e.g., `'a'`)
  - `bool` : Boolean values (`true` or `false`)
  - `string` : Strings of characters enclosed in double quotes (e.g., `"Hello"`)

> [!WARNING]
> ``string`` is not fully supported and works only for printing. For example `print("Hello, %s\n", "World")`
  
- **Composite Types:**
  - **Arrays** : Ordered collections of elements of the same type.
  - **Structures (struct)** : User-defined types grouping variables of different types.

- **Custom Types:**
  - Defined using the `type` keyword for arrays and structures.

---

## Variables

### Variable Declaration

Variables are declared using the `var` keyword followed by the identifier, a colon `:`, the data type, and a terminating semicolon `;`.

```argonaut
var variableName : dataType;
```

**Example:**

```argonaut
var count : int;
var pi : float;
var letter : char;
var isReady : bool;
```

### Variable Initialization

Variables are initialized using the assignment operator `:=`.

```argonaut
variableName := value;
```

**Example:**

```argonaut
count := 10;
pi := 3.14;
letter := 'A';
isReady := true;
```

---

## Operators and Expressions

### Arithmetic Expressions

Argonaut supports standard arithmetic operators for numerical computations.

- Addition: `+`
- Subtraction: `-`
- Multiplication: `*`
- Division: `/`
- Modulo: `%`
- Unary minus: `-x`

Precedence, from highest to lowest: unary `-`, then `* / %`, then `+ -`. Both operands of an operation must have the same type (`int` with `int`, `float` with `float`). An integer overflow is a run-time error.

**Example:**

```argonaut
result := a + b - c * d / e % f;
negated := -(a + b);
```

### Boolean Expressions

Boolean expressions are used in conditional statements and loops.

- **Relational Operators:**
  - Equal to: `==`
  - Not equal to: `!=`
  - Less than: `<`
  - Greater than: `>`
  - Less than or equal to: `<=`
  - Greater than or equal to: `>=`

- **Logical Operators:**
  - Logical AND: `&&`
  - Logical OR: `||`
  - Logical NOT: `!`

A comparison or a logical operation is an expression of type `bool`: it can be stored in a `bool` variable, returned by a function, printed with `%b`, and any `bool` expression can be used directly as a condition. Precedence, from highest to lowest: the comparisons, then `!`, then `&&`, then `||`. `&&` and `||` short-circuit: the right operand is not evaluated when the left one decides the result.

**Example:**

```argonaut
var ready : bool;
ready := x > 0 && y < 5;
if (ready) {
    // Code block
}
if (!ready || x == 0) {
    // Code block
}
```

---

## Control Flow Statements

### Conditional Statements

#### If Statement

Executes a block of code if the condition evaluates to true.

```argonaut
if (condition) {
    // Code block
}
```

**Example:**

```argonaut
if (score >= 60) {
    print("Passed\n");
}
```

#### If-Else Statement

Provides an alternative block if the condition evaluates to false.

```argonaut
if (condition) {
    // Code block if true
} else {
    // Code block if false
}
```

**Example:**

```argonaut
if (score >= 60) {
    print("Passed\n");
} else {
    print("Failed\n");
}
```

#### If-Else If-Else Statement

Supports multiple conditions.

```argonaut
if (condition1) {
    // Code block if condition1 is true
} else if (condition2) {
    // Code block if condition2 is true
} else {
    // Code block if all conditions are false
}
```

**Example:**

```argonaut
if (score >= 90) {
    print("Grade: A\n");
} else if (score >= 80) {
    print("Grade: B\n");
} else if (score >= 70) {
    print("Grade: C\n");
} else {
    print("Grade: D\n");
}
```

### Loop Statements

#### While Loop

Repeats a block of code while the condition is true.

```argonaut
while (condition) {
    // Code block
}
```

**Example:**

```argonaut
i := 0;
while (i < 10) {
    print("%d ", i);
    i := i + 1;
}
```

#### For Loop

Used for iterating over a range with an initializer, a condition, and an increment.

```argonaut
for (initialization; condition; increment) {
    // Code block
}
```

**Example:**

```argonaut
for (i := 0; i < 10; i++) {
    print("%d ", i);
}
```

#### Break and Continue Statements

- **Break Statement**: Exits the nearest enclosing loop.

  ```argonaut
  break;
  ```

- **Continue Statement**: Skips to the next iteration of the loop.

  ```argonaut
  continue;
  ```

**Example with Break:**

```argonaut
for (i := 0; i < 10; i++) {
    if (i == 5) {
        break;
    }
    print("%d ", i);
}
```

**Example with Continue:**

```argonaut
for (i := 0; i < 10; i++) {
    if (i % 2 == 0) {
        continue;
    }
    print("%d ", i);
}
```

---

## Functions and Procedures

Argonaut distinguishes between functions and procedures.

- **Functions** return a value and are defined using the `func` keyword.
- **Procedures** do not return a value and are defined using the `proc` keyword.

### Function Declaration

```argonaut
func functionName(parameter_list) -> returnType {
    // Declarations
    // Statements
}
```

**Example:**

```argonaut
func factorial(n : int) -> int {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
```

### Procedure Declaration

```argonaut
proc procedureName(parameter_list) {
    // Declarations
    // Statements
}
```

**Example:**

```argonaut
proc print_message(message : string) {
    print("%s\n", message);
}
```

> [!WARNING]
> `string` parameters and variables are **not implemented yet**: this example does not compile today (`tests/lang/bugs/b24_wiki_string_param.arn` tracks it). Only string *literals* passed to `print` work.

### Parameters and Arguments

- **Parameters** are declared within parentheses and include the parameter name and type.
- **Arguments** are the expressions or values passed to functions or procedures.

**Example:**

```argonaut
func add(a : int, b : int) -> int {
    return a + b;
}
```

**Calling the function:**

```argonaut
result := add(5, 3);
```

### Return Statement

A function must return a value on every path: the compiler rejects a function whose body can reach its end without a `return` (an `if`/`else` whose two branches return is fine, a loop does not count).

```argonaut
return expression;
```

**Example:**

```argonaut
func square(n : int) -> int {
    return n * n;
}
```

In a procedure, `return;` (without a value) leaves the procedure early.

```argonaut
proc show(n : int) {
    if (n < 0) { return; }
    print("%d\n", n);
}
```

### Nested Functions

Functions can be nested within other functions or procedures.

**Example:**

```argonaut
func outerFunction(value : int) -> int {
    func innerFunction(x : int) -> int {
        return x * 2;
    }
    return innerFunction(value) + value;
}
```

### Function and Procedure Calls

Functions and procedures are called using their name followed by arguments enclosed in parentheses.

```argonaut
functionName(argument_list);
```

**Example:**

```argonaut
result := factorial(5);
print_message("Hello, World!");
```

---

## Arrays

### Array Declaration

Arrays are declared using the `type` keyword to define a custom array type. You must specify the index range and the data type of the elements.

```argonaut
type arrayName : array[startIndex:endIndex] of dataType;
```

**Example:**

```argonaut
type intArray : array[0:9] of int;
var numbers : intArray;
```

### Array Access

Access elements using square brackets `[]` with the index. The number and the type of the indices are checked at compile time, the range at run time.

```argonaut
numbers[0] := 10;
value := numbers[0];
numbers[1]++;
```

Accesses chain freely: an element that is a structure can be followed by `.field`, a field that is an array by `[index]`, and an array of arrays is indexed level by level.

```argonaut
type Row : array[0:2] of int;
type Grid : array[0:2] of Row;
var g : Grid;
g[1][2] := 5;       // the same as g[1, 2]
people[i].name[0] := 'A';
```

### Multidimensional Arrays

Arrays can have multiple dimensions.

```argonaut
type matrix : array[0:2, 0:2] of int;
var grid : matrix;
```

**Accessing Elements:**

```argonaut
grid[0, 0] := 1;
value := grid[1, 2];
```

---

## Structures

### Structure Declaration

Structures are user-defined types that group related variables.

```argonaut
type StructName : struct {
    field1 : dataType;
    field2 : dataType;
    // Additional fields
} fstruct;
```

**Example:**

```argonaut
type Point : struct {
    x : int;
    y : int;
} fstruct;
```

### Structure Access

Access structure fields using the dot `.` operator; nested fields and array fields chain the same way (`r.topLeft.x`, `s.values[i]`).

```argonaut
var p : Point;
p.x := 10;
p.y := 20;
p.x++;
```

### Nested Structures

Structures can contain other structures or arrays.

**Example:**

```argonaut
type Rectangle : struct {
    topLeft : Point;
    bottomRight : Point;
} fstruct;
```

---

## Input and Output

### Printing to Console

Use the `print` procedure with a format string and corresponding arguments.

```argonaut
print(formatString, argument_list);
```

**Format Specifiers:**

- `%d` : Integer (a `bool` is printed as 0 or 1)
- `%f` : Floating-point number
- `%c` : Character
- `%b` : Boolean, printed as `true` or `false`
- `%s` : String literal
- `%%` : A literal percent sign

String and character literals accept the escape sequences `\n`, `\t`, `\r`, `\\`, `\'` and `\"`.

**Example:**

```argonaut
print("The value is %d (%b), 100%% done\n", value, value > 0);
```

### Reading Input

Use the `input` procedure to read user input.

```argonaut
input(formatString, variable_list);
```

**Example:**

```argonaut
var name : string;
print("Enter your name: ");
input("%s", name);
```

> [!WARNING]
> Reading a `string` is **not implemented yet** (`tests/lang/bugs/b25_wiki_input_string.arn` tracks it). `input` works with `%d`, `%f`, `%c` and `%b` targets.

---

## Examples

### Factorial Function

```argonaut
func factorial(n : int) -> int {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

proc main() {
    var number : int;
    var result : int;
    number := 5;
    result := factorial(number);
    print("Factorial of %d = %d\n", number, result);
}

main();
```

**Output:**

```
Factorial of 5 = 120
```

### Nested Structures Example

```argonaut
// Define 'Point' and 'Rectangle' structures
type Point : struct {
    x : int;
    y : int;
} fstruct;

type Rectangle : struct {
    topLeft : Point;
    bottomRight : Point;
} fstruct;

// Function to calculate the area of a rectangle
func calculate_area(rect : Rectangle) -> int {
    var width : int;
    var height : int;
    width := rect.bottomRight.x - rect.topLeft.x;
    height := rect.bottomRight.y - rect.topLeft.y;
    return width * height;
}

proc print_rectangle(rect : Rectangle) {
    print("Top-left corner: (%d, %d)\n", rect.topLeft.x, rect.topLeft.y);
    print("Bottom-right corner: (%d, %d)\n", rect.bottomRight.x, rect.bottomRight.y);
    print("Area: %d\n", calculate_area(rect));
}

proc main() {
    var rect : Rectangle;
    rect.topLeft.x := 1;
    rect.topLeft.y := 1;
    rect.bottomRight.x := 4;
    rect.bottomRight.y := 5;
    print_rectangle(rect);
}

main();
```

**Output:**

```
Top-left corner: (1, 1)
Bottom-right corner: (4, 5)
Area: 12
```

### Array Handling

```argonaut
type arr : array[0:4] of int;
var numbers : arr;
var i : int;

for (i := 0; i <= 4; i++) {
    numbers[i] := (i + 1) * 10;
}

for (i := 0; i <= 4; i++) {
    print("numbers[%d] = %d\n", i, numbers[i]);
}
```

**Output:**

```
numbers[0] = 10
numbers[1] = 20
numbers[2] = 30
numbers[3] = 40
numbers[4] = 50
```

### Input and Output Example

```argonaut
proc main() {
    var age : int;

    // Prompt the user for their age
    print("Enter your age: ");
    input("%d", age);

    // Display the collected information
    print("\nHello! You are %d years old.\n", age);
}

main();
```

**Sample Interaction:**

```
Enter your age: 25

Hello! You are 25 years old.
```
