#ifndef ARGONAUT_COMPILER_H
#define ARGONAUT_COMPILER_H

#include <stdio.h>

/**
 * @brief Compiles an Argonaut source stream.
 *
 * This function initializes the compiler state, parses the source, and performs the semantic checks driven by the current frontend.
 *
 * @param source The source stream to compile.
 * @param source_name The source name retained by diagnostics.
 * @return The number of errors reported during compilation.
 */
int compile_source(FILE *source, const char *source_name);

/**
 * @brief Writes the compiled transitional text IR to a caller-owned stream.
 *
 * @param out The output stream that receives the compiled IR.
 */
void write_compiled_ir(FILE *out);

/**
 * @brief Prints the current compiler tables and AST for diagnostics.
 *
 * @param out The output stream that receives the compiler state.
 */
void fprintf_compiler_state(FILE *out);

#endif // ARGONAUT_COMPILER_H
