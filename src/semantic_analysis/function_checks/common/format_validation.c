#include "../../../data/region_table.h"
#include "../../../symbol_table/declaration/declaration_table.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"
#include "../../../symbol_table/representation/representation_table.h"
#include "../../../type_system/type_inference/type_inference.h"

#include "format_specifiers.h"
#include "format_validation.h"

static int resolve_format_argument_type(Node *argument, FormatArgumentKind kind) {
    if (kind == INPUT_FORMAT_ARGUMENTS && argument->type == A_IDENTIFIER)
        check_variable_definition(argument->index_lexicographic);
    return resolve_expression_type(argument);
}

static void report_argument_count_mismatch(int expected_count, int actual_count) {
    set_error_type(&error, SEMANTIC_ERROR);
    set_error_message(&error,
                      "Argument mismatch in format string at %s.\n"
                      "  Format string expects %d arguments, but %d were provided.\n"
                      "  Ensure the correct number of arguments are passed to match the format string.\n",
                      get_formatted_location(), expected_count, actual_count);
    yerror(error);
}

static void report_argument_type_mismatch(int argument_index, int argument_type, char specifier,
                                          FormatArgumentKind kind) {
    set_error_type(&error, TYPE_ERROR);
    if (kind == INPUT_FORMAT_ARGUMENTS) {
        set_error_message(&error,
                          "Type mismatch for argument %d at %s.\n"
                          "  Input entity %d has type '%s', but specifier '%c' requires a different type.\n"
                          "  Ensure the input type matches the expected type for the specifier.\n",
                          argument_index, get_formatted_location(), argument_index, type_name(argument_type),
                          specifier);
    } else {
        set_error_message(&error,
                          "Type mismatch for argument %d at %s.\n"
                          "  Argument '%d' has type '%s', but specifier '%c' expects a different type.\n"
                          "  Ensure the argument type matches the expected type for the specifier.\n",
                          argument_index, get_formatted_location(), argument_index, type_name(argument_type),
                          specifier);
    }
    yerror(error);
}

void validate_format_arguments(Node *format_string, Node *argument_list, FormatArgumentKind kind) {
    const char *format = get_lexeme(format_string->index_lexicographic);
    int expected_count = count_format_specifiers(format);
    int actual_count = count_argument_list(argument_list);
    if (expected_count != actual_count) {
        report_argument_count_mismatch(expected_count, actual_count);
        return;
    }

    Node *current_argument = argument_list->child;
    const char *format_cursor = format;
    for (int i = 0; i < expected_count; i++) {
        const char *specifier = find_next_format_specifier(format_cursor);
        if (specifier == NULL) return;

        int argument_type = resolve_format_argument_type(current_argument->child, kind);
        int suppress_cascading_error = kind == INPUT_FORMAT_ARGUMENTS && argument_type == NULL_VALUE;
        if (!suppress_cascading_error && !is_compatible_with_format_specifier(argument_type, *specifier))
            report_argument_type_mismatch(i + 1, argument_type, *specifier, kind);

        format_cursor = specifier + 1;
        current_argument = current_argument->sibling;
    }
}
