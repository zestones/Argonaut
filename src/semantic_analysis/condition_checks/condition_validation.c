#include "../../type_system/type_inference/type_inference.h"
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"
#include "../semantic_checks.h"

void check_condition(Node *condition) {
    // Step 1: Resolve the type of the condition
    int condition_type = resolve_condition_type(condition);

    // Step 2: Check if the condition is a valid boolean expression
    if (condition_type != A_BOOLEAN_LITERAL) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, 
            "Invalid condition at %s.\n"
            "  Expected a boolean expression, but received '%s'.\n"
            "  Ensure the expression evaluates to a boolean value.\n",
            get_formatted_location(),
            (condition_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(condition_type)
        );
        yerror(error);
        return;
    }
}