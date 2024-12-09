#include "../symbol_table/representation_table.h" 
#include "../symbol_table/declaration_table.h" 
#include "../ast/ast.h"

int resolve_array_access_type(Node *array_access) {
    int index_array_declaration = get_declaration_description(array_access->index_declaration);
    int index_representation = get_declaration_description(index_array_declaration);

    return get_representation_value(index_representation);
}