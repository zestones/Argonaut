#ifndef __ADDRESS_CALCULATION_H__
#define __ADDRESS_CALCULATION_H__

#include "../../../ast/ast.h"

int get_variable_address(int index_declaration);

int get_array_address(Node *start_array_access, int base_address);

int get_struct_field_address(Node *struct_field_access, int base_address, int index_declaration);

#endif // __ADDRESS_CALCULATION_H__