#ifndef __TYPE_SYSTEM_H__
#define __TYPE_SYSTEM_H__

#include "../ast/ast.h"

// EXPRESSION RESOLUTION
int resolve_expression_type(Node *expression);


// FUNCTION RESOLUTION
int resolve_func_proc_parameter_count(int index_lexeme_lexicographic, Nature nature);
int resolve_func_proc_return_type(Node *function_call);
int resolve_func_proc_parameter_type_at(int num_param, int index_declaration, Nature nature);


// ARRAY RESOLUTION
int resolve_array_access_type(Node *array_access);


// STRUCT RESOLUTION
int resolve_field_access(Node *current_node, int current_type_declaration);
int resolve_struct_field_access_type(Node *struct_field_access);

#endif // __TYPE_SYSTEM_H__