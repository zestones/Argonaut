#include "../../../lib/table_printer.h"
#include "../../../lib/colors.h" 

#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../data/region_table.h"
#include "declaration_table.h"

#include "../../utils/utils.h"
#include "../../utils/stack.h"
#include "../utility.h"

static Declaration declaration_table[MAX_DECLARATION_COUNT];
static int declaration_table_size = 0;
static int overflow_zone_size = MAX_LEXEME_COUNT;

Declaration construct_declaration(Nature nature, int next, int region, int description, int execution) {
    Declaration new_declaration;

    new_declaration.nature = nature;
    new_declaration.next = next;
    new_declaration.region = region;
    new_declaration.description = description;
    new_declaration.execution = execution;

    return new_declaration;
}

void init_declaration_table() {
    memset(declaration_table, NULL_VALUE, sizeof(declaration_table));
}

static void insert_declaration_overflow_zone(int index, Nature nature, int region, int description, int execution) {
    if (overflow_zone_size >= MAX_DECLARATION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Declaration table overflow zone is full\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    int previous_index = index;
    declaration_table[overflow_zone_size] = construct_declaration(nature, NULL_VALUE, region, description, execution);
    
    while (declaration_table[previous_index].next != NULL_VALUE) {
        previous_index = declaration_table[previous_index].next;
    }

    declaration_table[previous_index].next = overflow_zone_size;
    overflow_zone_size++;
}

void insert_declaration(int index, Nature nature, int region, int description, int execution) {
    if (declaration_table_size >= MAX_LEXEME_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Declaration table is full\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    // If the index is empty, insert the declaration otherwise insert it in the overflow zone //
    if (declaration_table[index].nature == NULL_VALUE) {
        declaration_table[index] = construct_declaration(nature, NULL_VALUE, region, description, execution);
        declaration_table_size++;
    } 
    else {
        insert_declaration_overflow_zone(index, nature, region, description, execution);
    }
}

void insert_declaration_var(int index, int region, int description, int execution) {
    insert_declaration(index, TYPE_VAR, region, description, execution);  
}

void insert_declaration_array(int index, int region, int description) {
    insert_declaration(index, TYPE_ARRAY, region, description, NULL_VALUE);
}

void insert_declaration_struct(int index, int region, int description) {
    insert_declaration(index, TYPE_STRUCT, region, description, NULL_VALUE);
}

void insert_declaration_func(int index, int region, int description) {
    insert_declaration(index, TYPE_FUNC, region, description, get_current_region_id());
}

void insert_declaration_proc(int index, int region, int description) {
    insert_declaration(index, TYPE_PROC, region, description, get_current_region_id());
}

void insert_declaration_param(int index, int region, int description, int execution) {
    insert_declaration(index, TYPE_PARAM, region, description, execution);
}

void insert_declaration_row(int index, Nature nature, int next, int region, int description, int execution) {
    if (index >= MAX_LEXEME_COUNT) {
        Declaration new_declaration = construct_declaration(nature, next, region, description, execution);
        declaration_table[index] = new_declaration;
        overflow_zone_size++;
    } else {
        Declaration new_declaration = construct_declaration(nature, next, region, description, execution);
        declaration_table[index] = new_declaration;
        declaration_table_size++;
    }
}

int is_declaration_base_type(int index) {
    return (declaration_table[index].nature == TYPE_BASE);
}

static int is_valid_declaration_index(int index) {
    if (index >= MAX_DECLARATION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Declaration index out of bounds\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }
    return index != NULL_VALUE;
}

int get_declaration_nature(int index) {
    if (!is_valid_declaration_index(index)) return NULL_VALUE;
    return declaration_table[index].nature;
}

int get_declaration_description(int index) {
    if (!is_valid_declaration_index(index)) return NULL_VALUE;
    return declaration_table[index].description;
}

int get_declaration_region(int index) {
    if (!is_valid_declaration_index(index)) return NULL_VALUE;
    return declaration_table[index].region;
}

int get_declaration_execution(int index) {
    if (!is_valid_declaration_index(index)) return NULL_VALUE;
    return declaration_table[index].execution;
}

static int find_predecessor(int current) {
    for (int i = 0; i < MAX_DECLARATION_COUNT; i++) {
        if (declaration_table[i].next == current) {
            return i; 
        }
    }
    return NULL_VALUE;
}

int get_declaration_lexicographic_index(int index) {
    if (!is_valid_declaration_index(index)) return NULL_VALUE;

    int current = index;
    int predecessor = NULL_VALUE;

    while (1) {
        predecessor = find_predecessor(current);
        if (predecessor == NULL_VALUE) break; // The current declaration is the first one
        current = predecessor;
    }

    return current;
}

Declaration *get_declaration_table() {
    return declaration_table;
}

void update_declaration_execution(int index, int execution) {
    if (index >= MAX_DECLARATION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Update Declaration Execution index out of bounds\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    declaration_table[index].execution = execution;
}

static char* format_declaration_row(void* data) {
    Declaration* declaration = (Declaration*)data;
    char* formatted_row = (char*)malloc(256 * sizeof(char));

    // save some memory by not formatting empty rows
    if (declaration->nature == NULL_VALUE) return NULL;
    sprintf(formatted_row, "%d|%d|%d|%d|%d", 
            declaration->nature, 
            declaration->next, 
            declaration->region, 
            declaration->description, 
            declaration->execution);
    
    return formatted_row;
}

void export_declaration_table(const char* filename) {
    export_table(filename, 
                 declaration_table, 
                 MAX_DECLARATION_COUNT, 
                 sizeof(Declaration), 
                 format_declaration_row, 
                 "BEGIN_DECLARATION_TABLE", 
                 "END_DECLARATION_TABLE");
}

void fprintf_declaration_table(FILE* out) {
    const int col_width_index = 10;
    const int col_width_nature = 15;
    const int col_width_next = 10;
    const int col_width_region = 15;
    const int col_width_description = 15;
    const int col_width_execution = 15;

    print_table_title(out, "Declaration Table");
    print_table_separator(out, 6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);
    print_table_header(out, 6, col_width_index, "Index", col_width_nature, "Nature", col_width_next, "Suivant", col_width_region, "Région", col_width_description, "Description", col_width_execution, "Exécution");
    print_table_separator(out, 6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);

    for (int i = 0; i < MAX_DECLARATION_COUNT; i++) {
        if (declaration_table[i].nature == NULL_VALUE) continue;
        if (i == MAX_LEXEME_COUNT) {
            print_table_separator(out, 6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);
            print_table_separator(out, 6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);
        }

        int char_length = 20;
        char region_str[char_length], next_str[char_length], index_str[char_length];
        char description_str[char_length], execution_str[char_length], nature_str[char_length];

        sprintf(region_str, "%d", declaration_table[i].region);
        sprintf(next_str, "%d", declaration_table[i].next);
        sprintf(description_str, "%d", declaration_table[i].description);
        sprintf(execution_str, "%d", declaration_table[i].execution);
        sprintf(index_str, "%d", i);
        sprintf(nature_str, "%s", nature_to_string(declaration_table[i].nature));

        print_table_row(out, 
                        6, 
                        col_width_index, index_str,
                        col_width_nature, nature_str,
                        col_width_next, next_str,
                        col_width_region, region_str,
                        col_width_description, description_str,
                        col_width_execution, execution_str
                    );

        // Print separator after the base types
        if (i == 3) {
            print_table_separator(out, 6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);
        }
    }

    print_table_separator(out, 6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);
}
