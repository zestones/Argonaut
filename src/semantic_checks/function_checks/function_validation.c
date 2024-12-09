#include "../../symbol_table/representation_table.h" 
#include "../../symbol_table/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"


void check_func_proc_definition(int index_lexeme_lexicographic) {
    // TODO: the procedure call should not be assigned to a variable
    if (find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_FUNC) == NULL_VALUE && 
        find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_PROC) == NULL_VALUE) {

        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Function or procedure '%s' is not defined.", get_lexeme(index_lexeme_lexicographic));

        yerror(error);
    }
}

void check_func_proc_redefinition(int index_lexeme_lexicographic, char *type) {
    // TODO: Change condition to 'OR' if we don't want to allow function and procedure with the same name 
    if (find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_FUNC) != NULL_VALUE &&
        find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_PROC) != NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Redefinition of %s '%s'.", type, get_lexeme(index_lexeme_lexicographic));

        yerror(error);
    }
}

void check_func_argument_list(int index_lexeme_lexicographic, Node *argument_list) {
    // Step 1: Retrieve the function declaration index
    int index_declaration = find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_FUNC);

    // Step 2: Access the Representation Table
    int index_representation_start = get_declaration_description(index_declaration);
    int parameter_count = get_representation_value(index_representation_start + 1); // Parameter count

    // Step 3: Traverse the argument list
    Node *current_argument = argument_list->child;
    int argument_index = 0;

    while (current_argument != NULL) {

        // Step 3.1: Check if more arguments were provided than expected
        if (argument_index >= parameter_count) {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(&error, "Function '%s' expects %d arguments, but more were provided.", get_lexeme(index_lexeme_lexicographic), parameter_count);
            yerror(error);
            break;
        }

        // Retrieve expected parameter type for the current argument index
        int param_type_index = index_representation_start + 2 + (argument_index * 2) + 1;
        int expected_type = get_representation_value(param_type_index);

        // Determine argument type from AST
        int argument_type = current_argument->child->type;

        if (argument_type == A_IDENTIFIER) {
            // Step 3.2: Retrieve the type of the identifier
            int index_declaration = current_argument->child->index_declaration;
            int declaration_description = get_declaration_nature(index_declaration);
            
            // FIXME: This is a temporary fix to handle the case where the argument is a variable
            // TODO: move to a separate function/file to handle variable type checking
            if (declaration_description == TYPE_VAR) {
                argument_type = get_declaration_description(index_declaration);
            }
        }

        // Step 4: Check type match
        if (argument_type != expected_type) {
            set_error_type(&error, TYPE_ERROR);
            set_error_message(&error, "Function '%s' expects argument %d to be of type '%s', but type '%s' was provided.",
                            get_lexeme(index_lexeme_lexicographic), argument_index + 1, get_type_string(expected_type), get_type_string(argument_type));
            yerror(error);
        }

        // Move to the next argument and update the argument index
        current_argument = current_argument->sibling;
        argument_index++;
    }

    // Step 5: Check if fewer arguments were provided
    if (argument_index < parameter_count) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Function '%s' expects %d arguments, but only %d were provided.", get_lexeme(index_lexeme_lexicographic), parameter_count, argument_index);
        yerror(error);
    }
}

