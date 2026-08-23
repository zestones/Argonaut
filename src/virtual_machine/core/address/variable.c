#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../interpreter/expression/expression.h"

#include "../../stack_management/stack_management.h"
#include "../execution.h"

#include "address_calculation.h"


int get_variable_address(int index_declaration) {
    if (!is_declaration_loaded(index_declaration)) {
        set_error_type(&error, RUN_TIME_ERROR);
        set_error_message(&error, "Corrupted IR: a variable refers to declaration %d, which does not exist.",
                          index_declaration);
        yerror(error);
        exit(EXIT_FAILURE);
    }

    int region_declaration = get_declaration_region(index_declaration);
    int execution_offset = get_declaration_execution(index_declaration);
    int nis_declaration = get_region_nis(region_declaration);

    if (region_declaration == 0) return execution_offset;
    else return execution_offset - (nis_declaration + 1);
}

int get_lvalue_address(Node *access) {
    switch (access->type) {
        case A_IDENTIFIER: return get_variable_address(access->index_declaration);
        case A_ARRAY_ACCESS: return get_array_address(access, NULL_VALUE);
        case A_STRUCT_FIELD_ACCESS: return get_struct_field_address(access, NULL_VALUE, NULL_VALUE);
        default:
            fprintf(stderr, "<variable.c> - node %s is not an lvalue\n", NodeTypeStrings[access->type]);
            exit(EXIT_FAILURE);
    }
}

int get_lvalue_type(Node *access) {
    int type;
    switch (access->type) {
        case A_IDENTIFIER: return get_declaration_description(access->index_declaration);
        case A_ARRAY_ACCESS: {
            int declaration = access->index_declaration;
            int nature = get_declaration_nature(declaration);
            int array_type =
                (nature == TYPE_VAR || nature == TYPE_PARAM) ? get_declaration_description(declaration) : declaration;
            array_element_offset(access, array_type, &type);
            return type;
        }
        case A_STRUCT_FIELD_ACCESS:
            struct_field_offset(access->child, get_declaration_description(access->index_declaration), &type);
            return type;
        default:
            fprintf(stderr, "<variable.c> - node %s is not an lvalue\n", NodeTypeStrings[access->type]);
            exit(EXIT_FAILURE);
    }
}