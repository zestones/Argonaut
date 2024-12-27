#ifndef __FORMAT_ARRAY_ACCESS_CODE_H__
#define __FORMAT_ARRAY_ACCESS_CODE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../ast/ast.h"
#include "../../symbol_table/declaration/declaration_table.h" 


/**
 * @brief Formats a struct access expression into a string representation.
 * 
 * This function formats an expression that accesses a struct's field or array element. 
 * The struct name is followed by the field or array access, each formatted recursively 
 * with the appropriate dot notation for struct field access and array index notation.
 * 
 * @param struct_access A pointer to the node representing the struct access expression.
 * @return A dynamically allocated string representing the formatted struct access expression.
 * 
 * @note The caller is responsible for freeing the returned string.
 * 
 * @warning The function assumes that the struct access is valid and that field and array 
 *          accesses are correctly formed. Memory allocation failures may result in undefined behavior.
 */
char *format_struct_access(Node *struct_access);

/**
 * @brief Formats an array access expression into a string representation.
 * 
 * This function formats an array access expression into a string. The formatted string 
 * includes the array name and the formatted array indices or field accesses. If the 
 * array has any field accesses, they are represented with a dot (`.`) before the field name. 
 * The string is dynamically allocated and must be freed by the caller.
 * 
 * @param array_access A pointer to the node representing the array access.
 * @return A dynamically allocated string representing the formatted array access.
 * 
 * @note The caller is responsible for freeing the returned string.
 * 
 * @warning If memory allocation fails during concatenation, the function may result in undefined behavior.
 */
char *format_array_access(Node *array_access);

/**
 * @brief Formats an expression into a string representation.
 * 
 * This function takes an expression node and recursively formats it based on its 
 * type into a string. The types include literals, identifiers, arithmetic operations, 
 * array accesses, field accesses, and function or procedure calls. Each expression type 
 * is formatted differently according to its structure.
 * 
 * @param expression A pointer to the node representing the expression to be formatted.
 * @return A dynamically allocated string representing the formatted expression.
 * 
 * @note The caller is responsible for freeing the returned string.
 * 
 * @warning If an unrecognized expression type is encountered, an error will be logged 
 *          and `NULL` will be returned. Memory allocation failures may result in undefined behavior.
 */
char *format_expression(Node *expression);

/**
 * @brief Formats a function or procedure call into a string representation.
 * 
 * This function formats a function or procedure call represented by a node, 
 * where the function or procedure name is followed by a list of formatted arguments.
 * The arguments are recursively formatted, and the result is enclosed in parentheses.
 * 
 * @param func_proc_call A pointer to the node representing the function or procedure call.
 * @return A dynamically allocated string representing the formatted function or procedure call.
 * 
 * @note The caller is responsible for freeing the returned string.
 * 
 * @warning The function assumes that the arguments are properly formatted. 
 *          Memory allocation failures during string concatenation may lead to undefined behavior.
 */
char *format_func_proc_call(Node *func_proc_call);

#endif // __FORMAT_ARRAY_ACCESS_CODE_H__