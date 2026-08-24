#include "cli.h"

#include "../../lib/colors.h"
#include "../compiler/compiler.h"

#include <stdlib.h>

static void print_usage(const char *program_name) {
    fprintf(stderr, COLOR_BOLD_YELLOW "\nUsage:\n" COLOR_RESET);
    fprintf(stderr, "  %s -a <input_file> [options]\n\n", program_name);

    fprintf(stderr, COLOR_BOLD_YELLOW "Description:\n" COLOR_RESET);
    fprintf(stderr, COLOR_CYAN
            "  This program compiles the specified file and offers options for generating logs, verbose output,\n");
    fprintf(stderr, "  and exporting tables to an output file.\n");
    fprintf(stderr, "  Use this program to compile Argonaut code and analyze the intermediate steps of the "
                    "process.\n\n" COLOR_RESET);

    fprintf(stderr, COLOR_BOLD_YELLOW "Arguments:\n" COLOR_RESET);
    fprintf(stderr, COLOR_GREEN "  -a <input_file>   " COLOR_RESET ": The source file to compile (required).\n");
    fprintf(stderr,
            COLOR_GREEN "  -v, --verbose     " COLOR_RESET ": Display verbose output (e.g., symbol table and AST).\n");
    fprintf(stderr, COLOR_GREEN "  -l, --log         " COLOR_RESET ": Enable logging during compilation.\n");
    fprintf(stderr, COLOR_GREEN "  -o <output_file>, --output <output_file> " COLOR_RESET
                                ": Specify the output file to save results.\n");
    fprintf(stderr, COLOR_GREEN "  -h, --help        " COLOR_RESET ": Display this help information.\n");

    fprintf(stderr, "\n");
    fprintf(stderr, COLOR_BOLD_YELLOW "Examples:\n" COLOR_RESET);
    fprintf(stderr, COLOR_CYAN "  %s -a my_program.arg -vl -o output.txt\n" COLOR_RESET, program_name);
    fprintf(stderr, "    - Compile the file 'my_program.arg' with verbose output and logging.\n");
    fprintf(stderr, "    - Results saved to 'output.txt'.\n\n");

    fprintf(stderr, COLOR_CYAN "  %s -a my_program.arg --verbose --log --output output.txt\n" COLOR_RESET,
            program_name);
    fprintf(stderr, "    - Compile the file 'my_program.arg' with long options.\n");
    fprintf(stderr, "    - Results saved to 'output.txt'.\n\n");

    fprintf(stderr, COLOR_BOLD_YELLOW "Constraints:\n" COLOR_RESET);
    fprintf(stderr, COLOR_RED "  - The input file must be specified with the '-a <input_file>' option.\n");
    fprintf(stderr, COLOR_RED
            "  - If '-o <output_file>' is used, the output file will be overwritten if it exists.\n\n" COLOR_RESET);
}

static void print_verbose_state(int enabled) {
    if (!enabled) return;
    fprintf(stdout, COLOR_GREEN "Verbose mode enabled. Printing tables and ast...\n" COLOR_RESET);
    fprintf_compiler_state(stdout);
}

static void write_log(int enabled) {
    if (!enabled) return;

    const char *filename = "log.txt";
    FILE *output = fopen(filename, "w");
    if (output == NULL) {
        fprintf(stderr, "Error opening log file %s\n", filename);
        return;
    }

    fprintf(stdout, COLOR_GREEN "Logging enabled. Exporting tables to %s\n" COLOR_RESET, filename);
    fprintf_compiler_state(output);
    fclose(output);
}

int main(int argc, char **argv) {
    ArgonautOptions options;
    CommandLineStatus status = parse_command_line(argc, argv, CLI_COMPILER, print_usage, &options);
    if (status != COMMAND_LINE_READY) return status == COMMAND_LINE_HELP ? EXIT_SUCCESS : EXIT_FAILURE;

    FILE *input = open_argonaut_file(options.input_file, "r");
    int error_count = compile_source(input, options.input_file);
    fclose(input);

    print_verbose_state(options.verbose);
    write_log(options.log);

    if (error_count > 0) {
        fflush(stdout);
        fprintf(stderr, COLOR_RED "Compilation failed: %d error(s).\n" COLOR_RESET, error_count);
        return EXIT_FAILURE;
    }

    if (options.output_file != NULL) {
        FILE *output = open_argonaut_file(options.output_file, "w");
        write_compiled_ir(output);
        fclose(output);
    }

    return EXIT_SUCCESS;
}
