#include "../../symbol_table/representation_table.h" 
#include "../../symbol_table/declaration_table.h" 
#include "../../type_system/type_system.h"
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"


static int determine_argument_type(Node *argument_node) {
    int argument_type = argument_node->child->type;

    if (argument_type == A_IDENTIFIER) {
        int index_declaration = argument_node->child->index_declaration;
        int nature = get_declaration_nature(index_declaration);

        if (nature == TYPE_VAR) {
            argument_type = get_declaration_description(index_declaration);
        }
    } 
    else if (argument_type == A_FUNCTION_CALL_STATEMENT) {
        argument_type = resolve_function_call_type(argument_node);
    }
    else if (argument_type == A_STRUCT_FIELD_ACCESS) {
        argument_type = resolve_struct_field_access_type(argument_node->child);
    }
    // TODO: handle array access

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
        int expected_type = resolve_function_parameter_type_at(argument_index, index_representation_start);
        validate_argument_type(current_argument, expected_type, argument_index, index_lexeme_lexicographic);

        current_argument = current_argument->sibling;
        argument_index++;
    }
}
