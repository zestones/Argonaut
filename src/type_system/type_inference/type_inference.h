#ifndef __TYPE_SYSTEM_H__
#define __TYPE_SYSTEM_H__

#include "../../ast/ast.h"
#include "../../symbol_table/declaration/declaration_table.h" 


/**
 * @brief Resolves the type of an expression.
 * 
 * This function resolves the type of various types of expressions, including literals, operations, array accesses, 
 * and function/procedure calls. It ensures type compatibility for expressions involving operations.
 * 
 * @param expression A pointer to the node representing the expression to resolve.
 * @return The resolved type of the expression, or `NULL_VALUE` if an error occurs.
 * 
 * @note This function handles the resolution of expression types, including operator operands and function calls.
 * 
 * @warning Type errors or unresolved expressions may lead to semantic errors during type resolution.
 */
int resolve_expression_type(Node *expression);

/**
 * @brief Human-readable name of an expression type, for diagnostics.
 *
 * @param type A declaration index, STRING_LITERAL_TYPE or NULL_VALUE.
 * @return The type name ("int", "string", a user type...), or "UNKNOWN".
 */
const char *type_name(int type);

/**
 * @brief Resolves the type of a condition expression.
 * 
 * This function resolves the type of a condition expression used in control structures (e.g., if statements, loops).
 * It ensures that the condition is of a boolean type.
 * 
 * @param condition A pointer to the node representing the condition expression.
 * @return The resolved type of the condition, which should be a boolean, or `NULL_VALUE` if an error occurs.
 * 
 * @note This function checks that conditions are properly typed as booleans for use in conditional statements.
 */


/**
 * @brief Resolves the number of parameters for a function or procedure.
 * 
 * This function resolves the parameter count for a function or procedure call by retrieving the number of parameters
 * defined in the declaration of the function or procedure.
 * 
 * @param index_lexeme_lexicographic The lexicographic index of the function or procedure name.
 * @param nature The nature of the entity, either `TYPE_FUNC` or `TYPE_PROC`.
 * @return The number of parameters for the function or procedure, or `NULL_VALUE` if an error occurs.
 * 
 * @note The function handles both functions and procedures, depending on the provided nature.
 */
int resolve_func_proc_parameter_count(int index_lexeme_lexicographic, Nature nature);

/**
 * @brief Resolves the return type of a function or procedure call.
 * 
 * This function resolves the return type of a function or procedure call by examining the declaration associated
 * with the function or procedure.
 * 
 * @param function_call A pointer to the node representing the function or procedure call.
 * @return The resolved return type of the function or procedure, or `NULL_VALUE` if an error occurs.
 * 
 * @note The return type is fetched from the function/procedure declaration.
 */
int resolve_func_proc_return_type(Node *function_call);

/**
 * @brief Resolves the type of a specific parameter in a function or procedure.
 * 
 * This function retrieves the type of a specific parameter based on the function or procedure declaration. 
 * It calculates the correct index to the parameter's type using the provided parameter index and the nature 
 * of the function or procedure (distinguishing between functions and procedures).
 * 
 * @param num_param The index of the parameter whose type is to be resolved (0-based).
 * @param index_declaration The declaration index of the function or procedure.
 * @param nature The type of the entity being resolved (either `TYPE_FUNC` or `TYPE_PROC`).
 * 
 * @return The type of the specified parameter, or `NULL_VALUE` if the parameter type cannot be found.
 */
int resolve_func_proc_parameter_type_at(int num_param, int index_declaration, Nature nature);






/**
 * @brief Type of the value designated by an access chain (t[i], s.f, t[i].f[j], g[i][j]...).
 *
 * Reports the errors it finds (unknown variable, not an array, wrong number of indices,
 * non-integer index, unknown field, field of a non-struct).
 *
 * @param access The root node of the chain (A_ARRAY_ACCESS or A_STRUCT_FIELD_ACCESS).
 * @return The declaration index of the type, or NULL_VALUE after an error.
 */
int resolve_access_type(Node *access);

#endif // __TYPE_SYSTEM_H__