#ifndef TABLE_PRINTER_H
#define TABLE_PRINTER_H

#include "colors.h"
#include <stdarg.h>
#include <stdio.h>


static inline void print_table_separator(int num_columns, ...) {
    va_list args;
    va_start(args, num_columns);

    fprintf(stdout, "+");
    for (int i = 0; i < num_columns; i++) {
        int col_width = va_arg(args, int);
        for (int j = 0; j < col_width + 2; j++) {
            fprintf(stdout, "-");
        }

        fprintf(stdout, "+");
    }

    fprintf(stdout, "\n");
    va_end(args);
}

static inline void print_table_header(int num_columns, ...) {
    va_list args;
    va_start(args, num_columns);

    fprintf(stdout, "|");
    for (int i = 0; i < num_columns; i++) {
        int col_width = va_arg(args, int);
        const char* col_name = va_arg(args, const char*);
        fprintf(stdout, " " COLOR_BOLD "%-*s" COLOR_RESET " |", col_width, col_name);
    }

    fprintf(stdout, "\n");
    va_end(args);
}

static inline void print_table_row(int num_columns, ...) {
    va_list args;
    va_start(args, num_columns);

    fprintf(stdout, "|");
    for (int i = 0; i < num_columns; i++) {
        int col_width = va_arg(args, int);
        const char* col_value = va_arg(args, const char*);
        fprintf(stdout, " %-*s |", col_width, col_value);
    }

    fprintf(stdout, "\n");
    va_end(args);
}


#endif // TABLE_PRINTER_H
