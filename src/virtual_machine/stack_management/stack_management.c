#include "../../utils/stack.h"

#include "stack_management.h"

static Stack execution_stack;

void init_execution_stack() {
    execution_stack = construct_stack();
}

void push_execution_stack(int type, void *value) {
    vm_cell cell = construct_vm_cell(type, NULL);
    push(&execution_stack, &cell, sizeof(vm_cell));
}

void allocate_execution_cells(int type, int size) {
    for (int i = 0; i < size; i++) {
        vm_cell cell = construct_vm_cell(type, NULL);
        push_execution_stack(type, &cell);
    }
}

void update_execution_cell(int address, vm_cell to) {
    vm_cell cell = *(vm_cell *) get_stack_value(execution_stack, address);

    update_vm_cell(&cell, get_vm_cell_value(to));
    update_stack(&execution_stack, address, &cell, sizeof(vm_cell));
}

vm_cell get_execution_cell(int address) {
    return *(vm_cell *) get_stack_value(execution_stack, address);
}

void fprintf_vm_stack(FILE *out) {
    print_stack(execution_stack, format_cell);
}