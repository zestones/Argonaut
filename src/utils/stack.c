#include "../../lib/table_printer.h"    
#include "../../lib/colors.h"
#include "stack.h"

Stack construct_stack() {
    Stack new_stack;
    memset(new_stack.data, NULL_VALUE, sizeof(new_stack.data)); 
    new_stack.top = 0;
    return new_stack;
}

int is_empty(const Stack stack) {
    return stack.top == 0;
}

int is_full(const Stack stack) {
    return stack.top == MAX_SIZE;
}

int size(const Stack stack) {
    return stack.top;
}

void push(Stack *stack, void *value, size_t value_size) {
    if (is_full(*stack)) {
        fprintf(stderr, "Error: Stack overflow\n");
        exit(EXIT_FAILURE);
    }


    stack->data[stack->top] = malloc(value_size);
    if (!stack->data[stack->top]) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    memcpy(stack->data[stack->top], value, value_size);
    stack->top++;
}

void *pop(Stack *stack) {
    if (is_empty(*stack)) {
        fprintf(stderr, "Error: Stack underflow\n");
        exit(EXIT_FAILURE);
    }

    stack->top--;
    void *value = stack->data[stack->top];
    return value;
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
    stack->data[index] = malloc(value_size);
    if (!stack->data[index]) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    memcpy(stack->data[index], value, value_size);
}

void *get_stack_value(const Stack stack, int index) {
    if (index < 0 || index >= stack.top) {
        fprintf(stderr, "Error: Get Stack Value - Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    void *value = stack.data[index];
    return value;
}

void stack_cpy(Stack *dest, const Stack src, size_t value_size) {
    dest->top = src.top;
    for (int i = 0; i < src.top; i++) {
        dest->data[i] = malloc(value_size);
        if (dest->data[i] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed during stack copy\n");
            exit(EXIT_FAILURE);
        }

        memcpy(dest->data[i], src.data[i], value_size);
    }
}

void print_stack(const Stack stack, const char* (*format_func)(void *)) {
    const int col_width_index = 10;
    const int col_width_value = 15;

    print_table_title(stdout, "Stack");
    print_table_separator(stdout, 2, col_width_index, col_width_value);
    print_table_header(stdout, 2, col_width_index, "Index", col_width_value, "Value");
    print_table_separator(stdout, 2, col_width_index, col_width_value);

    for (int i = 0; i < stack.top; i++) {
        char index_str[20];
        sprintf(index_str, "%d", i);
        print_table_row(stdout, 2, col_width_index, index_str, col_width_value, format_func(stack.data[i]));
    }

    print_table_separator(stdout, 2, col_width_index, col_width_value);
}