#include "../../../lib/table_printer.h"
#include "../../utils/stack.h"

#include "frame/stack_frame.h"
#include "stack_management.h"

static Stack execution_stack;

void init_execution_stack() {
    execution_stack = construct_stack();
}

stack_frame *peek_execution_stack_as_mutable() {
    return (stack_frame *) peek(execution_stack);
}

stack_frame peek_execution_stack() {
    return *peek_execution_stack_as_mutable();
}

void push_frame_to_execution_stack(stack_frame frame) {
    push(&execution_stack, &frame, sizeof(stack_frame));
}

stack_frame pop_frame_from_execution_stack() {
    return *(stack_frame *) pop(&execution_stack);
}

int get_execution_stack_current_frame_id() {
    return size(execution_stack) - 1;
}

stack_frame *get_stack_frame_by_id(int id) {
    return (stack_frame *) get_stack_value(execution_stack, id);
}

stack_frame *find_stack_frame_by_region_index(int region_index) {
    for (int i = 0; i < size(execution_stack); i++) {
        stack_frame *frame = (stack_frame *) get_stack_value(execution_stack, i);
        if (frame->region_index == region_index) return frame;
    }

    printf("Error no stack frame found ! (find_stack_frame_by_region_index)\n");
    exit(EXIT_FAILURE);
}

void fprintf_vm_stack(FILE *out) {
    char title[15];

    for (int i = 0; i < size(execution_stack); i++) {
        stack_frame frame = *(stack_frame *) get_stack_value(execution_stack, i);

        snprintf(title, sizeof(title), "Stack Frame: %d", i);
        print_table_title(out, title);
        fprintf_stack_frame(out, frame);
    }
}

