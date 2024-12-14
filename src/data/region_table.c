#include "../../lib/table_printer.h"
#include "../../lib/colors.h" 

#include "../symbol_table/utility.h"
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

void init_region_table() {
    memset(region_table, NULL_VALUE, sizeof(region_table));
    init_stack_region();
}

void start_region() {
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

void end_region() { pop_region(); }

int get_region_size(int index) {
    if (index >= MAX_REGION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Get Region Size index out of bounds\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    return region_table[index].size;
}

int get_region_nis(int index) {
    if (index >= MAX_REGION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Get NIS index out of bounds\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    return region_table[index].nis;
}

int get_current_region_nis() { 
    // To get the current region NIS, we need to peek the region stack to retrieve the current region index
    // Then we can use the region index to get the NIS from the region table
    return get_region_nis(peek_region());
}

int get_current_region_id() { return region_table_size - 1; }

void update_region_size(int index, int size) {
   if (index >= MAX_REGION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Update Region Size index out of bounds\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    region_table[index].size = size;
}

void update_region_ast(int index, AST ast) {
    if (index >= MAX_REGION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Update Region AST index out of bounds\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    region_table[index].ast = ast;
}

static char* format_region_row(void* element) {
    Region* region = (Region*)element;
    char* formatted_row = (char*)malloc(512 * sizeof(char));

    sprintf(formatted_row, "%d|%d|", region->size, region->nis);

    char* ast_data = format_ast(region->ast);
    if (ast_data) {
        formatted_row = realloc(formatted_row, strlen(formatted_row) + strlen(ast_data) + 10);
        strcat(formatted_row, ast_data);
        free(ast_data);
    }

    return formatted_row;
}

void export_region_table(const char* filename) {
    export_table(filename, 
                 region_table, 
                 region_table_size, 
                 sizeof(Region), 
                 format_region_row, 
                 "BEGIN_REGION_TABLE", 
                 "END_REGION_TABLE");
}

void fprintf_region_table(FILE* out) {
    const int col_width_index = 10;
    const int col_width_size = 10;
    const int col_width_nis = 10;
    const int col_width_ast = 10;

    print_table_title(out, "Region Table");
    print_table_separator(out, 4, col_width_index, col_width_size, col_width_nis, col_width_ast);
    print_table_header(out, 4, col_width_index, "Index", col_width_size, "Size", col_width_nis, "NIS", col_width_ast, "AST");
    print_table_separator(out, 4, col_width_index, col_width_size, col_width_nis, col_width_ast);

    for (int i = 0; i < MAX_REGION_COUNT; i++) {
        if (region_table[i].nis == NULL_VALUE) continue;

        int char_length = 20;
        char index_str[char_length], size_str[char_length], nis_str[char_length], ast_str[char_length];

        sprintf(index_str, "%d", i);
        sprintf(size_str, "%d", region_table[i].size);
        sprintf(nis_str, "%d", region_table[i].nis);
        // TODO: print in a file ?
        // sprintf(ast_str, "%d", region_table[i].ast);

        print_table_row(out,
                        4, 
                        col_width_index, index_str,
                        col_width_size, size_str,
                        col_width_nis, nis_str,
                        col_width_ast, "-1"
                    );
    }

    print_table_separator(out, 4, col_width_index, col_width_size, col_width_nis, col_width_ast);

    // Print the ast associated with each region
    for (int i = 0; i < MAX_REGION_COUNT; i++) {
        if (region_table[i].nis == NULL_VALUE) continue;

        char title[50];
        sprintf(title, "Region %d AST", i);
        print_table_title(out, title);
        
        print_table_separator(out, 1, 70);
        fprintf_ast(out, region_table[i].ast);
        print_table_separator(out, 1, 70);
        
        fprintf(out, "\n");
    }
}