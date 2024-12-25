#ifndef __STACK_MANAGEMENT_H__
#define __STACK_MANAGEMENT_H__

#include "frame/stack_frame.h"
#include "../core/vm_cell.h"

void init_execution_stack();

stack_frame *current_frame();

stack_frame *peek_execution_stack_as_mutable();

stack_frame peek_execution_stack();

void push_frame_to_execution_stack(stack_frame frame);

void fprintf_vm_stack(FILE *out);

#endif 

