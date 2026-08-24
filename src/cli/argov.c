#include "cli.h"

#include "../../lib/colors.h"
#include "../virtual_machine/vm.h"

#include <stdlib.h>

static void print_usage(const char *program_name) {
    fprintf(stdout, COLOR_BOLD_YELLOW "\nUsage:\n" COLOR_RESET);
    fprintf(stdout, COLOR_GREEN "  %s -a <input_file> [options]\n\n", program_name);

    fprintf(stdout, COLOR_BOLD_YELLOW "Description:\n" COLOR_RESET);
    fprintf(stdout, COLOR_CYAN "  This program interprets the specified input file and optionally displays\n");
    fprintf(stdout, "  additional information such as the symbol tables and abstract syntax tree (AST).\n");
    fprintf(stdout,
            "  Use this tool to execute Argonaut code and analyze its internal representation.\n\n" COLOR_RESET);

    fprintf(stdout, COLOR_BOLD_YELLOW "Arguments:\n" COLOR_RESET);
    fprintf(stdout, COLOR_GREEN "  -a <file>          " COLOR_RESET ": The input file to be interpreted (required).\n");
    fprintf(stdout, COLOR_GREEN "  -v, --verbose      " COLOR_RESET
                                ": Enable verbose mode (display symbol tables, AST and execution Stack).\n");
    fprintf(stdout, COLOR_GREEN "  -h, --help         " COLOR_RESET ": Show this help message and exit.\n");

    fprintf(stdout, "\n" COLOR_BOLD_YELLOW "Examples:\n" COLOR_RESET);
    fprintf(stdout, COLOR_CYAN "  %s -a my_program.arg -v\n" COLOR_RESET, program_name);
    fprintf(stdout,
            "    - Interprets 'my_program.arg' with verbose output (symbol tables, AST, execution Stack, etc.).\n\n");

    fprintf(stdout, COLOR_CYAN "  %s -a my_program.arg --verbose\n" COLOR_RESET, program_name);
    fprintf(stdout, "    - Long option equivalent of the above example.\n\n");

    fprintf(stdout, COLOR_BOLD_YELLOW "Constraints:\n" COLOR_RESET);
    fprintf(stdout, COLOR_RED "  - The input file must be specified with the '-a <file>' option.\n");
    fprintf(stdout, COLOR_RED "  - If no file is specified, the program will terminate with an error.\n\n");
}

static void print_verbose_state(int enabled) {
    if (!enabled) return;
    fprintf(stdout, COLOR_GREEN "Verbose mode enabled. Printing tables and ast...\n" COLOR_RESET);
    fprintf_vm_state(stdout);
}

int main(int argc, char **argv) {
    ArgonautOptions options;
    CommandLineStatus status = parse_command_line(argc, argv, CLI_VM, print_usage, &options);
    if (status != COMMAND_LINE_READY) return status == COMMAND_LINE_HELP ? EXIT_SUCCESS : EXIT_FAILURE;

    FILE *input = open_argonaut_file(options.input_file, "r");
    execute_ir(input, options.input_file);
    fclose(input);

    print_verbose_state(options.verbose);
    return EXIT_SUCCESS;
}
