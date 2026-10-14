#ifndef ARGONAUT_TEXT_IR_H
#define ARGONAUT_TEXT_IR_H

#include <stdio.h>

/**
 * @brief Initializes the tables backing the transitional text IR.
 *
 * This function prepares empty lexeme, hash, declaration, representation, and region tables for compilation or IR loading.
 */
void initialize_text_ir(void);

/**
 * @brief Writes the complete versioned text IR to a caller-owned stream.
 *
 * @param out The output stream that receives the IR header and serialized tables.
 */
void write_text_ir(FILE *out);

/**
 * @brief Prints the internal tables and AST in their human-readable debug form.
 *
 * @param out The output stream that receives the diagnostic representation.
 */
void fprintf_text_ir_state(FILE *out);

#endif // ARGONAUT_TEXT_IR_H
