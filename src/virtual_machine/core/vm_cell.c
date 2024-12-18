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
                cell.value.real = *((float *)value);
                break;
            case BOOLEAN:
                cell.value.boolean = *((char *)value);
                break;
            case CHARACTER:
                cell.value.character = *((char *)value);
                break;
            default:
                fprintf(stderr, "construct_vm_cell Invalid type\n");
                exit(EXIT_FAILURE);
        }
    }

    return cell;
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
        default:
            sprintf(buffer, "Unknown Type");
            break;
    }

    return buffer;
}