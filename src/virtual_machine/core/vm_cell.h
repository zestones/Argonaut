#ifndef __VM_CELL_H__
#define __VM_CELL_H__

#include <stdio.h>
#include <stdlib.h>

#include "../../utils/utils.h"

typedef struct {
    union {
        int integer;
        float real;      
        char boolean;    
        char character;  
    } value;

    enum { INTEGER, REAL, BOOLEAN, CHARACTER } type;
    int is_initialized;
} vm_cell;

vm_cell construct_vm_cell(int type, void *value);

void *get_vm_cell_value(vm_cell cell);

void update_vm_cell(vm_cell *cell, void *value);

const char *format_cell(void *data);

void fprintf_vm_cell(FILE *out, vm_cell cell);

#endif // __VM_CELL_H__