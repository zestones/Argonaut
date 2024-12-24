#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <stdio.h>
#include <stdlib.h>

#include "../../../ast/ast.h"
#include "../../core/vm_cell.h"

vm_cell resolve_arithmetic_operation(int operation, vm_cell left, vm_cell right);

vm_cell resolve_expression(Node *expression);

vm_cell resolve_boolean_expression(Node *expression);

#endif // __EXPRESSION_H__