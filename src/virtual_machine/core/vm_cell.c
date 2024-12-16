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
            sprintf(buffer, "%d", cell->value.integer);
            break;
        case REAL:
            sprintf(buffer, "%.2f", cell->value.real);
            break;
        case BOOLEAN:
            sprintf(buffer, "%s", cell->value.boolean ? "true" : "false");
            break;
        case CHARACTER:
            sprintf(buffer, "'%c'", cell->value.character);
            break;
        default:
            sprintf(buffer, "Unknown Type");
            break;
    }

    return buffer;
}