#include "../../../symbol_table/representation/representation_table.h" 
#include "../../../symbol_table/declaration/declaration_table.h" 
#include "../../../type_system/type_system.h"
#include "../../../lexer/lexeme_table.h"
#include "../../../data/region_table.h"
#include "../common/format_specifiers.h"


void check_input_proc_argument_list(Node *format_string, Node *argument_list) {
    const char *format = get_lexeme(format_string->index_lexicographic);
    int expected_count = count_format_specifiers(format);

    int actual_count = count_argument_list(argument_list);
    if (expected_count != actual_count) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Mismatch: Format string expects %d arguments, but %d were provided.", 
                          expected_count, actual_count);
        yerror(error);
        return;
    }

    Node *current_arg = argument_list->child;
    for (int i = 0; i < expected_count; ++i) {
        char specifier = get_nth_format_specifier(format, i);

        check_variable_definition(current_arg->child->index_lexicographic);
        int entity_type = resolve_expression_type(current_arg->child);

        if (!is_compatible_with_format_specifier(entity_type, specifier)) {
            set_error_type(&error, TYPE_ERROR);
            set_error_message(&error, "Type mismatch: Input entity %d has type '%s', but specifier '%c' requires a different type.",
                              i + 1, get_lexeme(entity_type), specifier);
            yerror(error);
            return;
        }

        current_arg = current_arg->sibling;
    }
}
