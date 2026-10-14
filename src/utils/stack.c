#include <string.h>

#include "../../lib/table_printer.h"
#include "../../lib/colors.h"
#include "memory.h"
#include "stack.h"

#define INITIAL_CAPACITY 16

static void *allocate_copy(const void *value, size_t value_size) {
    void *copy = xmalloc(value_size);
    memcpy(copy, value, value_size);
    return copy;
}

Stack construct_stack(void) {
    Stack new_stack;
    new_stack.data = NULL;
    new_stack.capacity = 0;
    new_stack.top = 0;
    return new_stack;
}

int is_empty(const Stack stack) {
    return stack.top == 0;
}

int is_full(const Stack stack) {
    (void) stack;
    return 0;
}

int size(const Stack stack) {
    return stack.top;
}

void push(Stack *stack, void *value, size_t value_size) {
    if (stack->top == stack->capacity) {
        int capacity = stack->capacity ? stack->capacity * 2 : INITIAL_CAPACITY;
        stack->data = xrealloc(stack->data, (size_t) capacity * sizeof(*stack->data));
        stack->capacity = capacity;
    }

    stack->data[stack->top] = allocate_copy(value, value_size);
    stack->top++;
}

void *pop(Stack *stack) {
    if (is_empty(*stack)) {
        fprintf(stderr, "Error: Stack underflow\n");
        exit(EXIT_FAILURE);
    }

    stack->top--;
    return stack->data[stack->top];
}

void *peek(const Stack stack) {
    if (is_empty(stack)) {
        fprintf(stderr, "Error: Stack is empty\n");
        exit(EXIT_FAILURE);
    }

    return stack.data[stack.top - 1];
}

void update_stack(Stack *stack, int index, void *value, size_t value_size) {
    if (index < 0 || index >= stack->top) {
        fprintf(stderr, "Error: Stack Index out of bounds\n");
        exit(EXIT_FAILURE);
    }

    free(stack->data[index]);
    stack->data[index] = allocate_copy(value, value_size);
}

void *get_stack_value(const Stack stack, int index) {
    if (index < 0 || index >= stack.top) {
        fprintf(stderr, "Error: Get Stack Value - Index out of bounds\n");
        exit(EXIT_FAILURE);
    }

    return stack.data[index];
}

void stack_cpy(Stack *dest, const Stack src, size_t value_size) {
    destroy_stack(dest);
    for (int i = 0; i < src.top; i++) {
        push(dest, src.data[i], value_size);
    }
}

void destroy_stack(Stack *stack) {
    for (int i = 0; i < stack->top; i++) {
        free(stack->data[i]);
    }
    free(stack->data);

    *stack = construct_stack();
}

void print_stack(const Stack stack, const char *(*format_func)(void *) ) {
    const int col_width_index = 10;
    const int col_width_value = 15;

    print_table_title(stdout, "Stack");
    print_table_separator(stdout, 2, col_width_index, col_width_value);
    print_table_header(stdout, 2, col_width_index, "Index", col_width_value, "Value");
    print_table_separator(stdout, 2, col_width_index, col_width_value);

    for (int i = 0; i < stack.top; i++) {
        char index_str[20];
        snprintf(index_str, sizeof(index_str), "%d", i);
        print_table_row(stdout, 2, col_width_index, index_str, col_width_value, format_func(stack.data[i]));
    }

    print_table_separator(stdout, 2, col_width_index, col_width_value);
}
