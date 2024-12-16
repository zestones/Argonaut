/**
 * @file table_printer.h
 * @brief Header file for printing tables.
 * 
 * This file contains the function declarations for printing tables.
 * It provides functions to print a table title, separator, header, and row.
 */

#ifndef TABLE_PRINTER_H
#define TABLE_PRINTER_H


#include "colors.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


/**
 * @brief Prints a table title.
 * 
 * @param title The title to print.
 */
static inline void print_table_title(FILE *out, const char* title) {
    int title_length = strlen(title);
    int total_length = title_length + 4;

    fprintf(out, "\n+");
    for (int i = 0; i < total_length; i++) {
        fprintf(out, "-");
    }

    fprintf(out, "+\n");
    fprintf(out, "|" " %-*s " "|\n", total_length - 2, title);
}

/**
 * @brief Prints a table separator.
 * 
 * @param num_columns The number of columns in the table.
 * @param ... The width of each column.
 */
static inline void print_table_separator(FILE *out, int num_columns, ...) {
    va_list args;
    va_start(args, num_columns);

    fprintf(out, "+");
    for (int i = 0; i < num_columns; i++) {
        int col_width = va_arg(args, int);
        for (int j = 0; j < col_width + 2; j++) {
            fprintf(out, "-");
        }

        fprintf(out, "+");
    }

    fprintf(out, "\n");
    va_end(args);
}

/**
 * @brief Prints a table header.
 * 
 * @param num_columns The number of columns in the table.
 * @param ... The width and name of each column.
 */
static inline void print_table_header(FILE *out, int num_columns, ...) {
    va_list args;
    va_start(args, num_columns);

    fprintf(out, "|");
    for (int i = 0; i < num_columns; i++) {
        int col_width = va_arg(args, int);
        const char* col_name = va_arg(args, const char*);
        fprintf(out, " " "%-*s" " |", col_width, col_name);
    }

    fprintf(out, "\n");
    va_end(args);
}

/**
 * @brief Prints a table row.
 * 
 * @param num_columns The number of columns in the table.
 * @param ... The width and value of each column.
 */
static inline void print_table_row(FILE *out, int num_columns, ...) {
    va_list args;
    va_start(args, num_columns);

    fprintf(out, "|");
    for (int i = 0; i < num_columns; i++) {
        int col_width = va_arg(args, int);
        const char* col_value = va_arg(args, const char*);
        fprintf(out, " %-*s |", col_width, col_value);
    }

    fprintf(out, "\n");
    va_end(args);
}


#endif // TABLE_PRINTER_H
