#include "../../../lib/table_printer.h"
#include "../../../lib/colors.h"

#include "../../utils/utils.h"
#include "../../utils/index_validation.h"
#include "../../utils/memory.h"
#include "../utility.h"

#include "representation_table.h"

static int representation_table[MAX_REPRESENTATION_COUNT];
static int representation_table_size = 0;


void init_representation_table(void) {
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

void insert_representation_row(int index, int value) {
    validate_table_index("Representation table", index, MAX_REPRESENTATION_COUNT);
    representation_table[index] = value;
    representation_table_size++;
}

void update_representation(int index, int value) {
    validate_table_index("Representation table", index, MAX_REPRESENTATION_COUNT);
    representation_table[index] = value;
}

int get_representation_value(int index) {
    validate_table_index("Representation table", index, MAX_REPRESENTATION_COUNT);
    return representation_table[index];
}

static char *format_representation_row(void *element) {
    int *representation = (int *) element;
    const size_t room = 32;
    char *formatted_row = xmalloc(room);

    snprintf(formatted_row, room, "%d", *representation);
    return formatted_row;
}

void export_representation_table(const char *filename) {
    export_table(filename, representation_table, representation_table_size, sizeof(int), format_representation_row,
                 "BEGIN_REPRESENTATION_TABLE", "END_REPRESENTATION_TABLE");
}

void fprintf_representation_table(FILE *out) {
    const int col_width_index = 10;
    const int col_width_value = 20;

    print_table_title(out, "Representation Table");
    print_table_separator(out, 2, col_width_index, col_width_value);
    print_table_header(out, 2, col_width_index, "Index", col_width_value, "Value");
    print_table_separator(out, 2, col_width_index, col_width_value);

    for (int i = 0; i < representation_table_size; i++) {
        char index_str[32], value_str[32];
        snprintf(index_str, sizeof(index_str), "%d", i);
        snprintf(value_str, sizeof(value_str), "%d", representation_table[i]);

        print_table_row(out, 2, col_width_index, index_str, col_width_value, value_str);
    }

    print_table_separator(out, 2, col_width_index, col_width_value);
}
