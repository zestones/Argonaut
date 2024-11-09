#ifndef __FUNC_PROC_MANAGER_H__
#define __FUNC_PROC_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct func_proc_manager_context
{
    int index_return_type_representation;
    int index_number_of_parameters_representation;

    int number_of_parameters;

    int index_func_proc_name_lexicographic;
    int index_return_type_lexicographic;
} func_proc_manager_context;


void construct_func_proc_manager_context(int index_func_proc_name_lexicographic);

void func_proc_add_parameter(int index_lexeme_lexicographic, int index_type_lexicographic);


// ------ FUNC ------ //

void declaration_func_start();

void declaration_func_end(int index_return_type_lexicographic);

// ------ PROC ------ //

void declaration_proc_start();

void declaration_proc_end();

#endif // __FUNC_PROC_MANAGER_H__