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
 * @brief Checks an assignment to an array element or a structure field (any access chain).
 *
 * @param access The root node of the target chain.
 * @param expression The assigned expression.
 */
void check_access_assignment(Node *access, Node *expression);


#endif // __ASSIGNMENT_VALIDATION_H__