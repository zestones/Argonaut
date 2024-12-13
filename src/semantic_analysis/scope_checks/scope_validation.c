#include "../../utils/scope_tracker.h"
#include "../../lexer/lexeme_table.h"

void check_scope_redefinition(int index_lexeme_lexicographic, char *entity_type) {
    if (is_identifier_already_tracked(index_lexeme_lexicographic)) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(
            &error,
            "Redefinition error at %s.\n"
            "  The identifier '%s' is already defined in the current scope as a %s.\n"
            "  Consider renaming the identifier or altering the existing definition to resolve the conflict.\n",
            get_formatted_location(),
            get_lexeme(index_lexeme_lexicographic),
            entity_type
        );
        yerror(error);
    }
}
