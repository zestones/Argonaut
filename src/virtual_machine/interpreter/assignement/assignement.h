#ifndef __ASSIGNEMENT_H__
#define __ASSIGNEMENT_H__

#include "../../../data/region_table.h"

/**
 * @brief Resolves an assignment expression and updates the corresponding variable, array, or structure.
 *
 * This function takes in a node representing an assignment statement and processes it. Depending on the type of
 * assignment (variable, array, or structure), it resolves the right-hand side expression and applies the value
 * to the appropriate entity in the execution context (variable, array, or structure).
 * 
 * The function handles:
 * - Variable assignments (using `handle_variable_affectation`)
 * - Array assignments (using `handle_array_affectation`)
 * - Structure field assignments (using `handle_structure_affectation`)
 * 
 * It assumes the provided `assignement_start` node has a valid structure representing an assignment, and it
 * calls the appropriate handler based on the assignment type.
 *
 * @param assignement_start The root node of the assignment statement to resolve.
 *                          The node's child contains the specific assignment type (variable, array, or structure).
 */
void resolve_assignement(Node *assignement_start);

#endif // __ASSIGNEMENT_H__