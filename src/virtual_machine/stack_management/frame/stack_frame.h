#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include "../../../utils/stack.h"
#include "../../core/vm_cell.h"

#define MAX_STACK_SIZE 100

typedef struct {
    Stack cells;

    int static_link;
    int dynamic_link;
    int region_index;

    vm_cell region_value;
} stack_frame;

stack_frame construct_stack_frame(int static_link, int dynamic_link, int region_index);

vm_cell get_cell_from_stack_frame(stack_frame frame, int address);

void update_cell_in_stack_frame(stack_frame *frame, int address, vm_cell cell);

void push_cell_to_stack_frame(stack_frame *frame, vm_cell cell);

void allocate_cells_to_stack_frame(stack_frame *frame, int type, int size);

void fprintf_stack_frame(FILE *out, stack_frame frame);

#endif // __STACK_FRAME_H__