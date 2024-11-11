#include "../utils/utils.h"

#include "region_table.h"
#include "stack_region.h"


static int region_stack[MAX_REGION_COUNT];
static int region_stack_size = 0;


void init_stack_region() {  
    memset(region_stack, NULL_VALUE, sizeof(region_stack));
}

void push_region(int region_index) {
    if (region_stack_size >= MAX_REGION_COUNT) {
        fprintf(stderr, "<Error> Region stack overflow\n");
        exit(EXIT_FAILURE);
    }

    region_stack[region_stack_size++] = region_index;
}

int pop_region() {
    if (region_stack_size <= 0) {
        fprintf(stderr, "<Error> Region stack underflow\n");
        exit(EXIT_FAILURE);
    }

    return region_stack[--region_stack_size];
}

int peek_region() {
    if (region_stack_size <= 0) {
        return 0;
    }

    return region_stack[region_stack_size - 1];
}