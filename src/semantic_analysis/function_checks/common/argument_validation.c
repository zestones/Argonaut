#include "../../../symbol_table/representation/representation_table.h" 
#include "../../../symbol_table/declaration/declaration_table.h" 
#include "../../../type_system/type_inference/type_inference.h"
#include "../../../type_system/format/formatting.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"
#include "../../../data/region_table.h"

/**
 * @brief Resolves the type of a given argument node.
 * 
 * This function determines the type of an argument by examining its node type and resolving
 * it appropriately for various cases, such as identifiers, function calls, structure field access,
 * array access, or literals.
 * 
 * @param argument_node A pointer to the `Node` representing the argument.
 * @return The resolved type of the argument as an integer, or `NULL_VALUE` if the type cannot be resolved.
 */
static int resolve_argument_type(Node *argument_node) {
    int argument_type = argument_node->child->type;
    int type = NULL_VALUE;

    if (argument_type == A_IDENTIFIER) {
        int index_declaration = argument_node->child->index_declaration;
        int nature = get_declaration_nature(index_declaration);

        if (nature == TYPE_VAR || nature == TYPE_PARAM) {
            type = get_declaration_description(index_declaration);
        }
    } 
    else if (argument_type == A_FUNC_PROC_CALL_STATEMENT) {
        type = resolve_func_proc_return_type(argument_node->child);
    }
    else if (argument_type == A_STRUCT_FIELD_ACCESS) {
        type = resolve_struct_field_access_type(argument_node->child);
    }
    else if (argument_type == A_ARRAY_ACCESS) {
        type = resolve_array_access_type(argument_node->child);
    }
    else if (argument_type == A_ADD_OP || argument_type == A_SUB_OP || argument_type == A_MUL_OP || argument_type == A_DIV_OP || argument_type == A_MODULO_OP) {
        type = resolve_expression_type(argument_node->child);
    } 
    else if (argument_type == A_FLOAT_LITERAL || argument_type == A_INTEGER_LITERAL || argument_type == A_CHARACTER_LITERAL || argument_type == A_BOOLEAN_LITERAL) {
        type = argument_type;
    }

    return type;
}

/**
 * @brief Validates the type of a function or procedure argument against the expected type.
 * 
 * This function checks whether a given argument matches the expected type for a function or procedure.
 * If the types do not match, a detailed type error is logged, including information about the 
 * expected type, provided type, and argument index.
 * 
 * @param current_argument A pointer to the `Node` representing the current argument.
 * @param expected_type The expected type of the argument.
 * @param argument_index The index of the argument in the function or procedure's parameter list (zero-based).
 * @param index_lexeme_lexicographic The lexeme index of the function or procedure.
 * @param nature The nature of the callable (e.g., `TYPE_FUNC` for functions, `TYPE_PROC` for procedures).
 */
static void validate_argument_type(Node *current_argument, int expected_type, int argument_index, int index_lexeme_lexicographic, Nature nature) {
    int argument_type = resolve_argument_type(current_argument);

    if (argument_type != expected_type) {
        set_error_type(&error, TYPE_ERROR);
        char *func_proc = (nature == TYPE_FUNC) ? "Function" : "Procedure";
        set_error_message(&error, 
            "%s '%s' expects argument %d to be of type '%s' at %s.\n"
            "  But the provided argument '%s' is of type '%s'.\n"
            "  Ensure the argument type matches the expected type for the function or procedure.\n",
            func_proc, 
            get_lexeme(index_lexeme_lexicographic), 
            argument_index + 1, 
            get_lexeme(expected_type),
            get_formatted_location(),
            format_expression(current_argument->child),
            (argument_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(argument_type)
        );
        yerror(error);
    }
}

void validate_argument_count(Node *argument_list, int parameter_count) {
    Node *current_argument = (argument_list != NULL) ? argument_list->child : NULL;
    int argument_index = 0;

    while (current_argument != NULL) {
        if (argument_index >= parameter_count) {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(&error, 
                "Function expects %d arguments at %s.\n"
                "  However, more arguments were provided.\n"
                "  Ensure the correct number of arguments are passed to the function.\n",
                parameter_count,
                get_formatted_location()
            );
            yerror(error);
            break;
        }
    
        current_argument = current_argument->sibling;
        argument_index++;
    }

    if (argument_index < parameter_count) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, 
            "Function expects %d arguments at %s.\n"
            "  However, only %d arguments were provided.\n"
            "  Ensure the correct number of arguments are passed to the function.\n",
            parameter_count, 
            get_formatted_location(),
            argument_index
        );
        yerror(error);
    }
}

void validate_each_argument(Node *argument_list, int parameter_count, int index_lexeme_lexicographic, Nature nature) {
    int index_declaration = find_declaration_index_by_nature(index_lexeme_lexicographic, nature);
    Node *current_argument = (argument_list != NULL) ? argument_list->child : NULL;
    
    int argument_index = 0;
    while (current_argument != NULL && argument_index < parameter_count) {
        int expected_type = resolve_func_proc_parameter_type_at(argument_index, index_declaration, nature);
        validate_argument_type(current_argument, expected_type, argument_index, index_lexeme_lexicographic, nature);

        current_argument = current_argument->sibling;
        argument_index++;
    }
}
