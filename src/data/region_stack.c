#include "../utils/utils.h"
#include "../../lib/table_printer.h"

#include "region_stack.h"


static Stack region_stack;


void init_stack_region() { region_stack = construct_stack(); }

void push_region(int region_index) { push(&region_stack, region_index); }

int pop_region() { return pop(&region_stack); }

int peek_region() { return peek(region_stack); }

int get_region_stack_size() { return size(region_stack); }

Stack get_region_stack() { return region_stack; }

void print_stack_region() { print_stack(region_stack); }