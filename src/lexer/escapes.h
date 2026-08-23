#ifndef __ESCAPES_H__
#define __ESCAPES_H__

#include <stddef.h>

/**
 * @brief Decodes the C escape sequences of a literal: \n \t \r \\ \' \".
 *
 * @param text The characters between the quotes of the literal (not NUL-terminated).
 * @param length The number of characters to decode.
 * @param bad On failure, receives the position of the unknown escape sequence in `text`.
 * @return A malloc'ed decoded string, or NULL if an unknown escape sequence was found.
 */
char *decode_escapes(const char *text, size_t length, const char **bad);

/**
 * @brief Escapes a decoded text so that it fits on one line: \ -> \\, LF -> \n, CR -> \r, TAB -> \t.
 *
 * Used by the IR export and by the table dumps.
 *
 * @param text The text to escape (NUL-terminated).
 * @return A malloc'ed escaped string.
 */
char *escape_text(const char *text);

/**
 * @brief Inverse of escape_text(), used when loading an IR file.
 *
 * @param text The escaped text (NUL-terminated).
 * @return A malloc'ed decoded string.
 */
char *unescape_text(const char *text);

#endif // __ESCAPES_H__
