#include "../../lib/table_printer.h"
#include "../../lib/colors.h" 

#include "../symbol_table/representation_table.h"
#include "../symbol_table/declaration_table.h"
#include "../utils/utils.h"
#include "array_manager.h"

static array_manager_context context;

void construct_array_manager_context(int current_lexeme_lexicographic_index) {
    context.array_size = 0;
    
    context.index_current_lexeme_lexicographic = current_lexeme_lexicographic_index;
    context.index_current_type_lexicographic = NULL_VALUE;

    context.number_of_dimensions = 0;
    context.index_number_of_dimensions_in_representation_table = NULL_VALUE;
}

void declaration_array_start() {
    // TODO: index_current_type_lexicographic is WRONG, should the index type of the declaration table (! NOT THE LEXICOGRAPHIC INDEX) 
    insert_declaration_array(context.index_current_lexeme_lexicographic, insert_representation(context.index_current_type_lexicographic));
    context.index_number_of_dimensions_in_representation_table = insert_representation(context.number_of_dimensions);
}

void array_add_dimension(int min, int max) {
    // TODO: update the number of dimensions in the representation table
    // TODO: insert the min and max values in the representation table 
    context.number_of_dimensions++;

    insert_representation(min);
    insert_representation(max);
}   

void declaration_array_end() {
    update_representation(context.index_number_of_dimensions_in_representation_table, context.number_of_dimensions);
}