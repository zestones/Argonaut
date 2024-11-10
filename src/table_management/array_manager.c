#include "../symbol_table/representation_table.h"
#include "../symbol_table/declaration_table.h"
#include "../utils/utils.h"
#include "array_manager.h"

static array_manager_context context;

void construct_array_manager_context(int index_lexeme_lexicographic) {
    context.array_size = 0;
    
    context.index_lexeme_lexicographic = index_lexeme_lexicographic;
    context.index_type_declaration = NULL_VALUE;

    context.number_of_dimensions = 0;
    context.index_number_of_dimensions_representation = NULL_VALUE;
}

void declaration_array_start() {
    // TODO: index_type_declaration is WRONG, should be the index type of the declaration table (! NOT THE LEXICOGRAPHIC INDEX) 
    insert_declaration_array(context.index_lexeme_lexicographic, current_nis, insert_representation(context.index_type_declaration));
    context.index_number_of_dimensions_representation = insert_representation(context.number_of_dimensions);
}

void array_add_dimension(int min, int max) {
    context.number_of_dimensions++;

    insert_representation(min);
    insert_representation(max);
}   

void declaration_array_end() {
    update_representation(context.index_number_of_dimensions_representation, context.number_of_dimensions);
}