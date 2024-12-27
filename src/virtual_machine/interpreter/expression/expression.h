#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <stdio.h>
#include <stdlib.h>

#include "../../../ast/ast.h"
#include "../../core/vm_cell.h"

/**
 * @brief Resolves an expression from the abstract syntax tree (AST) and returns the corresponding VM cell.
 *
 * This function evaluates different types of expressions in the AST and returns a `vm_cell` representing the value of 
 * the expression. It handles variables, literals, arithmetic operations, array access, structure field access, 
 * and function/procedure calls.
 *
 * @details
 * The function supports the following expression types:
 * - `A_IDENTIFIER`: Resolves a variable and checks if it has been initialized.
 * - `A_INTEGER_LITERAL`: Resolves an integer literal.
 * - `A_FLOAT_LITERAL`: Resolves a floating-point literal.
 * - `A_BOOLEAN_LITERAL`: Resolves a boolean literal (`true` or `false`).
 * - `A_CHARACTER_LITERAL`: Resolves a character literal.
 * - `A_STRING_LITERAL`: Resolves a string literal.
 * - `A_ADD_OP`, `A_SUB_OP`, `A_MUL_OP`, `A_DIV_OP`: Resolves arithmetic operations between two operands.
 * - `A_ARRAY_ACCESS`: Resolves an array access and returns the corresponding cell.
 * - `A_STRUCT_FIELD_ACCESS`: Resolves a structure field access and returns the corresponding cell.
 * - `A_FUNC_PROC_CALL_STATEMENT`: Resolves a function or procedure call and returns the return value.
 *
 * @param expression The AST node representing the expression to resolve.
 * 
 * @return A `vm_cell` containing the resolved value of the expression. If the expression type is not implemented,
 *         it returns a `NULL_VALUE` cell.
 */
vm_cell resolve_expression(Node *expression);

/**
 * @brief Resolves an arithmetic operation between two operands and returns the resulting value.
 *
 * This function evaluates the specified arithmetic operation between two operands (left and right). The operands 
 * can either be integers or real (floating-point) numbers. It performs addition, subtraction, multiplication, 
 * or division based on the specified operation.
 *
 * @details
 * The function performs the following checks:
 * - Division by zero is detected, and an error is reported.
 * - Both operands must be initialized before performing the operation. If any operand is uninitialized, 
 *   an error is reported.
 *
 * @param operation The type of arithmetic operation to perform (e.g., addition, subtraction, etc.).
 * @param left The left operand of the arithmetic operation.
 * @param right The right operand of the arithmetic operation.
 * 
 * @return A `vm_cell` containing the result of the arithmetic operation.
 *         If the operation type or operand types are invalid, no result is returned, but the program exits.
 */
vm_cell resolve_arithmetic_operation(int operation, vm_cell left, vm_cell right);

/**
 * @brief Resolves a boolean expression and returns the resulting boolean value.
 *
 * This function resolves a boolean expression, which can involve logical operations (AND, OR, NOT), 
 * comparison operations (==, !=, <, >, <=, >=), and literals. It returns a `vm_cell` containing the 
 * resulting boolean value (`1` for true, `0` for false).
 *
 * The function recursively resolves the operands of binary operations (left and right expressions) 
 * and applies the appropriate operation. For unary operations, the function resolves the single operand.
 *
 * @param expression A pointer to the AST node representing the boolean expression to resolve.
 * 
 * @return A `vm_cell` containing the resulting boolean value of the expression. If the expression 
 *         type is invalid, the function terminates the program with an error message.
 */
vm_cell resolve_boolean_expression(Node *expression);

#endif // __EXPRESSION_H__