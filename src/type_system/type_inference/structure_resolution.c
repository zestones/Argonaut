#include "../../symbol_table/representation/representation_table.h" 
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"
#include "../format/formatting.h"
#include "type_inference.h"

static int resolve_struct_declaration(Node *current_node) {
    check_variable_definition(current_node->index_lexicographic);

    if (get_declaration_nature(current_node->index_declaration) == TYPE_VAR) {
        int struct_index_declaration = get_declaration_description(current_node->index_declaration);
        
        if (get_declaration_nature(struct_index_declaration) != TYPE_STRUCT) {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(&error, 
                "Field access error at %s.\n"
                "  The entity '%s' is not a struct but is of type '%s'.\n"
                "  Ensure that the entity is a struct before attempting to access its fields.\n",
                get_formatted_location(),
                get_lexeme(current_node->index_lexicographic),
                get_lexeme(get_declaration_description(current_node->index_declaration))
            );
            yerror(error);
            return NULL_VALUE;
        }

        return struct_index_declaration;
    }
}

static int get_struct_field_index_declaration(int current_type_representation, int field_index) {
    return get_representation_value(current_type_representation + 2 + (field_index * 3));
}

static int get_struct_field_index_lexico(int current_type_representation, int field_index) {
    return get_representation_value(current_type_representation + 1 + (field_index * 3));
}

static int is_field_in_struct(int current_type_representation, int num_fields, int index_lexicographic, int *field_index) {
    for (int i = 0; i < num_fields; i++) {
        if (get_struct_field_index_lexico(current_type_representation, i) == index_lexicographic) {
            *field_index = i;
            return 1;
        }
    }
    return 0;
}

int resolve_field_access(Node *current_node, int current_type_declaration) {
    int current_type_representation = get_declaration_description(current_type_declaration);
    int num_fields = get_representation_value(current_type_representation);

    int index_lexicographic = current_node->index_lexicographic;
    int field_index = -1;
    
    if (!is_field_in_struct(current_type_representation, num_fields, index_lexicographic, &field_index)) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(
            &error,
            "Field access error at %s.\n"
            "  The field '%s' does not exist in the struct '%s'.\n"
            "  Ensure the field name is correct and defined within the struct type.\n",
            get_formatted_location(),
            get_lexeme(index_lexicographic),
            get_lexeme(get_declaration_lexicographic_index(current_type_declaration))
        );
        yerror(error);
        return NULL_VALUE;
    }

    int field_declaration_index = get_struct_field_index_declaration(current_type_representation, field_index);
    if (get_declaration_nature(field_declaration_index) == TYPE_ARRAY && current_node->type == A_ARRAY_ACCESS) {
        current_node->index_declaration = field_declaration_index;
        int value = resolve_array_access_type(current_node);
        return value;
    }
    
    if (get_declaration_nature(field_declaration_index) != TYPE_STRUCT && current_node->child != NULL) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, 
            "Field access error at %s.\n"
            "  The entity '%s' is not a struct, so field access is not possible.\n"
            "  Verify that the entity is a struct before attempting to access its fields.\n",
            get_formatted_location(),
            get_lexeme(index_lexicographic)
        );
        yerror(error);
        return NULL_VALUE;
    }

    return field_declaration_index;
}

int resolve_struct_field_access_type(Node *struct_field_access) {
    Node *current_node = struct_field_access;

    int current_type_declaration = resolve_struct_declaration(current_node);
    if (current_type_declaration == NULL_VALUE) {
        return NULL_VALUE;
    }

    current_node = current_node->child;
    while (current_node != NULL && (current_node->type == A_STRUCT_FIELD_ACCESS || current_node->type == A_ARRAY_ACCESS)) {
        current_type_declaration = resolve_field_access(current_node, current_type_declaration);
        if (current_type_declaration == NULL_VALUE) {
            return NULL_VALUE;
        }

        current_node = current_node->child;
    }

    return current_type_declaration;
}
