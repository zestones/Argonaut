#include "scope_tracker.h"

static int scope_identifier_list[MAX_DECLARATION_COUNT];
static int scope_identifier_count;

void initialize_scope_identifier_tracker() {
    scope_identifier_count = 0;
    memset(scope_identifier_list, NULL_VALUE, sizeof(scope_identifier_list));
}

void add_identifier_to_scope_tracker(int index_lexeme_lexicographic) {
    scope_identifier_list[scope_identifier_count++] = index_lexeme_lexicographic;
}

int is_identifier_already_tracked(int index_lexeme_lexicographic) {
    for (int i = 0; i < scope_identifier_count; i++) {
        if (scope_identifier_list[i] == index_lexeme_lexicographic) {
            return 1;
        }
    }
    return 0;
}

void clear_scope_identifier_tracker() {
    scope_identifier_count = 0;
    memset(scope_identifier_list, NULL_VALUE, sizeof(scope_identifier_list));
}
