/**
 * @file func_proc_manager.h
 * @brief Header file for managing function and procedure declarations.
 *
 * This file contains the definitions and function declarations for managing
 * the context of function and procedure declarations, including their parameters
 * and return types.
 */

#ifndef __FUNC_PROC_MANAGER_H__
#define __FUNC_PROC_MANAGER_H__


#include <stdio.h>
#include <stdlib.h>


/**
 * @struct func_proc_manager_context
 * @brief Structure to hold context information for function and procedure declarations.
 * 
 * This structure is used to hold information retrieved from our grammar rules, such as
 * the number of parameters, the return type, and the lexicographic order of the function
 * or procedure name. 
 * 
 * These information is used to fill the tables with the correct information.
 */
typedef struct func_proc_manager_context
{
    int index_return_type_representation;           // Index representing the return type.
    int index_number_of_parameters_representation;  // Index representing the number of parameters.

    int number_of_parameters;                       // Number of parameters.   

    int index_func_proc_name_lexicographic;         // Index representing the lexicographic order of the function or procedure name.
    int index_return_type_lexicographic;            // Index representing the lexicographic order of the return type.
} func_proc_manager_context;

/**
 * @brief Constructs the context for a function or procedure declaration.
 *
 * @param index_func_proc_name_lexicographic Index representing the lexicographic order of the function or procedure name.
 */
void construct_func_proc_manager_context(int index_func_proc_name_lexicographic);

/**
 * @brief Adds a parameter to the current function or procedure context.
 *
 * @param index_lexeme_lexicographic Index representing the lexicographic order of the parameter lexeme.
 * @param index_type_lexicographic Index representing the lexicographic order of the parameter type.
 */
void func_proc_add_parameter(int index_lexeme_lexicographic, int index_type_lexicographic);

/**
 * @brief Starts the declaration of a function.
 */
void declaration_func_start();

/**
 * @brief Ends the declaration of a function.
 *
 * @param index_return_type_lexicographic Index representing the lexicographic order of the return type.
 */
void declaration_func_end(int index_return_type_lexicographic);

/**
 * @brief Starts the declaration of a procedure.
 */
void declaration_proc_start();

/**
 * @brief Ends the declaration of a procedure.
 */
void declaration_proc_end();

#endif // __FUNC_PROC_MANAGER_H__