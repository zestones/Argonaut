#include "../../lib/table_printer.h"
#include "../../lib/colors.h" 

#include "representation_table.h"
#include "../utils/utils.h"

static int representation_table[MAX_REPRESENTATION_COUNT];
static int representation_table_size = 0;


void init_representation_table() {
    memset(representation_table, NULL_VALUE, sizeof(representation_table));
}

int insert_representation(int value) {
    int current_index = representation_table_size;
    if (representation_table_size >= MAX_REPRESENTATION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Representation table is full\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    representation_table[representation_table_size] = value;
    representation_table_size++;
    
    return current_index;
}

void update_representation(int index, int value) {
    if (index < 0 || index >= MAX_REPRESENTATION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error>: Index %d out of bounds. Valid range is 0 to %d.\n" COLOR_RESET, index, MAX_REPRESENTATION_COUNT);
        exit(EXIT_FAILURE);
    }

    representation_table[index] = value;
}

int get_representation_value(int index) {
    if (index < 0 || index >= MAX_REPRESENTATION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error>: Index %d out of bounds. Valid range is 0 to %d.\n" COLOR_RESET, index, MAX_REPRESENTATION_COUNT);
        exit(EXIT_FAILURE);
    }

    return representation_table[index];
}

void print_representation_table() {
    const int col_width_index = 10;
    const int col_width_value = 20;

    print_table_title("Representation Table");
    print_table_separator(2, col_width_index, col_width_value);
    print_table_header(2, col_width_index, "Index", col_width_value, "Value");
    print_table_separator(2, col_width_index, col_width_value);

    for (int i = 0; i < representation_table_size; i++) {
        char index_str[10], value_str[10];
        sprintf(index_str, "%d", i);
        sprintf(value_str, "%d", representation_table[i]);

        print_table_row(2,
                        col_width_index, index_str,
                        col_width_value, value_str);
    }

    print_table_separator(2, col_width_index, col_width_value);
}