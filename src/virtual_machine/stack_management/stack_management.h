#ifndef __STACK_MANAGEMENT_H__
#define __STACK_MANAGEMENT_H__

#include "../core/vm_cell.h"

void init_execution_stack();

void push_execution_stack(int type, void *value);

void update_execution_cell(int address, vm_cell to);

void allocate_execution_cells(int type, int size);

vm_cell get_execution_cell(int address);

void fprintf_vm_stack(FILE *out);

#endif 

