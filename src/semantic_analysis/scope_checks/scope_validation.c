#include "../../utils/scope_tracker.h"
#include "../../lexer/lexeme_table.h"

void check_scope_redefinition(int index_lexeme_lexicographic, char *entity_type) {
    if (is_identifier_already_tracked(index_lexeme_lexicographic)) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Identifier '%s' is already defined in the current scope as a %s.", get_lexeme(index_lexeme_lexicographic), entity_type);

        yerror(error);
    }
}
