#include "cli.h"

#include "../../lib/colors.h"

#include <getopt.h>
#include <stdlib.h>

FILE *open_argonaut_file(const char *filename, const char *mode) {
    FILE *file = fopen(filename, mode);
    if (file != NULL) return file;

    fprintf(stderr, COLOR_RED "Error: Could not open file '%s'.\n", filename);
    perror("\t> Reason");
    fprintf(stderr, COLOR_RESET);
    exit(EXIT_FAILURE);
}

CommandLineStatus parse_command_line(int argc, char **argv, CliProgram program, UsagePrinter print_usage,
                                     ArgonautOptions *options) {
    static const struct option compiler_options[] = {{"help", no_argument, NULL, 'h'},
                                                     {"verbose", no_argument, NULL, 'v'},
                                                     {"log", no_argument, NULL, 'l'},
                                                     {"output", required_argument, NULL, 'o'},
                                                     {NULL, 0, NULL, 0}};
    static const struct option vm_options[] = {
        {"help", no_argument, NULL, 'h'}, {"verbose", no_argument, NULL, 'v'}, {NULL, 0, NULL, 0}};

    *options = (ArgonautOptions) {NULL, NULL, 0, 0};
    const char *short_options = program == CLI_COMPILER ? "a:vlo:h" : "a:vh";
    const struct option *long_options = program == CLI_COMPILER ? compiler_options : vm_options;

    int option;
    while ((option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (option) {
            case 'a': options->input_file = optarg; break;
            case 'v': options->verbose = 1; break;
            case 'l': options->log = 1; break;
            case 'o': options->output_file = optarg; break;
            case 'h': print_usage(argv[0]); return COMMAND_LINE_HELP;
            default:
                fprintf(stderr, COLOR_RED "Error: Unknown option '-%c'.\n" COLOR_RESET, optopt);
                print_usage(argv[0]);
                return COMMAND_LINE_ERROR;
        }
    }

    if (options->input_file != NULL) return COMMAND_LINE_READY;

    if (program == CLI_COMPILER)
        fprintf(stderr, COLOR_RED "Error: Missing required '-a <file>' argument.\n" COLOR_RESET);
    else fprintf(stderr, COLOR_RED "Error: No input file specified.\n" COLOR_RESET);
    print_usage(argv[0]);
    return COMMAND_LINE_ERROR;
}
