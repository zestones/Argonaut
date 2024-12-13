#include "errors.h"

Error construct_error(ErrorType type, int line, int column, const char *format, ...) {
    Error error;

    error.line = line;
    error.column = column;

    va_list args;
    va_start(args, format);
    vsnprintf(error.message, sizeof(error.message), format, args);
    va_end(args);

    return error;
}

void yywarn(const Error error) {
    printf(COLOR_YELLOW "[");
    switch (error.type) {
        case SYNTAX_ERROR:        printf("Syntax Warning");           break;
        case SEMANTIC_ERROR:      printf("Semantic Warning");         break;
        case TYPE_ERROR:          printf("Type Warning");             break;
        case LEXICAL_ERROR:       printf("Lexical Warning");          break;
        case INVALID_INPUT_ERROR: printf("Invalid Input Warning");    break;
        default:                  printf("Unknown Warning");          break;
    }
    printf("]");
    printf(" %s" COLOR_RESET "\n", error.message);
}

void yerror(const Error error) {
    fprintf(stderr, COLOR_RED "[");
    switch (error.type) {
        case SYNTAX_ERROR:        fprintf(stderr, "Syntax Error");         break;
        case SEMANTIC_ERROR:      fprintf(stderr, "Semantic Error");       break;
        case TYPE_ERROR:          fprintf(stderr, "Type Error");           break;
        case LEXICAL_ERROR:       fprintf(stderr, "Lexical Error");        break;
        case INVALID_INPUT_ERROR: fprintf(stderr, "Invalid Input Error");  break;
        default:                  fprintf(stderr, "Unknown Error");        break;
    }

    fprintf(stderr, "]");
    fprintf(stderr, " %s" COLOR_RESET "\n", error.message);
    // exit(EXIT_FAILURE);  // Uncomment this line to exit the program on error
}

char* get_formatted_location() {
    static char location[256];
    snprintf(location, sizeof(location), "%s:%d:%d", error.filename, error.line, error.column);
    return location;
}

void set_error_message(Error *error, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(error->message, sizeof(error->message), format, args);
    va_end(args);
}

void set_error_type(Error *error, ErrorType type) {
    error->type = type;
}