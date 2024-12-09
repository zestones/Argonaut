
#include "../../symbol_table/representation_table.h" 
#include "../../symbol_table/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"

static int resolve_struct_field_access_type(Node *struct_field_access) {
    Node *current_node = struct_field_access;

    int current_type_declaration = find_declaration_index(get_declaration_description(current_node->index_declaration));
    int current_type_representation = get_declaration_description(current_type_declaration);

    if (get_declaration_nature(current_type_declaration) != TYPE_STRUCT) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Field access is only allowed on struct types.");
        yerror(error);
        return NULL_VALUE;
    }

    current_node = current_node->child;
    while (current_node != NULL && current_node->type == A_STRUCT_FIELD_ACCESS) {
        int num_fields = get_representation_value(current_type_representation);

        // Retrieve the field's lexeme
        int index_lexicographic = current_node->index_lexicographic;
        int field_declaration_index = -1;

        // Search for the field in the struct representation
        int field_found = 0;
        for (int i = 0; i < num_fields; i++) {
            int field_lexeme_representation = get_representation_value(current_type_representation + 1 + (i * 3));
            if (field_lexeme_representation == index_lexicographic) {
                field_declaration_index = get_representation_value(current_type_representation + 2 + (i * 3));
                field_found = 1;
                break;
            }
        }

        if (!field_found) {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(&error, "Field '%s' does not exist in struct.", get_lexeme(index_lexicographic));
            yerror(error);
            return NULL_VALUE;
        }

        // Update the current type for the next access
        current_type_declaration = field_declaration_index;
        current_type_representation = get_declaration_description(current_type_declaration);

        if (get_declaration_nature(current_type_declaration) != TYPE_STRUCT && current_node->child != NULL) {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(&error, "Field '%s' is not a struct, so further field access is invalid.", get_lexeme(index_lexicographic));
            yerror(error);
            return NULL_VALUE;
        }

        // Move to the next field in the chain
        current_node = current_node->child;
    }

    // Return the type of the final field
    return current_type_declaration;
}


static int determine_argument_type(Node *argument_node) {
    int argument_type = argument_node->child->type;

    if (argument_type == A_IDENTIFIER) {
        int index_declaration = argument_node->child->index_declaration;
        int nature = get_declaration_nature(index_declaration);

        // TODO: handle TYPE_STRUCT and TYPE_ARRAY
        if (nature == TYPE_VAR) {
            argument_type = get_declaration_description(index_declaration);
        }
    } 
    else if (argument_type == A_FUNCTION_CALL_STATEMENT) {
        int index_declaration = argument_node->child->index_declaration;
        int index_representation = get_declaration_description(index_declaration);
        argument_type = get_representation_value(index_representation);
    }
    else if (argument_type == A_STRUCT_FIELD_ACCESS) {
        argument_type = resolve_struct_field_access_type(argument_node->child);
    }

    return argument_type;
}

static void validate_argument_type(Node *current_argument, int expected_type, int argument_index, int index_lexeme_lexicographic) {
    int argument_type = determine_argument_type(current_argument);

    if (argument_type != expected_type) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, "Function '%s' expects argument %d to be of type '%s', but type '%s' was provided.",
                            get_lexeme(index_lexeme_lexicographic), argument_index + 1, get_lexeme(expected_type), get_lexeme(argument_type));
        yerror(error);
    }
}

void validate_argument_count(Node *argument_list, int parameter_count) {
    Node *current_argument = (argument_list != NULL) ? argument_list->child : NULL;
    int argument_index = 0;

    while (current_argument != NULL) {
        if (argument_index >= parameter_count) {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(&error, "Function expects %d arguments, but more were provided.", parameter_count);
            yerror(error);
            break;
        }
    
        current_argument = current_argument->sibling;
        argument_index++;
    }

    if (argument_index < parameter_count) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Function expects %d arguments, but only %d were provided.", parameter_count, argument_index);
        yerror(error);
    }
}


void validate_each_argument(Node *argument_list, int index_representation_start, int parameter_count, int index_lexeme_lexicographic) {
    Node *current_argument = (argument_list != NULL) ? argument_list->child : NULL;
    int argument_index = 0;

    while (current_argument != NULL && argument_index < parameter_count) {
        int param_type_index = index_representation_start + 2 + (argument_index * 2) + 1;
        int expected_type = get_representation_value(param_type_index);

        validate_argument_type(current_argument, expected_type, argument_index, index_lexeme_lexicographic);

        current_argument = current_argument->sibling;
        argument_index++;
    }
}
