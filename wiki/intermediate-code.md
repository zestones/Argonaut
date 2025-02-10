# Intermediate Code

## AST Node Format in Intermediate Code

```text
Node(<type>, <lex-index>, <decl-index>) [Child: <child>] [Sibling: <sibling>]
```

This format ensures consistency when interpreting or debugging the AST representation.
