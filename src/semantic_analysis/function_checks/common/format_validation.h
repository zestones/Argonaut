/**
 * @file format_validation.h
 * @brief Shared validation for predefined procedures that use format strings.
 */

#ifndef ARGONAUT_FORMAT_VALIDATION_H
#define ARGONAUT_FORMAT_VALIDATION_H

#include "../../../ast/ast.h"

/**
 * @brief Selects the procedure-specific rules applied to formatted arguments.
 */
typedef enum { PRINT_FORMAT_ARGUMENTS, INPUT_FORMAT_ARGUMENTS } FormatArgumentKind;

/**
 * @brief Validates argument count and types against a format string.
 *
 * Input arguments additionally have to designate declared writable entities.
 *
 * @param format_string The format-string AST node.
 * @param argument_list The argument-list AST node.
 * @param kind Whether the arguments belong to print or input.
 */
void validate_format_arguments(Node *format_string, Node *argument_list, FormatArgumentKind kind);

#endif // ARGONAUT_FORMAT_VALIDATION_H
