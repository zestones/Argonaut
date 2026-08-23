# Argonaut language specification (draft 0.1)

This document describes the language **as implemented today** (revision `ae3f852` plus the build fixes) and lists the design decisions that are still open. It takes precedence over the wiki when they disagree; any change of semantics goes through an update of this file and tests in `tests/lang/`.

Notation: EBNF in the style of the Go specification — `{ X }` repetition (zero or more), `[ X ]` option, `|` alternative, `"…"` terminal.

## 1. Lexical structure

```
identifier    = letter { letter | digit | "_" } .          // cannot start with "_"
integer       = "0" | nonzero_digit { digit } .            // no sign: "-" is the unary minus (D1)
float         = digit { digit } "." { digit } .
boolean       = "true" | "false" .
character     = "'" ( char_except_quote_or_backslash | escape ) "'" .
string        = '"' { char_except_quote_or_backslash | escape } '"' .   // may span several lines
escape        = "\n" | "\t" | "\r" | "\\" | "\'" | "\"" .   // decoded by the lexer (D6)
comment       = "//" … end of line .
```

Reserved words: `var type struct fstruct array of func proc return if else while for break continue print input int float bool char string true false`.

An integer literal must fit in 32 bits; an unknown escape sequence is a lexical error. Current implementation limits (each reported as an explicit error): 8,192 distinct lexemes, 16,384 declarations, 1,024 regions (program + functions + procedures), 3,000 call frames (see D9).

## 2. Grammar

```
program         = { declaration } { statement } .

declaration     = var_decl | type_decl | func_decl | proc_decl .
var_decl        = "var" identifier ":" type ";" .
type            = "int" | "float" | "bool" | "char" | "string" | identifier .
type_decl       = "type" identifier ":" "struct" "{" { field } "}" "fstruct" ";"
                | "type" identifier ":" "array" "[" range { "," range } "]" "of" type ";" .
field           = identifier ":" type ";" .
range           = bound ":" bound .
bound           = [ "-" ] integer .
func_decl       = "func" identifier "(" [ parameters ] ")" "->" type "{" { declaration } { statement } "}" .
proc_decl       = "proc" identifier "(" [ parameters ] ")" "{" { declaration } { statement } "}" .
parameters      = parameter { "," parameter } .
parameter       = identifier ":" type .

statement       = assignment ";" | if | loop | call ";" | print ";" | input ";" | "return" [ expression ] ";" .
assignment      = lvalue ":=" expression | lvalue "++" | lvalue "--" .
lvalue          = identifier | access .
access          = identifier suffix { suffix } .
suffix          = "[" expression { "," expression } "]" | "." identifier .
if              = "if" "(" expression ")" block [ "else" ( block | if ) ] .
loop            = "while" "(" expression ")" block
                | "for" "(" assignment ";" expression ";" assignment ")" block
                | "break" ";" | "continue" ";" .
block           = "{" { statement } "}" .

expression      = expression binary_operator expression
                | "!" expression | "-" expression
                | "(" expression ")" | atom .
binary_operator = "||" | "&&" | "==" | "!=" | "<" | ">" | "<=" | ">=" | "+" | "-" | "*" | "/" | "%" .
atom            = identifier | integer | float | boolean | character | string | call | access .
call            = identifier "(" [ expression { "," expression } ] ")" .
print           = "print" "(" string [ "," expression { "," expression } ] ")" .
input           = "input" "(" string [ "," lvalue { "," lvalue } ] ")" .
```

Precedence, highest to lowest: unary `-`; `* / %`; `+ -`; the comparisons (non-associative); `!`; `&&`; `||`. `&&` and `||` are left associative and short-circuit: the right operand is evaluated only when the result is not already known (D2).

## 3. Semantics

- **Strict static typing.** No implicit conversion: `1 + 2.5` is an error, so is `x := 1.5` for an `int`. Both operands of an arithmetic operation or of a comparison must have the same type.
- **Base types**: `int` (32-bit signed), `float` (single precision in the VM), `bool`, `char`. `string` is recognised by the lexer but **is not implemented** (D5, deferred); string literals are only usable in `print`.
- **Booleans.** A comparison or a logical operation is an expression of type `bool`: it can be stored (`b := x < y`), returned, passed, printed and used as a condition; any expression of type `bool` is a condition (`if (flag)`, `while (!done(i))`). `==` and `!=` accept `int`, `float`, `char` and `bool` operands, the ordering operators everything but `bool` (D3).
- **Unary minus**: `-x`, `-(a + b)`, `-5`; `3 - -2` is 5 (D1).
- **Arrays**: arbitrary inclusive bounds (`array[-2:2]`), several dimensions (`array[0:2, 0:2]`), indices checked at compile time for their number and type and at run time for their range (`Index … out of bounds`). Elements may be structures or arrays. Accesses chain freely: `t[i].f`, `s.t[i]`, `t[i].f[j]`, and an array of arrays is indexed level by level, `g[i][j]` or equivalently `g[i, j]` (D8).
- **Structures**: fields of any type, nesting; access `s.a.b`, `s.t[i]`, `t[i].a`; `a[i]++` and `s.f--` work on any lvalue (D8).
- **Scope**: lexical, by regions; functions and procedures nest to any depth and each level can access the variables of the enclosing levels. Declarations precede statements in every block. A variable may share its name with a type or a function.
- **Functions and procedures**: parameters are passed **by value** (arrays and structures are copied entirely, D12); a function returns a base type only and must return a value on every path, which the compiler checks (an `if`/`else` whose two branches return counts, a loop does not) (D4); `return;` leaves a procedure early (D7). There is no forward declaration, so two routines cannot call each other.
- **Integer overflow** in `+ - * /` and in the unary minus is a run-time error; so are division and modulo by zero (D10).
- **Input/output**: `print(format, …)` with `%d` (an `int`, or a `bool` printed as 0/1), `%f`, `%c`, `%b` (a `bool` printed as `true`/`false`) and `%s` for a string literal; `%%` prints a percent sign; `input(format, …)` reads `%d %f %c %b` into lvalues. Escape sequences are decoded in every literal (D6).
- **Diagnosed run-time errors**: division and modulo by zero, integer overflow, index out of bounds, uninitialised variable, array element or field, call depth above the limit, a read that fails in `input`.

## 4. Design decisions

Decided on 2026-08-22: D1, D2, D3, D4, D6, D7 (early `return;`), D8, D10 and D11 are implemented as recommended below; D12 is kept as it is. **Deferred** (to be added later, when the manipulation is possible): D5 (`string`) and D9 (dynamic tables); forward declarations (D7, second part) are still open. The sections below keep their original context for the record. Everything here is a language or design choice, not a bug: the implementation is consistent with itself, the question is what the language should be. Each decision lists what the code does today and why, the options, a recommendation, the cost of the change and the tests that encode it (a test in `tests/xfail/` flips to `PASS` once the decision is implemented; some tests encode one option and must be rewritten if another is chosen). Summary:

| #   | Question                                                                      | Today                                              | Options                                                                                          | Tests                         |
|-----|-------------------------------------------------------------------------------|----------------------------------------------------|--------------------------------------------------------------------------------------------------|-------------------------------|
| D1  | Is the sign part of the literal? (`5-3` is an error, there is no `-x`)        | **done**: unary minus, unsigned literals | remove `-` from the literals and add the unary minus (recommended)                               | `b01 b02 b03`                 |
| D2  | Precedence of `!`, `&&`, `\|\|` and short-circuit                               | **done**: `!` > `&&` > `\|\|`, short-circuit | C precedences (`!` > `&&` > `||`) and short-circuit (recommended)                                | `b07 b08 l06`                 |
| D3  | Is a boolean a condition? Is a comparison an expression?                      | **done**: booleans are expressions and conditions, `%b` | `if (b)`, `b := x < y`, printing a boolean                                                       | `l01 l02 l03`                 |
| D4  | Function without `return` on some path                                        | **done**: compile-time check, run-time safety net | compile-time error (flow analysis) or run-time error                                             | `b12`                         |
| D5  | `string`                                                                      | **deferred**: not implemented | implement (allocation, copy, `input`, `print`) or remove it from the lexer and the documentation | `b23 b24 b25`                 |
| D6  | Escapes (`'\n'`, `\"`, `%%`)                                                  | **done**: C escapes, `%%` | the same as C for `char` and strings; `%%` in `print`                                            | `b05 l04 l05`                 |
| D7  | `return;` in a procedure, forward declarations                                | **done** for `return;`; forward declarations still open | allow them (mutual recursion)                                                                    | `l10 l11`                     |
| D8  | `t[i].a[j]`, `a[i]++`, arrays of arrays                                       | **done**: uniform access chains | uniform access grammar (chainable `[ ]` and `.` suffixes)                                        | `l07 l08 l12`                 |
| D9  | Limits (1,024 regions, 8,192 lexemes, 16,384 declarations, 3,000 call frames) | **deferred**: fixed-size tables, explicit errors | dynamic tables; remove the call-depth limit (the interpreter recurses on the C stack)            | fixed (`b32 b33 b34 b35 l09`) |
| D10 | Integer overflow at run time                                                  | **done**: run-time error | run-time error, or documented wrap-around modulo 2³²                                             | none yet                      |
| D11 | IR format                                                                     | **done**: `ARGONAUT_IR 1` header, structure validated | version header; binary format later                                                              | `tests/ir/*`                  |
| D12 | Aggregates passed by value                                                    | **kept**: full copy | keep (simple) or add references                                                                  | `s11 s20 b16–b20`             |

### D1 — The sign is part of the literal, there is no unary minus

**Today.** The lexer rules for numbers are `[-]?{number}+` and `[-]?{number}+"."{number}*`, so `-3` is one token. `5-3` lexes as `5` then `-3` and is a syntax error (`b01`), `n-1` likewise (`b02`), and `-x` does not exist because the grammar has no unary operator (`b03`). `3 - -2` works and gives 5 (`s23`). This is the single most surprising behaviour for a new user.

**Options.** (a) Keep it and document "always put spaces around a binary minus; negate with `0 - x`". (b) Drop the `-` from the literal rules and add `'-' expression` to the grammar with a `%prec UMINUS` precedence above `* / %`, producing either a new `A_NEG_OP` node or a rewrite into `0 - e` (no VM change at all with the rewrite; with a node, one case in `arithmetic.c` and one in `expression_resolution.c`).

**Recommendation.** (b) with the `0 - e` rewrite: two lexer lines, one grammar rule, no IR change. Negative literals keep working because `- 3` becomes `0 - 3` and constant folding is not needed for correctness.

**Cost.** Small (an hour). `s23` must keep passing.

### D2 — Precedence of `!`, `&&`, `||` and short-circuit

**Today.** `grammar.y` declares `%right NOT` before `%left AND OR`, so `!` binds loosest and `&&`/`||` share one level, left associative: `(1==1) || (1==2) && (1==2)` is false (`b07`), `!(1==2) && (1==2)` is true (`b08`). The VM evaluates both operands of `&&` and `||` unconditionally, so `i < n && 10 / i > 0` divides by zero when `i` is 0 (`l06`).

**Options.** (a) Keep and document. (b) C/Java/Python semantics: `!` > `&&` > `||`, evaluate the right operand only when needed. (c) C precedences without short-circuit (there is no reason to prefer this).

**Recommendation.** (b): every mainstream language does this and code such as `if (i <= n && t[i] == 0)` depends on it. Three precedence lines in the grammar (`%left OR`, then `%left AND`, then `%right NOT`) and a conditional evaluation of the right operand in `boolean.c`.

**Cost.** Small. Existing programs that relied on the odd precedence would change meaning, but none of the examples or tests does.

### D3 — Booleans as conditions, conditions as expressions

**Today.** The grammar has two disjoint worlds: `condition` (comparisons combined with `&& || !`) and `expression` (arithmetic). A boolean variable cannot be a condition (`if (flag)` is a syntax error, you must write `flag == true`, `l01`), a comparison cannot be stored (`b := 1 < 2` is a type error, `l02`) and nothing prints a boolean (`l03`). The type checker already models `bool` as a base type, so the restriction is purely syntactic.

**Options.** (a) Keep. (b) Let a boolean expression (variable, literal, function call of type `bool`) stand as a condition: one grammar alternative, the VM's `resolve_boolean_expression` already handles identifiers and literals. (c) Also make conditions expressions of type `bool` (`b := x < y`), which means merging `condition` into `expression` with proper precedences, a larger grammar change that also subsumes D2. (d) Print booleans: either `%d` prints 0/1 or a new `%b` prints `true`/`false`.

**Recommendation.** (b) now, (c) together with D2 since both rewrite the same precedence block, `%b` printing `true`/`false` for (d).

**Cost.** (b) and (d) small; (c) medium (grammar conflicts to resolve, `condition_resolution.c` and `expression_resolution.c` merge).

### D4 — Function without `return` on some path

**Today.** Nothing checks that every path of a function returns. At run time the caller reads the return cell, which is uninitialised: `x := f()` fails with an obscure `get_vm_cell_value Invalid type`, or the garbage value is used (`b12`, written as "must be a compile error").

**Options.** (a) Flow analysis at compile time: reject a function whose body can fall off the end (Java/C# rule). (b) A run-time error at the call site when the callee returned no value: "function 'f' returned no value". (c) Both: (b) immediately, (a) later.

**Recommendation.** (c). (b) is a five-line check in `resolve_expression` for `A_FUNC_PROC_CALL_STATEMENT` and removes the undefined behaviour today; (a) is the right long-term answer and is worth doing when D7 (early `return`) is settled, since both touch the same flow analysis.

**Cost.** (b) small; (a) medium. `b12` currently expects a compile error and must be rewritten to `RUN: fail` if only (b) is done.

### D5 — `string`

**Today.** `string` is a keyword of the lexer and the grammar rule `type: STRING_TYPE` has no action (the `FIXME: HELP` in `grammar.y`), so `var s : string;` fails with "Undefined type" (`b23`). The wiki documents a `string` parameter and `input("%s", name)` that do not compile (`b24`, `b25`). String literals exist only as `print` arguments; the VM cell type `STRING` is a bare pointer into the lexeme table.

**Options.** (a) Implement strings as a base type: cells own a heap copy, assignment and argument passing copy, `input %s` reads a bounded buffer, `print %s` works, `==`/`!=` compare contents, cells are freed when a frame is destroyed; concatenation and indexing can come later. (b) Remove `string` from the lexer and the wiki, keep literals in `print` only, and state that Argonaut has no string type.

**Recommendation.** (b) unless you want to write programs that manipulate text. (a) is one to two days because ownership must be added to `vm_cell` (today cells never own memory, which is what makes `check-leaks` green) and every copy path (`copy_cells`, `update_vm_cell`, frame destruction) must handle it.

**Cost.** (b) half an hour; (a) one to two days.

### D6 — Escape sequences

**Today.** A character literal is exactly one character between quotes (`'\n'` is a lexical error, `b05`), a string literal cannot contain `"` (`l04`), and the escapes `\n \t \\ \"` are interpreted by `print` at run time, not by the lexer, while `%%` is not supported (prints `%%`, `l05`).

**Options.** (a) Keep. (b) C escapes (`\n \t \\ \' \" \0`) decoded by the lexer for both `char` and string literals, and `%%` in `print`/`input` formats. Decoding at lex time means the lexeme table stores the decoded text and `process_special_chars` in the VM disappears; the IR row escaping added this session (`\n` written as two characters) already copes with decoded newlines.

**Recommendation.** (b).

**Cost.** Small: two lexer rules with a decoding helper, `%%` in `format_specifiers.c` and `print.c`.

### D7 — `return;` in a procedure, forward declarations

**Today.** `return` requires an expression and must be inside a function, so a procedure cannot exit early (`l10`). A function or procedure must be declared before it is called and there is no prototype syntax, so two routines cannot call each other (`l11`).

**Options.** (a) Keep (early exit with `if`/`else` nesting, no mutual recursion). (b) Allow `return;` in procedures: one grammar alternative and, in the VM, an "exit the current routine" flag distinct from the return-value cell (today the VM stops executing a body when the return cell becomes initialised, which a procedure never does). (c) Add forward declarations (`func f(x : int) -> int;` then the body later) or resolve names in a second pass.

**Recommendation.** (b) yes; (c) only if you want mutual recursion, and then prototypes are the cheaper route given the single-pass compiler.

**Cost.** (b) small; (c) medium (the declaration tables assume a routine's region exists when its name is inserted).

### D8 — Chained accesses

**Today.** The grammar enumerates fixed access shapes: `t[i]`, `t[i].f`, `t[i].f.g`, `s.f`, `s.f.g`, `s.t[i]`. Anything else is a syntax error: `t[i].f[j]` (`l07`), `a[i]++` (`l08`). Arrays of arrays are declared but unusable: `g[i][j]` does not parse and `g[i, j]` is rejected by the type checker because the element type is `Row`, not `int` (`l12`). On the VM side, after this session `array_element_offset` and `struct_field_offset` compose, so the addressing is ready for arbitrary chains.

**Options.** (a) Keep. (b) A uniform postfix grammar, `lvalue = IDENTIFIER { "[" indices "]" | "." IDENTIFIER }`, used for reads, assignments and `++`/`--`, with one type-resolution walker replacing `array_resolution.c` and `structure_resolution.c`. Arrays of arrays then work with `g[i][j]` (each `[ ]` indexes one level) and the two-index form `g[i, j]` stays reserved for multi-dimensional arrays.

**Recommendation.** (b): it removes special cases rather than adding some, and the AST shape (a chain of access nodes) is what the VM already walks.

**Cost.** Medium (a day): grammar, type resolution, `format_*_access` for error messages, and the IR validator's shape rules for access nodes.

### D9 — Limits

**Today.** The tables are static arrays with explicit errors when full: 8,192 distinct lexemes, 16,384 declarations, 16,384 representation entries, 1,024 regions, and 3,000 call frames (`MAX_CALL_DEPTH` in `stack_management.h`). The call-depth limit exists because every Argonaut call recurses in the C interpreter (about 0.4 KB of C stack per level in release, 1.2 KB under sanitizers), so a runaway recursion must be stopped before the C stack overflows with a segfault. All previously crashing cases (`b32`–`b35`, `l09`) pass.

**Options.** (a) Keep. (b) Dynamic tables (no limits except memory) — the declaration table's "overflow zone" design, where the primary index is the lexeme index, makes this less trivial than it sounds. (c) Raise the depth limit to 10,000 (safe with the default 8 MB stack on Linux and macOS).

**Recommendation.** (a), possibly (c). Nothing realistic hits these limits.

**Cost.** (c) one line.

### D10 — Integer overflow at run time

**Today.** Integers are C `int` (32-bit). Out-of-range literals are rejected since this session, but `2147483647 + 1` at run time is undefined behaviour in C (UBSan reports it; in practice it wraps).

**Options.** (a) Document wrap-around modulo 2³² and compute in `unsigned` to make it defined. (b) A run-time error on overflow (`__builtin_add_overflow` and friends in `arithmetic.c`), consistent with division by zero being an error. (c) 64-bit integers (changes `vm_cell`, `%d` formatting and the IR).

**Recommendation.** (b).

**Cost.** Small; add a `b47_int_overflow_runtime.arn` test with `RUN: fail`.

### D11 — IR format

**Today.** A text file with four tables, no version marker. Since this session `argov` validates every index and the shape of every node, so a corrupted or foreign file is rejected instead of crashing (`tests/ir/*`). Nothing detects an IR produced by a different compiler version.

**Options.** (a) Keep. (b) Add a first line `ARGONAUT_IR 1` written by `argoc` and checked by `argov`, bumped when the tables or node types change. (c) A binary format (faster to load, smaller) — premature for now.

**Recommendation.** (b): ten lines, and it makes the "IR is internal" rule enforceable.

**Cost.** Small; add `tests/ir/wrong_version.ir`.

### D12 — Aggregates passed by value

**Today.** Structures and arrays are copied whole on every call and assignment (fixed this session: `b16`–`b20`, `s11`, `s20`). A 10x10 matrix argument is 100 cell copies; cells are 16 bytes, so this is cheap at the sizes Argonaut programs use.

**Options.** (a) Keep: simple, predictable, no aliasing. (b) Reference parameters (`ref`/`var` parameters, Pascal style) so a procedure can modify its argument — needs an address-passing mechanism in the VM and a new keyword.

**Recommendation.** (a) until a program needs (b); procedures that must modify a global aggregate can do so by name today.

**Cost.** (b) medium.

## 5. Intermediate representation (IR)

A text file made of four tables (`BEGIN_LEXEME_TABLE` … `END_REGION_TABLE`) written by `argoc -o` and read by `argov -a`. It is considered **internal**: no compatibility is guaranteed between versions until D11 is decided, and `argov` must reject any invalid file (see `tests/ir/`).
