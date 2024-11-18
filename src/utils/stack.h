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


#define MAX_SIZE 100

/**
 * @struct Stack
 * @brief A structure to represent a stack.
 * 
 * This structure defines a stack with a fixed maximum size. It contains an array to hold the stack elements
 * and an integer to keep track of the index of the top element.
 */
typedef struct {
    int data[MAX_SIZE]; // Array to store the elements of the stack.
    int top;            // Index of the top element in the stack.
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
 */
void push(Stack *stack, int value);

/**
 * @brief Pops a value from the stack.
 *
 * @param stack The stack to pop the value from.
 * @return The value popped from the stack.
 */
int pop(Stack *stack);

/**
 * @brief Peeks at the top value of the stack without popping it.
 *
 * @param stack The stack to peek at.
 * @return The value at the top of the stack.
 */
int peek(const Stack stack);

/**
 * @brief Copies the contents of one stack to another.
 *
 * @param dest The destination stack.
 * @param src The source stack.
 */
void stack_cpy(Stack *dest, const Stack src);

/**
 * @brief Prints the contents of the stack.
 *
 * @param stack The stack to print.
 */
void print_stack(const Stack stack);


#endif // __STACK_H__
