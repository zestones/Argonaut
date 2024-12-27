#ifndef __SCOPE_TRACKER_H__
#define __SCOPE_TRACKER_H__

#include "utils.h"
#include "../symbol_table/declaration/declaration_table.h"

/**
 * @brief Initializes the scope identifier tracker.
 * 
 * This function resets the scope identifier tracker by setting the identifier count to zero 
 * and clearing the list of tracked identifiers. It prepares the tracker for a new scope, ensuring
 * that any new identifiers are tracked independently of previous scopes.
 * 
 * @note This function is called at the start of a new scope, ensuring that identifiers 
 *       from previous scopes are not erroneously included.
 */
void initialize_scope_identifier_tracker();

/**
 * @brief Adds an identifier to the scope tracker.
 * 
 * This function adds a new identifier to the scope identifier list. It increments the identifier count 
 * and stores the given lexicographic index of the identifier.
 * 
 * @param index_lexeme_lexicographic The lexicographic index of the identifier to be added to the tracker.
 */
void add_identifier_to_scope_tracker(int index_lexeme_lexicographic);

/**
 * @brief Checks if an identifier is already tracked within the current scope.
 * 
 * This function checks whether a given identifier has already been added to the scope's tracker. 
 * It compares the lexicographic index of the identifier against those stored in the tracker.
 * 
 * @param index_lexeme_lexicographic The lexicographic index of the identifier to check.
 * 
 * @return 1 if the identifier is already tracked in the scope, 0 if it is not.
 */
int is_identifier_already_tracked(int index_lexeme_lexicographic);

/**
 * @brief Clears the scope identifier tracker.
 * 
 * This function resets the scope identifier tracker by setting the identifier count to zero 
 * and clearing the list of tracked identifiers. It is typically called after a scope has been 
 * processed, ensuring that the tracker is reset for the next scope.
 */
void clear_scope_identifier_tracker();

#endif // __SCOPE_TRACKER_H__
