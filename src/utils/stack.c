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
    return stack.top == MAX_SIZE - 1;
}

int size(const Stack stack) { 
    return stack.top; 
}

void push(Stack *stack, int value) {
    if (is_full(*stack)) {
        fprintf(stderr, COLOR_RED "<Error> Stack overflow\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    stack->data[stack->top++] = value;
}

int pop(Stack *stack) {
    if (is_empty(*stack)) {
        fprintf(stderr, COLOR_RED "<Error> Stack underflow\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }
    
    return stack->data[--stack->top];
}

int peek(const Stack stack) {
    if (is_empty(stack)) {
        fprintf(stderr, COLOR_RED "<Error> Stack is empty\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    return stack.data[stack.top - 1];
}

void stack_cpy(Stack *dest, const Stack src) {
    dest->top = src.top;
    for (int i = 0; i < src.top; i++) {
        dest->data[i] = src.data[i];
    }
}

void print_stack(const Stack stack) {
    const int col_width_index = 10;
    const int col_width_value = 15;

    print_table_title(stdout, "Stack");
    print_table_separator(stdout, 2, col_width_index, col_width_value);
    print_table_header(stdout, 2, col_width_index, "Index", col_width_value, "Value");
    print_table_separator(stdout, 2, col_width_index, col_width_value);

    for (int i = 0; i < stack.top; i++) {
        char index_str[20], value_str[20];
        sprintf(index_str, "%d", i);
        sprintf(value_str, "%d", stack.data[i]);

        print_table_row(stdout, 2, col_width_index, index_str, col_width_value, value_str);
    }

    print_table_separator(stdout, 2, col_width_index, col_width_value);
}