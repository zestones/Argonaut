#ifndef __ASSIGNMENT_VALIDATION_H__
#define __ASSIGNMENT_VALIDATION_H__

#include "../validation_utils.h"
#include "../../ast/ast.h"

/**
 * @brief Checks type compatibility during variable assignments.
 * 
 * This function ensures that the type of an expression being assigned to a variable
 * matches the declared type of the variable. If a type mismatch occurs, it logs an error
 * with detailed information about the location and the types involved.
 * 
 * @param index_lexeme_lexicographic The lexicographic index of the variable being assigned.
 * @param expression A pointer to the `Node` representing the expression being assigned.
 */
void check_variable_assignment(int index_lexeme_lexicographic, Node *expression);

/**
 * @brief Validates type compatibility for array assignments.
 * 
 * This function verifies that the type of an expression matches the type of an array 
 * element during assignments. If the types are incompatible, an error is logged with
 * a detailed message.
 * 
 * @param array A pointer to the `Node` representing the array being accessed.
 * @param expression A pointer to the `Node` representing the expression being assigned.
 */
void check_array_assignment(Node *array, Node *expression);

/**
 * @brief Ensures type compatibility for struct field assignments.
 * 
 * This function checks whether the type of an expression being assigned to a struct
 * field matches the declared type of that field. If there is a mismatch, an error is logged,
 * providing information about the types and the location of the assignment.
 * 
 * @param structure A pointer to the `Node` representing the struct field being accessed.
 * @param expression A pointer to the `Node` representing the expression being assigned.
 */
void check_struct_assignment(Node *structure, Node *expression);

#endif // __ASSIGNMENT_VALIDATION_H__