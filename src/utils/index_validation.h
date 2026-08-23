/**
 * @file index_validation.h
 * @brief Bounds validation shared by the fixed-size Argonaut tables.
 */

#ifndef ARGONAUT_INDEX_VALIDATION_H
#define ARGONAUT_INDEX_VALIDATION_H

/**
 * @brief Terminates when an index is outside a fixed-size table.
 *
 * @param table_name The table name used in the diagnostic.
 * @param index The index to validate.
 * @param capacity The number of valid table slots.
 */
void validate_table_index(const char *table_name, int index, int capacity);

#endif // ARGONAUT_INDEX_VALIDATION_H
