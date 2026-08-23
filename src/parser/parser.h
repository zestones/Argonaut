#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <string.h>

#include "../ast/ast.h"

#include "../../lib/colors.h"
#include "../utils/utils.h"

#include "../symbol_table/lexeme/lexeme_table.h"

#include "../symbol_table/declaration/declaration_table.h"
#include "../symbol_table/representation/representation_table.h"
#include "../symbol_table/hash/hash_table.h"

#include "../virtual_machine/interpreter/interpreter.h"
#include "../data/region_table.h"

/**
 * @enum Mode
 * @brief Enumeration of the different modes.
 */
typedef enum { COMPILATION, INTERPRETATION } Mode;

/**
 * @brief Command-line options shared by argoc and argov.
 */
typedef struct {
    char *input_file;  // Source or IR input path.
    char *output_file; // Compiler IR output path, or NULL.
    int verbose;       // Whether verbose tables are printed.
    int log;           // Whether compiler logging is enabled.
} ArgonautOptions;

/**
 * @brief Result of parsing command-line options.
 */
typedef enum { COMMAND_LINE_READY, COMMAND_LINE_HELP, COMMAND_LINE_ERROR } CommandLineStatus;

/**
 * @brief Binary-specific usage printer passed to the shared option parser.
 */
typedef void (*UsagePrinter)(const char *program_name);

/**
 * @brief Logs internal tables to a file if logging is enabled.
 * 
 * When logging is activated, this function writes the contents of various internal tables
 * to a file named `log.txt`. It helps with debugging or reviewing the internal state of the
 * interpreter/compiler after a run.
 * 
 * @param log A flag to indicate whether logging is enabled (non-zero) or disabled (zero).
 */
void ylog(int log);

/**
 * @brief Prints internal tables and the AST to standard output in verbose mode.
 * 
 * When debugging is enabled, this function prints the contents of key data tables and the 
 * Abstract Syntax Tree (AST) to the console. It is useful for verbose debugging during development.
 * 
 * @param debug A flag to indicate whether verbose mode is enabled (non-zero) or disabled (zero).
 */
void ydebug(int debug);

/**
 * @brief The main function to initialize, parse, and optionally interpret the program.
 * 
 * This function sets up the required tables, parses the input program, and executes it
 * if the mode is set to interpretation. It handles the entire lifecycle of the program 
 * from parsing to execution.
 * 
 * @param m The mode of operation (e.g., COMPILATION or INTERPRETATION).
 * 
 * @return The result of the parsing operation (usually zero for success, non-zero for errors).
 */
int yyrun(Mode m);

/**
 * @brief Opens an Argonaut source or IR file and reports a consistent fatal error on failure.
 *
 * @param filename The file to open.
 * @param mode The fopen mode.
 * @return The opened stream.
 */
FILE *open_argonaut_file(const char *filename, const char *mode);

/**
 * @brief Resets the global diagnostic location for a new input file.
 *
 * @param filename The input filename retained by the diagnostic state.
 */
void initialize_diagnostic_state(const char *filename);

/**
 * @brief Parses the command-line options shared by argoc and argov.
 *
 * Compilation mode additionally accepts logging and output-file options.
 *
 * @param argc The process argument count.
 * @param argv The process arguments.
 * @param mode Whether argoc or argov options are expected.
 * @param print_usage The binary-specific usage printer.
 * @param options The parsed options.
 * @return Whether execution should continue, exit successfully, or fail.
 */
CommandLineStatus parse_command_line(int argc, char **argv, Mode mode, UsagePrinter print_usage,
                                     ArgonautOptions *options);

#endif // __PARSER_H__
