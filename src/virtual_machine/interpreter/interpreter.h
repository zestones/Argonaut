#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

/**
 * @brief Resolves and executes a list of statements.
 * 
 * This function processes a list of statements in the program, iterating through each 
 * statement in the list. It handles different types of statements such as assignment, 
 * conditionals (`if`, `else`), input/output statements, loops, and function/procedure calls. 
 * Depending on the type of statement, the function delegates execution to the appropriate handler.
 * 
 * @param statement_list A pointer to the AST node representing the list of statements to resolve.
 */
void resolve_statement_list(AST statement_list);

/**
 * @brief Executes the code for a specific region in the program.
 * 
 * This function processes a specific region of the program, identified by its region 
 * index, by pushing the region to the execution stack and interpreting the corresponding 
 * AST. Once the region is processed, it pops the region and the execution frame from 
 * the stack.
 * 
 * @param region_index The index of the region to execute in the program.
 */
void execute(int region_index);

/**
 * @brief Interprets and executes the entire program.
 * 
 * This function initializes the execution stack and regions, starts by executing the 
 * global program, and continues processing through the program's AST. It serves as 
 * the main entry point for interpreting and executing the program.
 */
void interpret();

#endif 
