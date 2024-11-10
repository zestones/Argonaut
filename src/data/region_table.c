#include "../../lib/table_printer.h"
#include "../../lib/colors.h" 

#include "../utils/utils.h"
#include "region_table.h"

static Region region_table[MAX_REGION_COUNT];
static int region_table_size = 0;

Region construct_region(int size, int nis) {
    Region new_region;

    new_region.size = size;
    new_region.nis = nis;
    new_region.ast = NULL_VALUE;

    return new_region;
}

void init_region_table() {
    memset(region_table, NULL_VALUE, sizeof(region_table));
}

void insert_region(int nis) {
    if (region_table_size >= MAX_REGION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Region table is full\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    region_table[region_table_size] = construct_region(NULL_VALUE, nis);
    region_table_size++;
}

void print_region_table() {
    const int col_width_size = 10;
    const int col_width_nis = 10;
    const int col_width_ast = 10;

    print_table_title("Region Table");
    print_table_separator(3, col_width_size, col_width_nis, col_width_ast);
    print_table_header(3, col_width_size, "Size", col_width_nis, "NIS", col_width_ast, "AST");
    print_table_separator(3, col_width_size, col_width_nis, col_width_ast);

    for (int i = 0; i < MAX_REGION_COUNT; i++) {
        if (region_table[i].nis == NULL_VALUE) continue;

        int char_length = 20;
        char size_str[char_length], nis_str[char_length], ast_str[char_length];

        sprintf(size_str, "%d", region_table[i].size);
        sprintf(nis_str, "%d", region_table[i].nis);
        sprintf(ast_str, "%d", region_table[i].ast);

        print_table_row(3, col_width_size, size_str, col_width_nis, nis_str, col_width_ast, ast_str);
    }

    print_table_separator(3, col_width_size, col_width_nis, col_width_ast);
}