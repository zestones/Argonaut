/**
 * @file stack.h
 * @brief A simple stack of malloc'ed copies, growing on demand.
 */

#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/**
 * @struct Stack
 * @brief A growable stack of values of any type.
 *
 * Every pushed value is copied into its own malloc'ed block (`value_size` bytes) and the stack
 * keeps the pointers in a dynamically allocated array that doubles when it is full. Elements are
 * cast back to their type when read. `pop()` hands the block to the caller, who frees it;
 * `destroy_stack()` frees everything that is still on the stack.
 */
typedef struct {
    void **data;  // malloc'ed copies of the pushed values
    int capacity; // number of slots allocated in `data`
    int top;      // number of elements (index of the next free slot)
} Stack;

/**
 * @brief Constructs an empty stack (no memory is allocated until the first push).
 *
 * @return A new, empty stack.
 */
Stack construct_stack(void);

/**
 * @brief Checks if the stack is empty.
 *
 * @param stack The stack to check.
 * @return 1 if the stack is empty, 0 otherwise.
 */
int is_empty(const Stack stack);

/**
 * @brief Checks if the stack is full. A stack grows on demand, so this is always 0.
 *
 * @param stack The stack to check.
 * @return 0.
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
 * @brief Pushes a copy of a value onto the stack.
 *
 * @param stack The stack to push the value onto.
 * @param value The value to copy onto the stack.
 * @param value_size The size of the value (number of bytes copied).
 */
void push(Stack *stack, void *value, size_t value_size);

/**
 * @brief Pops the top value from the stack.
 *
 * @param stack The stack to pop the value from.
 * @return The malloc'ed copy of the value; the caller owns it and must free it.
 */
void *pop(Stack *stack);

/**
 * @brief Peeks at the top value of the stack without popping it.
 *
 * @param stack The stack to peek at.
 * @return A pointer to the value at the top of the stack (owned by the stack).
 */
void *peek(const Stack stack);

/**
 * @brief Gets the value at a specific index in the stack (0 is the bottom).
 *
 * @param stack The stack to get the value from.
 * @param index The index of the value to get.
 * @return A pointer to the value at the specified index (owned by the stack).
 */
void *get_stack_value(const Stack stack, int index);

/**
 * @brief Replaces the value at a specific index in the stack.
 *
 * @param stack The stack to update.
 * @param index The index of the value to update.
 * @param value The new value to copy into the stack.
 * @param value_size The size of the value (number of bytes copied).
 */
void update_stack(Stack *stack, int index, void *value, size_t value_size);

/**
 * @brief Deep-copies the contents of one stack into another (empty) stack.
 *
 * @param dest The destination stack, as returned by construct_stack().
 * @param src The source stack.
 * @param value_size The size of the values in the stack (number of bytes copied per element).
 */
void stack_cpy(Stack *dest, const Stack src, size_t value_size);

/**
 * @brief Frees every element still on the stack and the stack's own storage.
 *
 * @param stack The stack to destroy; it is left empty and can be reused.
 */
void destroy_stack(Stack *stack);

/**
 * @brief Prints the contents of the stack.
 *
 * @param stack The stack to print.
 * @param format_func A function to format the elements of the stack.
 */
void print_stack(const Stack stack, const char *(*format_func)(void *) );

#endif // __STACK_H__
