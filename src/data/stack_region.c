#include "../utils/utils.h"
#include "../../lib/table_printer.h"

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

int get_region_stack_size() { return region_stack_size; }

void print_stack_region() {
    const int col_width_index = 10;
    const int col_width_region = 15;

    print_table_title("Region Stack");
    print_table_separator(2, col_width_index, col_width_region);
    print_table_header(2, col_width_index, "Index", col_width_region, "Region");
    print_table_separator(2, col_width_index, col_width_region);

    for (int i = 0; i < region_stack_size; i++) {
        char index_str[20], region_str[20];
        sprintf(index_str, "%d", i);
        sprintf(region_str, "%d", region_stack[i]);

        print_table_row(2, col_width_index, index_str, col_width_region, region_str);
    }
}