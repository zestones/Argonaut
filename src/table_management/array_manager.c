#include "../symbol_table/representation/representation_table.h"
#include "../symbol_table/declaration/declaration_table.h"
#include "../data/region_table.h"
#include "../utils/utils.h"
#include "array_manager.h"

static array_manager_context context;

void construct_array_manager_context(int index_array_name_lexicographic) {
    context.array_size = 0;
    context.index_array_name_lexicographic = index_array_name_lexicographic;

    context.index_number_of_dimensions_representation = NULL_VALUE;
    context.index_array_type_representation = NULL_VALUE;
    context.number_of_dimensions = 0;
}

void declaration_array_start() {
    check_type_redefinition(context.index_array_name_lexicographic, TYPE_ARRAY);
    context.index_array_type_representation = insert_representation(NULL_VALUE);
    insert_declaration_array(context.index_array_name_lexicographic, peek_region(), context.index_array_type_representation);
    context.index_number_of_dimensions_representation = insert_representation(context.number_of_dimensions);
}

void array_add_dimension(int min, int max) {
    context.number_of_dimensions++;

    insert_representation(min);
    insert_representation(max);

    if (context.array_size == 0) context.array_size = max - min + 1;
    else context.array_size *= (max - min + 1);
}   

void declaration_array_end(int index_type_lexicographic) {
    check_type_definition(index_type_lexicographic);

    int index_array_name_declaration = find_declaration_index(context.index_array_name_lexicographic);
    int index_type_of_array_declaration = find_declaration_index(index_type_lexicographic);

    update_representation(context.index_number_of_dimensions_representation, context.number_of_dimensions);
    update_representation(context.index_array_type_representation, index_type_lexicographic);

    int execution = get_declaration_execution(index_type_of_array_declaration);
    update_declaration_execution(index_array_name_declaration, context.array_size * execution);
}