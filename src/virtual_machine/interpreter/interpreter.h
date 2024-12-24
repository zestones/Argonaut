#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

void interpret();

void interpret_ast(AST ast);

void resolve_statement_list(AST statement_list);

#endif 
