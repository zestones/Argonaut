#include "../symbol_table/representation/representation_table.h"
#include "../symbol_table/declaration/declaration_table.h"

#include "../data/region_table.h"
#include "structure_manager.h"

#include "../utils/scope_tracker.h"
#include "../utils/utils.h"

static structure_manager_context context;

void construct_structure_manager_context(int index_struct_name_lexicographic) {
    context.index_number_of_fields_representation = NULL_VALUE;
    context.index_struct_name_lexicographic = index_struct_name_lexicographic;
    
    context.execution_offset = 0;
    context.number_of_fields = 0;

    initialize_scope_identifier_tracker();
}

void declaration_structure_start() {
    check_type_redefinition(context.index_struct_name_lexicographic);
    context.index_number_of_fields_representation = insert_representation(context.number_of_fields);
    insert_declaration_struct(context.index_struct_name_lexicographic, peek_region(), context.index_number_of_fields_representation);
}

void structure_add_field(int index_lexeme_lexicographic, int index_type_lexicographic) {
    check_type_definition(index_type_lexicographic);
    
    check_scope_redefinition(index_lexeme_lexicographic, "structure field");
    add_identifier_to_scope_tracker(index_lexeme_lexicographic);

    int index_type_declaration = find_declaration_index(index_type_lexicographic);

    insert_representation(index_lexeme_lexicographic);
    insert_representation(index_type_declaration);
    insert_representation(context.execution_offset);

    context.number_of_fields++;
    context.execution_offset += get_declaration_execution(index_type_declaration); 
}

void declaration_structure_end() {
    int index_struct_name_declaration = find_declaration_index(context.index_struct_name_lexicographic);

    update_representation(context.index_number_of_fields_representation, context.number_of_fields);
    update_declaration_execution(index_struct_name_declaration, context.execution_offset);

    clear_scope_identifier_tracker();
}