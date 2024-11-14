#ifndef __SCOPE_TRACKER_H__
#define __SCOPE_TRACKER_H__

#include "utils.h"
#include "../symbol_table/declaration_table.h"

void initialize_scope_identifier_tracker();

void add_identifier_to_scope_tracker(int index_lexeme_lexicographic);

int is_identifier_already_tracked(int index_lexeme_lexicographic);

void check_scope_redefinition(int index_lexeme_lexicographic, char *entity_type);

void clear_scope_identifier_tracker();

#endif // __SCOPE_TRACKER_H__
