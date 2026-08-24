#ifndef ARGONAUT_CLI_H
#define ARGONAUT_CLI_H

#include <stdio.h>

/**
 * @enum CliProgram
 * @brief Identifies the command-line program whose options are being parsed.
 */
typedef enum { CLI_COMPILER, CLI_VM } CliProgram;

/**
 * @struct ArgonautOptions
 * @brief Stores the command-line options shared by argoc and argov.
 */
typedef struct {
    char *input_file;  /**< Source or IR input path. */
    char *output_file; /**< Compiler IR output path, or NULL. */
    int verbose;       /**< Whether verbose state output is enabled. */
    int log;           /**< Whether compiler logging is enabled. */
} ArgonautOptions;

/**
 * @enum CommandLineStatus
 * @brief Describes whether command-line parsing should continue, exit successfully, or fail.
 */
typedef enum { COMMAND_LINE_READY, COMMAND_LINE_HELP, COMMAND_LINE_ERROR } CommandLineStatus;

/**
 * @brief Prints the usage text of one Argonaut command-line program.
 *
 * @param program_name The executable name displayed in the usage text.
 */
typedef void (*UsagePrinter)(const char *program_name);

/**
 * @brief Opens a file for a command-line operation and reports a fatal error on failure.
 *
 * @param filename The path of the file to open.
 * @param mode The mode passed to fopen.
 * @return The opened file stream.
 */
FILE *open_argonaut_file(const char *filename, const char *mode);

/**
 * @brief Parses the options accepted by argoc or argov.
 *
 * @param argc The process argument count.
 * @param argv The process argument values.
 * @param program The command-line program whose option set is accepted.
 * @param print_usage The program-specific usage printer.
 * @param options The destination for the parsed options.
 * @return The status that determines whether the caller continues or exits.
 */
CommandLineStatus parse_command_line(int argc, char **argv, CliProgram program, UsagePrinter print_usage,
                                     ArgonautOptions *options);

#endif // ARGONAUT_CLI_H
