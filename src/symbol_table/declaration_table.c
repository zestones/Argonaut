#include "../../lib/table_printer.h"
#include "../../lib/colors.h"
#include "declaration_table.h"

static Declaration declaration_table[MAX_DECLARATION_COUNT];
static int declaration_table_size = 0;
static int declaration_buffer = MAX_DECLARATION_COUNT/2;

Declaration construct_declaration(Nature nature, int next, int region, int description, int execution) {
    Declaration new_declaration;

    new_declaration.nature = nature;
    new_declaration.next = next;
    new_declaration.region = region;
    new_declaration.description = description;
    new_declaration.execution = execution;

    return new_declaration;
}

void init_declaration_table()
{
    memset(declaration_table, -1, sizeof(declaration_table));

    insert_declaration(TYPE_B, -1, 0, 0, 1);
    insert_declaration(TYPE_B, -1, 0, 1, 1);
    insert_declaration(TYPE_B, -1, 0, 2, 1);
    insert_declaration(TYPE_B, -1, 0, 3, 1);
}

void insert_declaration(Nature nature, int next, int region, int description, int execution) {
    declaration_table[declaration_table_size] = construct_declaration(nature, next, region, description, execution);
    declaration_table_size++;
}

void print_declaration_table() {
    const int col_width_index = 10;
    const int col_width_nature = 10;
    const int col_width_next = 10;
    const int col_width_region = 10;
    const int col_width_description = 10;
    const int col_width_execution = 10;

    fprintf(stdout, COLOR_BOLD "\nDeclaration table:\n" COLOR_RESET);
    print_table_separator(6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);
    print_table_header(6, col_width_index, "Index", col_width_nature, "Nature", col_width_next, "Suivant", col_width_region, "Région", col_width_description, "Description", col_width_execution, "Exécution");
    print_table_separator(6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);

    for (int i = 0; i < declaration_table_size; i++) {
        char region_str[10], next_str[10], index_str[10];
        char description_str[10], execution_str[10], nature_str[10];

        sprintf(region_str, "%d", declaration_table[i].region);
        sprintf(next_str, "%d", declaration_table[i].next);
        sprintf(description_str, "%d", declaration_table[i].description);
        sprintf(execution_str, "%d", declaration_table[i].execution);
        sprintf(index_str, "%d", i);
        sprintf(nature_str, "%s", nature_to_string(declaration_table[i].nature));

        print_table_row(6, 
                        col_width_index, index_str,
                        col_width_nature, nature_str,
                        col_width_next, next_str,
                        col_width_region, region_str,
                        col_width_description, description_str,
                        col_width_execution, execution_str
                    );
    }

    print_table_separator(6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);
}