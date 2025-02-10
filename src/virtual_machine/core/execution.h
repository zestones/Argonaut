#ifndef __VIRTUAL_MACHINE_H__
#define __VIRTUAL_MACHINE_H__

#include <stdio.h>
#include <stdlib.h>

#include "../stack_management/frame/stack_frame.h"

#include "../../ast/ast.h"
#include "vm_cell.h"


/**
 * @brief Allocates memory in the stack frame for a variable based on its type declaration.
 *
 * This function recursively allocates the required memory for variables, including 
 * primitive types, arrays, and structs. It handles complex types by traversing their 
 * structure and allocating memory for each component.
 *
 * @param index_type_declaration The index of the type declaration for the variable. 
 *        This is used to determine the nature and structure of the type.
 * @param frame The stack frame to declare variable in.
 *
 * @details
 * - For base types (e.g., integers, floats), the function allocates the necessary 
 *   stack space directly.
 * - For arrays, the function recursively allocates memory for each element based 
 *   on the array's size and element type.
 * - For structs, the function iterates over each field in the struct and allocates 
 *   memory according to the field's type declaration.
 */
void declare_variable(stack_frame *frame, int index_type_declaration);

/**
 * @brief Handles the declaration of a variable by allocating memory in the stack frame.
 *
 * This function resolves the type of the variable from its declaration and delegates 
 * the memory allocation to `declare_variable`.
 *
 * @param index_declaration The index of the variable's declaration. This is used to 
 *        retrieve the type information for the variable.
 *
 * @details
 * - The function retrieves the type declaration index associated with the variable 
 *   and calls `declare_variable` to allocate memory based on the type.
 * - It ensures that all necessary memory for the variable, including nested structures 
 *   or arrays, is allocated.
 */
void handle_variable_declaration(int index_declaration);

/**
 * @brief Retrieves the cell associated with a variable from the stack frame.
 *
 * This function locates the memory cell corresponding to a variable's declaration
 * in the stack frame based on its address and region.
 *
 * @param index_declaration The index of the variable's declaration.
 * @return The `vm_cell` representing the memory location of the variable.
 */
vm_cell get_variable_cell(int index_declaration);

/**
 * @brief Retrieves the cell associated with a structure field from the stack frame.
 *
 * This function resolves the memory address of a structure field and retrieves the
 * corresponding cell from the stack frame.
 *
 * @param struct_access A node representing the structure field access.
 * @return The `vm_cell` representing the memory location of the structure field.
 */
vm_cell get_struct_cell(Node *struct_access);

/**
 * @brief Retrieves the cell associated with an array element from the stack frame.
 *
 * This function calculates the address of an array element and retrieves the
 * corresponding cell from the stack frame.
 *
 * @param arra_access A node representing the array element access.
 * @return The `vm_cell` representing the memory location of the array element.
 */
vm_cell get_array_cell(Node *arra_access);

/**
 * @brief Retrieves the return value cell from the current function's stack frame.
 *
 * This function accesses the stack frame of the currently executing function 
 * by using the dynamic link to locate its stack frame. It then retrieves the 
 * `region_value` field, which represents the memory location for storing 
 * the return value of the function.
 *
 * @return The `vm_cell` containing the return value location for the current function.
 */
vm_cell get_return_cell();

/**
 * @brief Updates the memory cell of a variable with a new value.
 *
 * This function handles the affectation (assignment) of a value to a variable
 * by updating the corresponding memory cell in the stack frame.
 *
 * @param index_declaration The index of the variable's declaration.
 * @param cell The `vm_cell` containing the new value to assign.
 */
void handle_variable_affectation(int index_declaration, vm_cell cell);

/**
 * @brief Updates the memory cell of an array element with a new value.
 *
 * This function calculates the address of an array element and updates the
 * corresponding memory cell in the stack frame with the provided value.
 *
 * @param array_index_list A node representing the indices used to access the array element.
 * @param cell The `vm_cell` containing the new value to assign.
 */
void handle_array_affectation(Node *array_index_list, vm_cell cell);

/**
 * @brief Updates the memory cell of a structure field with a new value.
 *
 * This function resolves the memory address of a structure field and updates
 * the corresponding cell in the stack frame with the provided value.
 *
 * @param struct_field_access A node representing the structure field access.
 * @param cell The `vm_cell` containing the new value to assign.
 */
void handle_structure_affectation(Node* struct_field_access, vm_cell cell);

/**
 * @brief Stores the return value of a function in the caller's stack frame.
 *
 * This function places the return value of a function in the `region_value`
 * of the dynamic link stack frame associated with the caller.
 *
 * @param cell The `vm_cell` containing the value to return.
 */
void handle_function_return_value(vm_cell cell);

#endif // __VIRTUAL_MACHINE_H__