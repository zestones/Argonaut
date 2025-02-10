#include "../../symbol_table/representation/representation_table.h" 
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../symbol_table/utility.h"
#include "../../data/region_table.h"
#include "../format/formatting.h"
#include "type_inference.h"

/**
 * @brief Resolves the declaration of a struct type.
 * 
 * This function resolves the declaration of a struct type, ensuring that the entity is indeed a struct. It checks
 * for the correct declaration type and provides an error if the type is incorrect.
 * 
 * @param current_node A pointer to the node representing the struct declaration.
 * @return The resolved declaration of the struct, or `NULL_VALUE` if an error occurs.
 * 
 * @note The function verifies that the entity being accessed is a struct type.
 */
static int resolve_struct_declaration(Node *current_node) {
    check_variable_definition(current_node->index_lexicographic);

    if (get_declaration_nature(current_node->index_declaration) == TYPE_VAR || get_declaration_nature(current_node->index_declaration) == TYPE_PARAM) {
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

    return NULL_VALUE;
}

/**
 * @brief Checks if a field is present in a struct.
 * 
 * This function checks if a field is present in a struct by comparing the lexicographic index of the field with
 * the lexicographic indices of the struct's fields. If the field is found, the function returns `1`, otherwise `0`.
 * 
 * @param current_type_declaration The declaration index of the struct type.
 * @param num_fields The number of fields in the struct.
 * @param index_lexicographic The lexicographic index of the field to find.
 * @param field_index A pointer to store the index of the field if found.
 * @return `1` if the field is found, otherwise `0`.
 */
static int is_field_in_struct(int current_type_declaration, int num_fields, int index_lexicographic, int *field_index) {
    for (int i = 0; i < num_fields; i++) {
        if (get_struct_nth_field_lexicographic(current_type_declaration, i) == index_lexicographic) {
            *field_index = i;
            return 1;
        }
    }
    return 0;
}

int resolve_field_access(Node *current_node, int current_type_declaration) {
    int num_fields = get_representation_value(get_declaration_description(current_type_declaration));

    int index_lexicographic = current_node->index_lexicographic;
    int field_index = -1;
    
    if (!is_field_in_struct(current_type_declaration, num_fields, index_lexicographic, &field_index)) {
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

    int field_declaration_index = get_struct_nth_field_declaration(current_type_declaration, field_index);
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
