#ifndef __LOOP_H__
#define __LOOP_H__

#include "../../../data/region_table.h"

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

#endif // __LOOP_H__