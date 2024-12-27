#ifndef __ADDRESS_CALCULATION_H__
#define __ADDRESS_CALCULATION_H__

#include "../../../ast/ast.h"

/**
 * @brief Computes the address of a variable in the stack frame based on its declaration index.
 *
 * This function calculates the address of a variable within the execution stack. The address
 * is determined by the variable's execution offset and its nesting level (NIS) within its
 * declaration region.
 *
 * @param index_declaration The index of the variable's declaration.
 * @return The computed address of the variable in the stack frame.
 */
int get_variable_address(int index_declaration);

/**
 * @brief Computes the memory address of an array element in the stack frame.
 *
 * This function determines the address of a specific array element in the stack frame,
 * based on the array's declaration, base address, and indices provided in the access node.
 * It handles arrays of any number of dimensions and checks index bounds for each dimension.
 *
 * @param start_array_access A pointer to the AST node representing the starting point 
 *        of the array access, containing the indices and other relevant information.
 * @param base_address The base address of the array in the stack. If set to `NULL_VALUE`,
 *        the function computes the base address using the array's declaration index.
 * @return The computed memory address of the array element.
 *
 * @details
 * - The function resolves indices for each dimension of the array, validates their bounds,
 *   and computes the corresponding memory offset.
 * - If the array contains structured elements (e.g., structs), it computes the field offset
 *   within the structure when necessary.
 * - The address computation accounts for the size of each array element and adjusts for 
 *   multidimensional arrays.
 * - The function ensures type safety and exits with an error if the indices are not integers
 *   or are out of bounds.
 *
 * @note The caller must ensure that the provided `start_array_access` node is valid and 
 *       represents a well-formed array access expression.
 *
 * @warning Improper use of this function, such as providing invalid indices, may result 
 *          in program termination due to bounds checking.
 */
int get_array_address(Node *start_array_access, int base_address);

/**
 * @brief Computes the memory address of a specific field within a struct or nested struct.
 *
 * This function calculates the memory address of a struct field, handling nested struct 
 * and array fields. It starts from a base address and traverses the access hierarchy 
 * to determine the final offset.
 *
 * @param struct_field_access A pointer to the AST node representing the field access. 
 *        This node contains information about the field hierarchy and indices for arrays 
 *        if applicable.
 * @param base_address The base address of the struct in memory. If set to `NULL_VALUE`,
 *        the base address is computed using the struct's declaration index.
 * @param index_declaration The index of the struct's declaration. This is used to resolve 
 *        the struct's layout and field details. If `base_address` is `NULL_VALUE`, the 
 *        declaration index is resolved from the `struct_field_access` node.
 * @return The computed memory address of the accessed field.
 *
 * @details
 * - The function traverses the hierarchy of field accesses, accumulating offsets for 
 *   each field. For nested structs, it recursively calculates the field address.
 * - If a field is an array, it computes the address of the array element based on the 
 *   indices provided in the access node.
 * - Offset calculations are based on the execution offsets and type sizes defined in 
 *   the struct's declaration.
 * - The function ensures type safety and handles various field types (e.g., basic types, 
 *   arrays, structs).
 *
 * @note
 * - The caller must ensure the `struct_field_access` node is valid and correctly represents 
 *   the field access.
 * - Proper struct declarations and field layouts must be available for accurate address computation.
 *
 * @warning Improper use of this function, such as providing invalid indices or struct 
 *          access hierarchies, may result in incorrect memory address calculations.
 */
int get_struct_field_address(Node *struct_field_access, int base_address, int index_declaration);

#endif // __ADDRESS_CALCULATION_H__