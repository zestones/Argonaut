#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

Stack construct_stack();

int is_empty(const Stack stack);

int is_full(const Stack stack);

int size(const Stack stack);

void push(Stack *stack, int value);

int pop(Stack *stack);

int peek(const Stack stack);

void stack_cpy(Stack *dest, const Stack src);

void print_stack(const Stack stack);


#endif // __STACK_H__
