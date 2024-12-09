#ifndef __TYPE_SYSTEM_H__
#define __TYPE_SYSTEM_H__

#include "../ast/ast.h"

int resolve_function_parameter_count(int index_lexeme_lexicographic);

int resolve_function_call_type(Node *function_call);

int resolve_function_parameter_type_at(int num_param, int index_declaration);


int resolve_struct_field_access_type(Node *struct_field_access);

#endif // __TYPE_SYSTEM_H__