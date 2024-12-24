#include "../../lib/table_printer.h"    
#include "../../lib/colors.h"

#include "vm_cell.h"

vm_cell construct_vm_cell(int type, void *value) {
    vm_cell cell;
    cell.type = type;
    cell.is_initialized = (value != NULL);

    if (cell.is_initialized) {
        switch (type) {
            case INTEGER:
                cell.value.integer = *((int *)value);
                break;
            case REAL:
                cell.value.real = *((double *)value);
                break;
            case BOOLEAN:
                cell.value.boolean = *((int *)value);
                break;
            case CHARACTER:
                cell.value.character = *((char *)value);
                break;

            case STRING: 
                cell.value.string = strdup((char *)value);
                break;

            case NULL_VALUE:
                printf("type not recognized !\n");
                break;
            default:
                fprintf(stderr, "construct_vm_cell Invalid type\n");
                exit(EXIT_FAILURE);
        }
    }

    return cell;
}

void *get_vm_cell_value(vm_cell cell) {
    void *cell_value;
    switch (cell.type) {
        case INTEGER: {
            cell_value = &cell.value.integer;
            return cell_value;
        }
        case REAL: {
            cell_value = &cell.value.real;
            return cell_value;
        }
        case BOOLEAN: {
            cell_value = &cell.value.boolean;
            return cell_value;
        }
        case CHARACTER: {
            cell_value = &cell.value.character;
            return cell_value;
        }
        case STRING: {
            cell_value = &cell.value.string;
            return cell_value;
        }
        default:
            fprintf(stderr, "get_vm_cell_value Invalid type\n");
            exit(EXIT_FAILURE);
    }
}

void update_vm_cell(vm_cell *cell, void *value) {
    cell->is_initialized = 1;

    switch (cell->type) {
        case INTEGER:
            cell->value.integer = *((int *)value);
            break;
        case REAL:
            cell->value.real = *((float *)value);
            break;
        case BOOLEAN:
            cell->value.boolean = *((char *)value);
            break;
        case CHARACTER:
            cell->value.character = *((char *)value);
            break;
        case STRING:
            cell->value.string = strdup((char *)value);
            break;
        
        default:
            fprintf(stderr, "update_vm_cell Invalid type\n");
            exit(EXIT_FAILURE);
    }
}

const char *format_cell(void *data) {
    vm_cell *cell = (vm_cell *)data;
    static char buffer[64];  

    if (!cell->is_initialized) {
        sprintf(buffer, "null (%s)", 
                cell->type == INTEGER   ? "int" :
                cell->type == REAL      ? "float" :
                cell->type == BOOLEAN   ? "bool" :
                cell->type == CHARACTER ? "char" : "unknown");
        return buffer;
    }

    switch (cell->type) {
        case INTEGER:
            sprintf(buffer, "%d (int)", cell->value.integer);
            break;
        case REAL:
            sprintf(buffer, "%.2f (float)", cell->value.real);
            break;
        case BOOLEAN:
            sprintf(buffer, "%s (bool)", cell->value.boolean ? "true" : "false");
            break;
        case CHARACTER:
            sprintf(buffer, "'%c' (char)", cell->value.character);
            break;
        case STRING: 
            sprintf(buffer, "%s (string)", cell->value.string);
            break;

        default:
            sprintf(buffer, "Unknown Type");
            break;
    }

    return buffer;
}

void fprintf_vm_cell(FILE *out, vm_cell cell) {
    const int col_width_type = 10;
    const int col_width_value = 5;

    print_table_separator(out, 2, col_width_type, col_width_value);
    print_table_header(out, 2, col_width_type, "Type", col_width_value, "Value");
    print_table_separator(out, 2, col_width_type, col_width_value);

    // Get the type as a string
    const char *type_str = 
        (cell.type == INTEGER)   ? "INTEGER" :
        (cell.type == REAL)      ? "REAL" :
        (cell.type == BOOLEAN)   ? "BOOLEAN" :
        (cell.type == CHARACTER) ? "CHARACTER" : 
        (cell.type == STRING)    ? "STRING" : "UNKNOWN";

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
