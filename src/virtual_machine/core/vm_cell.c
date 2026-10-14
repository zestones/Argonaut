#include "../../../lib/table_printer.h"
#include "../../../lib/colors.h"

#include "vm_cell.h"

vm_cell construct_vm_cell(int type, void *value) {
    vm_cell cell;
    memset(&cell, 0, sizeof(cell));
    cell.type = type;
    cell.is_initialized = (value != NULL);

    if (cell.is_initialized) {
        switch (type) {
            case INTEGER: cell.value.integer = *((int *) value); break;

            case REAL: cell.value.real = *((double *) value); break;

            case BOOLEAN: cell.value.boolean = *((int *) value); break;

            case CHARACTER: cell.value.character = *((char *) value); break;

            case STRING: cell.value.string = (char *) value; break;

            default: fprintf(stderr, "construct_vm_cell Invalid type\n"); exit(EXIT_FAILURE);
        }
    }

    return cell;
}

void *get_vm_cell_value(vm_cell *cell) {
    switch (cell->type) {
        case INTEGER: return &cell->value.integer;
        case REAL: return &cell->value.real;
        case BOOLEAN: return &cell->value.boolean;
        case CHARACTER: return &cell->value.character;
        case STRING: return cell->value.string;
        default: fprintf(stderr, "get_vm_cell_value Invalid type\n"); exit(EXIT_FAILURE);
    }
}

void update_vm_cell(vm_cell *cell, void *value) {
    cell->is_initialized = 1;

    switch (cell->type) {
        case INTEGER: cell->value.integer = *((int *) value); break;
        case REAL: cell->value.real = *((float *) value); break;
        case BOOLEAN: cell->value.boolean = *((int *) value); break;
        case CHARACTER: cell->value.character = *((char *) value); break;
        case STRING: cell->value.string = (char *) value; break;

        default: fprintf(stderr, "update_vm_cell Invalid type\n"); exit(EXIT_FAILURE);
    }
}

const char *get_cell_type_string(vm_cell cell) {
    return cell.type == INTEGER     ? "int"
           : cell.type == REAL      ? "float"
           : cell.type == BOOLEAN   ? "bool"
           : cell.type == CHARACTER ? "char"
                                    : "unknown";
}

const char *format_cell(void *data) {
    vm_cell *cell = (vm_cell *) data;
    static char buffer[64];

    if (!cell->is_initialized) {
        return get_cell_type_string(*cell);
    }

    switch (cell->type) {
        case INTEGER: snprintf(buffer, sizeof(buffer), "%d (int)", cell->value.integer); break;
        case REAL: snprintf(buffer, sizeof(buffer), "%.2f (float)", cell->value.real); break;
        case BOOLEAN: snprintf(buffer, sizeof(buffer), "%s (bool)", cell->value.boolean ? "true" : "false"); break;
        case CHARACTER: snprintf(buffer, sizeof(buffer), "'%c' (char)", cell->value.character); break;
        case STRING: snprintf(buffer, sizeof(buffer), "%s (string)", cell->value.string); break;

        default: snprintf(buffer, sizeof(buffer), "Unknown Type"); break;
    }

    return buffer;
}

void fprintf_vm_cell(FILE *out, vm_cell cell) {
    const int col_width_type = 10;
    const int col_width_value = 10;

    print_table_separator(out, 2, col_width_type, col_width_value);
    print_table_header(out, 2, col_width_type, "Type", col_width_value, "Value");
    print_table_separator(out, 2, col_width_type, col_width_value);

    // Get the type as a string
    const char *type_str = (cell.type == INTEGER)     ? "INTEGER"
                           : (cell.type == REAL)      ? "REAL"
                           : (cell.type == BOOLEAN)   ? "BOOLEAN"
                           : (cell.type == CHARACTER) ? "CHARACTER"
                           : (cell.type == STRING)    ? "STRING"
                                                      : "UNKNOWN";

    char value_str[64];
    if (cell.type == INTEGER) {
        snprintf(value_str, sizeof(value_str), "%d", cell.value.integer);
    } else if (cell.type == REAL) {
        snprintf(value_str, sizeof(value_str), "%.2f", cell.value.real);
    } else if (cell.type == BOOLEAN) {
        snprintf(value_str, sizeof(value_str), "%s", cell.value.boolean ? "true" : "false");
    } else if (cell.type == CHARACTER) {
        snprintf(value_str, sizeof(value_str), "'%c'", cell.value.character);
    } else if (cell.type == STRING) {
        snprintf(value_str, sizeof(value_str), "%s", cell.value.string);
    } else {
        snprintf(value_str, sizeof(value_str), "UNKNOWN");
    }

    print_table_row(out, 2, col_width_type, type_str, col_width_value, value_str);
    print_table_separator(out, 2, col_width_type, col_width_value);
}
