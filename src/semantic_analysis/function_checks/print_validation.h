#ifndef __PRINT_VALIDATION_H__
#define __PRINT_VALIDATION_H__

#include "../validation_utils.h"
#include "../../ast/ast.h"

void check_print_proc_argument_list(Node *format_string, Node *argument_list);

#endif // __PRINT_VALIDATION_H__