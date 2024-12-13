#ifndef __ASSIGNMENT_VALIDATION_H__
#define __ASSIGNMENT_VALIDATION_H__

#include "../validation_utils.h"
#include "../../ast/ast.h"

void check_variable_assignment(int index_lexeme_lexicographic, Node *expression);

void check_array_assignment(Node *array, Node *expression);

void check_struct_assignment(Node *structure, Node *expression);

#endif // __ASSIGNMENT_VALIDATION_H__