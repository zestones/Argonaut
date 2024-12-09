#ifndef __SCOPE_VALIDATION_H__
#define __SCOPE_VALIDATION_H__

#include "../validation_utils.h"

/**
 * @brief Checks if a scope entity has been redefined.
 *
 * This function checks if a scope entity has been redefined in the current scope.
 * If the scope entity has been redefined, a warning is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the scope entity's name.
 * @param entity_type The type of the scope entity. (function/procedure parameter, structure field)
 */
void check_scope_redefinition(int index_lexeme_lexicographic, char *entity_type);

#endif // __SCOPE_VALIDATION_H__