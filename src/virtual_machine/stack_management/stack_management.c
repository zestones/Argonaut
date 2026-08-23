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
    if (size(execution_stack) >= MAX_CALL_DEPTH) {
        set_error_type(&error, RUN_TIME_ERROR);
        set_error_message(&error,
                          "Stack overflow: the call depth exceeds %d frames.\n"
                          "  Check for unbounded recursion.",
                          MAX_CALL_DEPTH);
        yerror(error);
        exit(EXIT_FAILURE);
    }

    push(&execution_stack, &frame, sizeof(stack_frame));
}

void pop_frame_from_execution_stack(void) {
    stack_frame *frame = (stack_frame *) pop(&execution_stack);
    destroy_stack_frame(frame);
    free(frame);
}

stack_frame *get_stack_frame_by_id(int id) {
    return (stack_frame *) get_stack_value(execution_stack, id);
}

stack_frame *find_stack_frame_by_region_index(int region_index) {
    int frame_id = get_execution_stack_current_frame_id();
    while (frame_id >= 0) {
        stack_frame *current_frame = get_stack_frame_by_id(frame_id);
        if (current_frame->region_index == region_index) return current_frame;

        // The first frame is its own dynamic link: the walk must stop there instead of looping.
        if (current_frame->dynamic_link == frame_id) break;
        frame_id = current_frame->dynamic_link;
    }

    set_error_type(&error, RUN_TIME_ERROR);
    set_error_message(&error, "No stack frame found.\n"
                              "  If this message is seen during runtime, it indicates a critical issue.\n"
                              "  Please report this error with details about the context in which it occurred.");

    yerror(error);
    exit(EXIT_FAILURE);
}

int get_execution_stack_current_frame_id() {
    return size(execution_stack) - 1;
}

void fprintf_vm_stack(FILE *out) {
    char title[32];

    for (int i = 0; i < size(execution_stack); i++) {
        stack_frame frame = *(stack_frame *) get_stack_value(execution_stack, i);

        snprintf(title, sizeof(title), "Stack Frame: %d", i);
        print_table_title(out, title);
        fprintf_stack_frame(out, frame);
    }
}
