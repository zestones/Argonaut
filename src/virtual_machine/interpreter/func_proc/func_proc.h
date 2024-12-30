#ifndef __PROCEDURE_H__
#define __PROCEDURE_H__

/**
 * @brief Executes the print function for displaying formatted output.
 *
 * This function processes a `print` statement, which includes a format string and a list of arguments. 
 * It resolves the format string and corresponding arguments, then prints the formatted output.
 * If the format string is missing or invalid, it falls back to printing the raw argument without formatting.
 *
 * @param ast A pointer to the AST node representing the `print` statement.
 */
void execute_print(AST ast);

/**
 * @brief Executes the input function for reading values from the user.
 *
 * This function processes an `input` statement that includes a format string and a list of arguments. 
 * The format string specifies the types of the values to be read, and the arguments provide the 
 * locations where the input values will be stored.
 * 
 * @param ast A pointer to the AST node representing the `input` statement.
 */
void execute_input(AST ast);

/**
 * @brief Executes a function or procedure call.
 * 
 * This function handles the execution of a function or procedure call. It creates a new stack frame 
 * for the function or procedure, resolving parameters and arguments, then pushing the frame to the 
 * execution stack. After setting up the stack frame, it invokes the function or procedure using the 
 * `execute` function.
 * 
 * @param ast A pointer to the AST node representing the function or procedure call.
 */
void execute_func_proc_call(AST ast);

#endif // __PROCEDURE_H__