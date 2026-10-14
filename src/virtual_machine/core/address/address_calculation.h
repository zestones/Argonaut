#ifndef __ADDRESS_CALCULATION_H__
#define __ADDRESS_CALCULATION_H__

#include "../../../ast/ast.h"

/**
 * @brief Address of a variable or parameter inside its stack frame.
 *
 * @param index_declaration The declaration index of the variable.
 * @return The address of its first cell in the frame of its region.
 */
int get_variable_address(int index_declaration);

/**
 * @brief Offset of the element designated by an array access, relative to the start of the array.
 *
 * Evaluates the index expressions, checks them against the bounds of every dimension (the
 * first index varies fastest) and follows a trailing field access when the elements are
 * structures (`t[i].x`).
 *
 * @param access The A_ARRAY_ACCESS node.
 * @param array_type The declaration index of the array type.
 * @param type_out If not NULL, receives the declaration index of the designated value's type.
 * @return The offset in cells.
 */
int array_element_offset(Node *access, int array_type, int *type_out);

/**
 * @brief Offset of a (possibly nested) field, relative to the start of a structure.
 *
 * Follows the chain of field nodes (`a.b.c`), including array fields (`a.t[i]`).
 *
 * @param field The first A_STRUCT_FIELD_ACCESS (or A_ARRAY_ACCESS) field node.
 * @param struct_type The declaration index of the structure type.
 * @param type_out If not NULL, receives the declaration index of the designated value's type.
 * @return The offset in cells.
 */
int struct_field_offset(Node *field, int struct_type, int *type_out);

/**
 * @brief Address of the element designated by an array access.
 *
 * @param start_array_access The A_ARRAY_ACCESS node whose declaration is the array variable.
 * @param base_address The address of the array, or NULL_VALUE to use the variable's address.
 * @return The address of the element.
 */
int get_array_address(Node *start_array_access, int base_address);

/**
 * @brief Address of the field designated by a structure access.
 *
 * @param struct_field_access The A_STRUCT_FIELD_ACCESS node whose declaration is the structure
 *        variable (base_address == NULL_VALUE), or the first field node when the base is given.
 * @param base_address The address of the structure, or NULL_VALUE to use the variable's address.
 * @param index_declaration The declaration index of the structure type (ignored when
 *        base_address is NULL_VALUE, the variable's type is used).
 * @return The address of the field.
 */
int get_struct_field_address(Node *struct_field_access, int base_address, int index_declaration);

/**
 * @brief Address of the value designated by an lvalue node (variable, array element or field).
 *
 * @param access An A_IDENTIFIER, A_ARRAY_ACCESS or A_STRUCT_FIELD_ACCESS node.
 * @return The address of its first cell in the frame of the root variable's region.
 */
int get_lvalue_address(Node *access);

/**
 * @brief Declaration index of the type of the value designated by an lvalue node.
 *
 * @param access An A_IDENTIFIER, A_ARRAY_ACCESS or A_STRUCT_FIELD_ACCESS node.
 * @return The declaration index of the type (a base type, a structure or an array).
 */
int get_lvalue_type(Node *access);

#endif // __ADDRESS_CALCULATION_H__
