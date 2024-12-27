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
int resolve_condition_type(Node *condition);


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
 * @brief Resolves the type of an array access expression.
 * 
 * This function resolves the type of an array access expression, ensuring that field access within arrays 
 * is handled correctly and that the correct declaration is used for array elements.
 * 
 * @param array_access A pointer to the node representing the array access expression.
 * @return The resolved type of the array access expression, or `NULL_VALUE` if an error occurs.
 * 
 * @note This function handles array access types and field access within arrays.
 */
int resolve_array_access_type(Node *array_access);


/**
 * @brief Resolves the access to a field within a struct.
 * 
 * This function resolves the access to a specific field within a struct. It checks if the field exists within the
 * struct, verifies the field's type, and handles any array accesses or nested struct accesses.
 * 
 * @param current_node A pointer to the node representing the field access.
 * @param current_type_declaration The declaration index of the struct being accessed.
 * @return The resolved type of the field, or `NULL_VALUE` if an error occurs.
 * 
 * @note This function handles both direct field access and nested struct/array field accesses.
 */
int resolve_field_access(Node *current_node, int current_type_declaration);

/**
 * @brief Resolves the type of a struct field access expression.
 * 
 * This function resolves the type of a struct field access expression, ensuring that the field is valid and checking
 * for array access or nested field accesses.
 * 
 * @param struct_field_access A pointer to the node representing the struct field access expression.
 * @return The resolved type of the struct field access, or `NULL_VALUE` if an error occurs.
 * 
 * @note This function handles multiple levels of field accesses and array access within structs.
 */
int resolve_struct_field_access_type(Node *struct_field_access);

#endif // __TYPE_SYSTEM_H__