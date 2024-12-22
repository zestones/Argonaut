#ifndef __ADDRESS_CALCULATION_H__
#define __ADDRESS_CALCULATION_H__

#include "../../../ast/ast.h"

int get_variable_address(int index_declaration);

int get_array_address(Node *array_index_list);

int get_struct_field_address(Node *struct_field_access);

#endif // __ADDRESS_CALCULATION_H__