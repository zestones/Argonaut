#include "errors.h"

static int error_count = 0;

static const char *diagnostic_type_name(ErrorType type) {
    switch (type) {
        case SYNTAX_ERROR: return "Syntax";
        case SEMANTIC_ERROR: return "Semantic";
        case TYPE_ERROR: return "Type";
        case LEXICAL_ERROR: return "Lexical";
        case RUN_TIME_ERROR: return "Run Time";
        default: return "Unknown";
    }
}

static void format_error_message(Error *target, const char *format, va_list args) ARGO_PRINTF_LIKE(2, 0);

static void format_error_message(Error *target, const char *format, va_list args) {
    vsnprintf(target->message, sizeof(target->message), format, args);
}

static void print_diagnostic(const Error diagnostic, const char *color, const char *severity) {
    fprintf(stderr, "%s[%s %s] %s" COLOR_RESET "\n", color, diagnostic_type_name(diagnostic.type), severity,
            diagnostic.message);
}

static void exit_with_current_error(void) ARGO_NORETURN;

static void exit_with_current_error(void) {
    yerror(error);
    exit(EXIT_FAILURE);
}

int get_error_count(void) {
    return error_count;
}

Error construct_error(ErrorType type, int line, int column, const char *format, ...) {
    Error result;

    result.type = type;
    result.line = line;
    result.column = column;

    va_list args;
    va_start(args, format);
    format_error_message(&result, format, args);
    va_end(args);

    return result;
}

void yywarn(const Error diagnostic) {
    print_diagnostic(diagnostic, COLOR_YELLOW, "Warning");
}

void yerror(const Error diagnostic) {
    error_count++;
    print_diagnostic(diagnostic, COLOR_RED, "Error");
}

char *get_formatted_location(void) {
    static char location[256];
    snprintf(location, sizeof(location), "%s:%d:%d", error.filename, error.line, error.column);
    return location;
}

void set_error_message(Error *target, const char *format, ...) {
    va_list args;
    va_start(args, format);
    format_error_message(target, format, args);
    va_end(args);
}

void set_error_type(Error *target, ErrorType type) {
    target->type = type;
}

void fatal_runtime_error(const char *format, ...) {
    error.type = RUN_TIME_ERROR;

    va_list args;
    va_start(args, format);
    format_error_message(&error, format, args);
    va_end(args);

    exit_with_current_error();
}

void fatal_syntax_error(const char *token, const char *detail) {
    error.type = SYNTAX_ERROR;
    if (detail == NULL) {
        set_error_message(&error,
                          "Unexpected token found: '%s' at %s.\n"
                          "\t> This error is critical and will cause the program to terminate.\n"
                          "\t> Exiting due to a syntax error.",
                          token, get_formatted_location());
    } else {
        set_error_message(&error,
                          "Unexpected token found: '%s' at %s.\n"
                          "\t> %s\n"
                          "\t> This error is critical and will cause the program to terminate.\n"
                          "\t> Exiting due to a syntax error.",
                          token, get_formatted_location(), detail);
    }
    exit_with_current_error();
}
