#include "../../lib/table_printer.h"
#include "../../lib/colors.h"

#include "../symbol_table/utility.h"
#include "../utils/index_validation.h"
#include "../utils/text_buffer.h"
#include "../utils/utils.h"

#include "region_table.h"

static Region region_table[MAX_REGION_COUNT];
static int region_table_size = 0;


Region construct_region(int size, int nis) {
    Region new_region;

    new_region.size = size;
    new_region.nis = nis;
    new_region.ast = NULL;

    return new_region;
}

void init_region_table(void) {
    memset(region_table, NULL_VALUE, sizeof(region_table));
    init_stack_region();
}

void start_region(void) {
    if (region_table_size >= MAX_REGION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Region table is full\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    int nis = get_region_stack_size();
    int size = (region_table_size > 0) ? nis + 1 : 0;

    region_table[region_table_size] = construct_region(size, nis);
    push_region(region_table_size);

    region_table_size++;
}

void end_region(void) {
    pop_region();
}

void insert_region_row(int index, int size, int nis, AST ast) {
    validate_table_index("Region table row", index, MAX_REGION_COUNT);

    region_table[index].size = size;
    region_table[index].nis = nis;
    region_table[index].ast = ast;

    region_table_size++;
}

int is_region_loaded(int index) {
    return index >= 0 && index < MAX_REGION_COUNT && region_table[index].nis >= 0;
}

int get_region_size(int index) {
    validate_table_index("Region table", index, MAX_REGION_COUNT);
    return region_table[index].size;
}

int get_region_nis(int index) {
    validate_table_index("Region table", index, MAX_REGION_COUNT);
    return region_table[index].nis;
}

int get_current_region_nis(void) {
    // To get the current region NIS, we need to peek the region stack to retrieve the current region index
    // Then we can use the region index to get the NIS from the region table
    return get_region_nis(peek_region());
}

int get_current_region_id(void) {
    return region_table_size - 1;
}

AST get_region_ast(int index) {
    validate_table_index("Region table", index, MAX_REGION_COUNT);
    return region_table[index].ast;
}

void update_region_size(int index, int size) {
    validate_table_index("Region table", index, MAX_REGION_COUNT);
    region_table[index].size = size;
}

void update_region_ast(int index, AST ast) {
    validate_table_index("Region table", index, MAX_REGION_COUNT);
    region_table[index].ast = ast;
}

static char *format_region_row(void *element) {
    Region *region = (Region *) element;
    TextBuffer buffer = construct_text_buffer();
    char prefix[64];
    int prefix_length = snprintf(prefix, sizeof(prefix), "%d|%d|", region->size, region->nis);
    if (prefix_length < 0 || (size_t) prefix_length >= sizeof(prefix)) {
        fprintf(stderr, COLOR_RED "<Error> Failed to format region row\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }
    text_buffer_append_bytes(&buffer, prefix, (size_t) prefix_length);

    char *ast_data = format_ast(region->ast);
    if (ast_data != NULL) text_buffer_append(&buffer, ast_data);
    free(ast_data);

    return release_text_buffer(&buffer);
}

void export_region_table(const char *filename) {
    export_table(filename, region_table, region_table_size, sizeof(Region), format_region_row, "BEGIN_REGION_TABLE",
                 "END_REGION_TABLE");
}

void fprintf_region_table(FILE *out) {
    const int col_width_index = 10;
    const int col_width_size = 10;
    const int col_width_nis = 10;
    const int col_width_ast = 10;

    print_table_title(out, "Region Table");
    print_table_separator(out, 4, col_width_index, col_width_size, col_width_nis, col_width_ast);
    print_table_header(out, 4, col_width_index, "Index", col_width_size, "Size", col_width_nis, "NIS", col_width_ast,
                       "AST");
    print_table_separator(out, 4, col_width_index, col_width_size, col_width_nis, col_width_ast);

    for (int i = 0; i < MAX_REGION_COUNT; i++) {
        if (region_table[i].nis == NULL_VALUE) continue;

        char index_str[20], size_str[20], nis_str[20];

        snprintf(index_str, sizeof(index_str), "%d", i);
        snprintf(size_str, sizeof(size_str), "%d", region_table[i].size);
        snprintf(nis_str, sizeof(nis_str), "%d", region_table[i].nis);

        print_table_row(out, 4, col_width_index, index_str, col_width_size, size_str, col_width_nis, nis_str,
                        col_width_ast, "-1");
    }

    print_table_separator(out, 4, col_width_index, col_width_size, col_width_nis, col_width_ast);

    // Print the ast associated with each region
    for (int i = 0; i < MAX_REGION_COUNT; i++) {
        if (region_table[i].nis == NULL_VALUE) continue;

        char title[50];
        snprintf(title, sizeof(title), "Region %d AST", i);
        print_table_title(out, title);

        print_table_separator(out, 1, 70);
        fprintf_ast(out, region_table[i].ast);
        print_table_separator(out, 1, 70);

        fprintf(out, "\n");
    }
}
