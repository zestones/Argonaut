#ifndef __VM_CELL_H__
#define __VM_CELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/utils.h"

/**
 * @enum vm_type
 * @brief Enumeration representing different types of values that can be stored in a `vm_cell`.
 *
 * This enum is used to distinguish between the various types that a `vm_cell` can hold, such as integers,
 * floating-point numbers, booleans, characters, and strings.
 */
typedef enum vm_type { INTEGER, REAL, BOOLEAN, CHARACTER, STRING } vm_type;

/**
 * @struct vm_cell
 * @brief Struct representing a cell in the virtual machine.
 *
 * A `vm_cell` holds a value of a specific type (as indicated by the `type` field) and includes the value
 * itself stored in a union. Additionally, it tracks whether the value has been initialized or not.
 * 
 * The `value` field is a union that can hold different types of data (integer, real, boolean, character, or string).
 * The `type` field indicates which data type is currently stored, and the `is_initialized` field marks whether
 * the cell contains a valid value.
 */
typedef struct {
    union {
        int integer;    // Integer value (stores `int`)
        float real;     // Real value (stores `float`)
        int boolean;    // Boolean value (stores `int`, 0 or 1)
        char character; // Character value (stores `char`) 
        char *string;   // String value (stores `char *`)
    } value;            // Union holding the value of the cell

    vm_type type;        // The type of value stored in the cell
    int is_initialized;  // Flag indicating whether the cell is initialized (1 = initialized, 0 = not)
} vm_cell;

/**
 * @brief Constructs a `vm_cell` with a specific type and value.
 *
 * Allocates and initializes a `vm_cell` based on the provided type and value.
 * If the value is `NULL`, the cell is marked as uninitialized.
 *
 * @param type The type of the cell (e.g., INTEGER, REAL, etc.).
 * @param value A pointer to the value to initialize the cell. Can be `NULL`.
 * @return A constructed `vm_cell` instance.
 * @note For `STRING` type, the value is duplicated internally.
 */
vm_cell construct_vm_cell(int type, void *value);

/**
 * @brief Retrieves the value from a `vm_cell` as a pointer.
 *
 * This function provides a generic pointer to the value stored in the given
 * `vm_cell`. The caller is responsible for interpreting the type.
 *
 * @param cell The `vm_cell` to extract the value from.
 * @return A pointer to the value inside the `vm_cell`.
 * @note The returned pointer must not be freed.
 */
void *get_vm_cell_value(vm_cell cell);

/**
 * @brief Updates an existing `vm_cell` with a new value.
 *
 * Modifies the contents of an existing `vm_cell` by assigning it a new value
 * while preserving its type. Marks the cell as initialized.
 *
 * @param cell A pointer to the `vm_cell` to update.
 * @param value A pointer to the new value to assign.
 * @note For `STRING` type, the value is duplicated internally.
 */
void update_vm_cell(vm_cell *cell, void *value);

/**
 * @brief Formats a `vm_cell` as a human-readable string.
 *
 * Converts the contents of a `vm_cell` to a formatted string representation
 * for display purposes. Handles both initialized and uninitialized cells.
 *
 * @param data A pointer to the `vm_cell` to format.
 * @return A constant string representation of the cell.
 * @note The returned string is statically allocated and must not be freed.
 */
const char *format_cell(void *data);

/**
 * @brief Prints a `vm_cell` to a file stream in a tabular format.
 *
 * Outputs the type and value of the `vm_cell` to the specified file stream,
 * formatted as a table with appropriate column headers and separators.
 *
 * @param out The file stream to write to.
 * @param cell The `vm_cell` to print.
 */
void fprintf_vm_cell(FILE *out, vm_cell cell);

#endif // __VM_CELL_H__