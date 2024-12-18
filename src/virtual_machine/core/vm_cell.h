#ifndef __VM_CELL_H__
#define __VM_CELL_H__

#include <stdio.h>
#include <stdlib.h>

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

void update_vm_cell(vm_cell *cell, void *value);

const char *format_cell(void *data);

#endif // __VM_CELL_H__