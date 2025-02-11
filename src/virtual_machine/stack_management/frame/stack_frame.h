#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include "../../../utils/stack.h"
#include "../../core/vm_cell.h"

typedef struct {
    Stack cells;

    int static_link;
    int dynamic_link;
    int region_index;

    vm_cell region_value;
} stack_frame;

/**
 * @brief Constructs a new stack frame for the function or procedure call.
 * 
 * This function creates a new stack frame that contains the necessary information 
 * to manage function execution. The stack frame holds the static and dynamic links 
 * (pointers to previous frames), the region index, and a stack to hold the variables 
 * in the frame. Additionally, it initializes the `region_value` to indicate the return 
 * value or state of the region.
 * 
 * @param dynamic_link The dynamic link, which points to the calling function's frame.
 * @param region_index The index of the region this frame belongs to.
 * 
 * @return A `stack_frame` that is initialized with the provided values.
 */
stack_frame construct_stack_frame(int dynamic_link, int region_index);

/**
 * @brief Allocates multiple cells to a stack frame.
 * 
 * This function adds multiple cells of the same type to the stack of the given stack frame.
 * It is used to allocate memory for an array or structure inside the stack frame.
 * 
 * @param frame A pointer to the stack frame where the cells will be allocated.
 * @param type The type of the cells to be added.
 * @param size The number of cells to be allocated.
 */
void allocate_cells_to_stack_frame(stack_frame *frame, int type, int size);

/**
 * @brief Updates the value of a cell at a given address in the stack frame.
 * 
 * This function updates the value of a cell at a specific address in the stack frame.
 * It retrieves the cell from the stack, updates its value, and stores it back in the stack.
 * 
 * @param frame A pointer to the stack frame where the cell is located.
 * @param address The address of the cell to be updated.
 * @param to The new value to update the cell with.
 */
void update_cell_in_stack_frame(stack_frame *frame, int address, vm_cell cell);

/**
 * @brief Retrieves the value of a cell from the stack frame.
 * 
 * This function retrieves a cell at a specific address in the stack frame. The cell can 
 * then be used for reading or manipulation. 
 * 
 * @param frame The stack frame containing the cell.
 * @param address The address of the cell to retrieve.
 * 
 * @return The `vm_cell` located at the specified address in the stack frame.
 */
vm_cell get_cell_from_stack_frame(stack_frame frame, int address);

/**
 * @brief Prints a detailed representation of a stack frame.
 * 
 * This function prints a human-readable representation of a stack frame, including its 
 * static link, dynamic link, region index, and the values stored in the stack cells. 
 * It uses a formatted table for easy readability.
 * 
 * @param out A pointer to the `FILE` stream where the stack frame details will be printed.
 * @param frame The stack frame to print.
 */
void fprintf_stack_frame(FILE *out, stack_frame frame);

#endif // __STACK_FRAME_H__