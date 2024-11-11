#ifndef __STACK_REGION_H__
#define __STACK_REGION_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utils/stack.h"


void init_stack_region();

void push_region(int region_index);

int pop_region();

int peek_region();

int get_region_stack_size();

Stack get_region_stack();

void print_stack_region() ;

#endif // __STACK_REGION_H__

