#include "../../../../lib/table_printer.h"

#include "stack_frame.h"

stack_frame construct_stack_frame(int dynamic_link, int region_index) {
    stack_frame frame;
 
    // frame.static_link = static_link;
    frame.dynamic_link = dynamic_link;
    frame.region_index = region_index;

    frame.cells = construct_stack();
    frame.region_value = construct_vm_cell(NULL_VALUE, NULL);

    return frame;
}

/**
 * @brief Adds a new cell to the stack frame's stack.
 * 
 * This function adds a new cell of the specified type to the stack of a given stack frame. 
 * It is used to allocate space for variables and values in the stack frame.
 * 
 * @param frame A pointer to the stack frame to which the cell will be added.
 * @param type The type of the cell to be added.
 */
static void add_cell_to_stack_frame(stack_frame *frame, int type) {
    vm_cell cell = construct_vm_cell(type, NULL);
    push(&frame->cells, &cell, sizeof(vm_cell));
}

void allocate_cells_to_stack_frame(stack_frame *frame, int type, int size) {
    for (int i = 0; i < size; i++) {
        add_cell_to_stack_frame(frame, type);
    }
}

void update_cell_in_stack_frame(stack_frame *frame, int address, vm_cell to) {
    vm_cell cell = *(vm_cell *) get_stack_value(frame->cells, address);

    update_vm_cell(&cell, get_vm_cell_value(to));
    update_stack(&frame->cells, address, &cell, sizeof(vm_cell));
}

vm_cell get_cell_from_stack_frame(stack_frame frame, int address) {
    return *(vm_cell *) get_stack_value(frame.cells, address);
}

void fprintf_stack_frame(FILE *out, stack_frame frame) {
    const int num_col = 2;
    const int width = 15;
    char str_region_index[width];
    char str_dynamic_link[width];


    print_table_separator(out, num_col, width, width, width, width);
    fprintf(stdout, "\n");

    print_table_separator(out, num_col, width, width, width, width);
    print_table_header(out, num_col, width, "DYNAMIC LINK", width, "REGION INDEX");
    print_table_separator(out, num_col, width, width, width, width);

    sprintf(str_region_index, "%d", frame.region_index);
    sprintf(str_dynamic_link, "%d", frame.dynamic_link);

    print_table_row(out,
                    num_col, 
                    width, str_dynamic_link,
                    width, str_region_index
                );

    print_table_separator(out, num_col, width, width, width, width);
    
    print_stack(frame.cells, format_cell);

    print_table_title(out, "Return Value");
    fprintf_vm_cell(out, frame.region_value);

    fprintf(stdout, "\n");
    print_table_separator(out, num_col, width, width, width, width);
    fprintf(stdout, "\n");
}