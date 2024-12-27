#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "../../../data/region_table.h"

/**
 * @brief Executes a conditional statement (if-else).
 *
 * This function evaluates the boolean condition in the provided abstract syntax tree (AST) node. If the condition 
 * evaluates to true, it resolves and executes the statement list of the 'if' branch. If the condition is false, 
 * and if an 'else' branch is present, it will resolve and execute the 'else' statement list.
 *
 * @param ast The root node of the conditional statement in the AST. 
 *            It should represent either an 'if' statement with an optional 'else' branch.
 */
void execute_condition(AST ast);

/**
 * @brief Executes a loop statement (while loop).
 *
 * This function executes a while loop, continuously evaluating the condition provided in the AST node. 
 * If the condition evaluates to true, it resolves and executes the body of the loop. The loop continues to execute 
 * as long as the condition remains true.
 *
 * @param ast The root node of the loop statement in the AST.
 *            It should represent a 'while' loop with a condition and a statement list for the loop body.
 */
void execute_loop(AST ast);

#endif // __CONDITION_H__