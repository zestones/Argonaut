
#ifndef __FORMAT_SPECIFIERS_H__
#define __FORMAT_SPECIFIERS_H__

#include <string.h>
#include "../../../ast/ast.h"


char *strip_quotes(const char *raw_format);

char get_nth_format_specifier(const char *format, int n);

int count_format_specifiers(const char *format);

int is_compatible_with_format_specifier(int argument_type, char specifier);

/**
 * @brief Tells whether a character is a supported conversion specifier (d, f, s, c, b).
 */
int is_format_specifier(char c);

int count_argument_list(Node *argument_list);


#endif // __FORMAT_SPECIFIERS_H__