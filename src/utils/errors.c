#include "errors.h"

static int error_count = 0;

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
    vsnprintf(result.message, sizeof(result.message), format, args);
    va_end(args);

    return result;
}

void yywarn(const Error diagnostic) {
    fprintf(stderr, COLOR_YELLOW "[");
    switch (diagnostic.type) {
        case SYNTAX_ERROR: fprintf(stderr, "Syntax Warning"); break;
        case SEMANTIC_ERROR: fprintf(stderr, "Semantic Warning"); break;
        case TYPE_ERROR: fprintf(stderr, "Type Warning"); break;
        case LEXICAL_ERROR: fprintf(stderr, "Lexical Warning"); break;
        case RUN_TIME_ERROR: fprintf(stderr, "Run Time Warning"); break;
        default: fprintf(stderr, "Unknown Warning"); break;
    }
    fprintf(stderr, "]");
    fprintf(stderr, " %s" COLOR_RESET "\n", diagnostic.message);
}

void yerror(const Error diagnostic) {
    error_count++;
    fprintf(stderr, COLOR_RED "[");
    switch (diagnostic.type) {
        case SYNTAX_ERROR: fprintf(stderr, "Syntax Error"); break;
        case SEMANTIC_ERROR: fprintf(stderr, "Semantic Error"); break;
        case TYPE_ERROR: fprintf(stderr, "Type Error"); break;
        case LEXICAL_ERROR: fprintf(stderr, "Lexical Error"); break;
        case RUN_TIME_ERROR: fprintf(stderr, "Run Time Error"); break;
        default: fprintf(stderr, "Unknown Error"); break;
    }

    fprintf(stderr, "]");
    fprintf(stderr, " %s" COLOR_RESET "\n", diagnostic.message);
}

char *get_formatted_location(void) {
    static char location[256];
    snprintf(location, sizeof(location), "%s:%d:%d", error.filename, error.line, error.column);
    return location;
}

void set_error_message(Error *target, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(target->message, sizeof(target->message), format, args);
    va_end(args);
}

void set_error_type(Error *target, ErrorType type) {
    target->type = type;
}
