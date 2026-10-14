/**
 * @file numeric_parser.h
 * @brief Strict parsers for Argonaut's numeric value ranges.
 */

#ifndef ARGONAUT_NUMERIC_PARSER_H
#define ARGONAUT_NUMERIC_PARSER_H

/**
 * @brief Parses a complete string as an Argonaut integer.
 *
 * @param text The text to parse.
 * @param value The optional destination for the parsed value; it is left unchanged on failure.
 * @return 1 when the complete text is a representable integer, 0 otherwise.
 */
int parse_integer_text(const char *text, int *value);

/**
 * @brief Parses a complete string as an Argonaut real value.
 *
 * Argonaut stores real values as `float`, so finite values outside that range are rejected even though parsing uses `double`.
 *
 * @param text The text to parse.
 * @param value The optional destination for the parsed value; it is left unchanged on failure.
 * @return 1 when the complete text is a representable real value, 0 otherwise.
 */
int parse_real_text(const char *text, double *value);

#endif // ARGONAUT_NUMERIC_PARSER_H
