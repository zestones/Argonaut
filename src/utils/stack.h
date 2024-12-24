/**
 * @file stack.h
 * @brief A simple stack implementation in C.
 *
 * This header file contains the definition and function declarations for a stack data structure.
 */

#ifndef __STACK_H__
#define __STACK_H__


#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


#define MAX_SIZE 5000

/**
 * @struct Stack
 * @brief A structure to represent a stack.
 * 
 * This structure defines a stack with a fixed maximum size. It contains an array to hold the stack elements
 * and an integer to keep track of the index of the top element.
 * The stack can store elements of any data type by using a void pointer. The data should be cast to the appropriate
 * type when popped from the stack.
 */
typedef struct {
    void *data[MAX_SIZE];   // Array to store the elements of the stack.
    int top;                // Index of the top element in the stack.
} Stack;

/**
 * @brief Constructs a new stack.
 *
 * @return A new stack with initialized values.
 */
Stack construct_stack();

/**
 * @brief Checks if the stack is empty.
 *
 * @param stack The stack to check.
 * @return 1 if the stack is empty, 0 otherwise.
 */
int is_empty(const Stack stack);

/**
 * @brief Checks if the stack is full.
 *
 * @param stack The stack to check.
 * @return 1 if the stack is full, 0 otherwise.
 */
int is_full(const Stack stack);

/**
 * @brief Returns the size of the stack.
 *
 * @param stack The stack to check.
 * @return The number of elements in the stack.
 */
int size(const Stack stack);

/**
 * @brief Pushes a value onto the stack.
 *
 * @param stack The stack to push the value onto.
 * @param value The value to push onto the stack.
 * @param value_size The size of the value to push (needed for allocating memory).
 */
void push(Stack *stack, void *value, size_t value_size);

/**
 * @brief Pops a value from the stack.
 *
 * @param stack The stack to pop the value from.
 * @return The value popped from the stack.
 */
void *pop(Stack *stack);

/**
 * @brief Peeks at the top value of the stack without popping it.
 *
 * @param stack The stack to peek at.
 * @return The value at the top of the stack.
 */
void *peek(const Stack stack);

/**
 * @brief Gets the value at a specific index in the stack.
 *
 * @param stack The stack to get the value from.
 * @param index The index of the value to get.
 * @return The value at the specified index.
 */
void *get_stack_value(const Stack stack, int index);

/**
 * @brief Updates the value at a specific index in the stack.
 *
 * @param stack The stack to update.
 * @param index The index of the value to update.
 * @param value The new value to store in the stack.
 * @param value_size The size of the value to store (needed for allocating memory).
 */
void update_stack(Stack *stack, int index, void *value, size_t value_size);

/**
 * @brief Copies the contents of one stack to another.
 *
 * @param dest The destination stack.
 * @param src The source stack.
 * @param value_size The size of the values in the stack (needed for allocating memory).
 */
void stack_cpy(Stack *dest, const Stack src, size_t value_size);

/**
 * @brief Prints the contents of the stack.
 *
 * @param stack The stack to print.
 * @param format_func A function to format the elements of the stack.
 */
void print_stack(const Stack stack, const char* (*format_func)(void *));


#endif // __STACK_H__
