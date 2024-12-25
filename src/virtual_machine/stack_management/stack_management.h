#ifndef __STACK_MANAGEMENT_H__
#define __STACK_MANAGEMENT_H__

#include "frame/stack_frame.h"
#include "../core/vm_cell.h"

void init_execution_stack();

stack_frame *current_frame();

stack_frame *peek_execution_stack_as_mutable();

stack_frame peek_execution_stack();

void push_frame_to_execution_stack(stack_frame frame);

stack_frame pop_frame_from_execution_stack();

stack_frame *find_stack_frame_by_region_index(int region_index);

int get_execution_stack_current_frame_id();

stack_frame *get_stack_frame_by_id(int id);

void fprintf_vm_stack(FILE *out);

#endif 

