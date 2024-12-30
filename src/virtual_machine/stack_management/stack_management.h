#ifndef __STACK_MANAGEMENT_H__
#define __STACK_MANAGEMENT_H__

#include "frame/stack_frame.h"
#include "../core/vm_cell.h"

/**
 * @brief Initializes the execution stack.
 * 
 * This function initializes the execution stack, which is responsible for managing the 
 * stack frames during the execution of the program. The stack is constructed as an empty stack.
 */
void init_execution_stack();

/**
 * @brief Peeks at the top stack frame in a mutable way.
 * 
 * This function returns a pointer to the top stack frame of the execution stack in a mutable 
 * form, allowing modifications to the top frame directly.
 * 
 * @return A pointer to the stack frame at the top of the execution stack.
 */
stack_frame *peek_execution_stack_as_mutable();

/**
 * @brief Peeks at the top stack frame in a read-only way.
 * 
 * This function returns a copy of the stack frame at the top of the execution stack. 
 * It allows read access to the top frame without modifying it.
 * 
 * @return A copy of the stack frame at the top of the execution stack.
 */
stack_frame peek_execution_stack();

/**
 * @brief Pushes a new stack frame onto the execution stack.
 * 
 * This function pushes a new stack frame onto the execution stack. The frame contains the 
 * static and dynamic links, the region index, and the stack of variables used during 
 * the function or procedure call.
 * 
 * @param frame The stack frame to be pushed onto the execution stack.
 */
void push_frame_to_execution_stack(stack_frame frame);

/**
 * @brief Pops the top stack frame from the execution stack.
 * 
 * This function pops the top stack frame from the execution stack and returns it. The 
 * frame is removed from the stack and can be used for further processing or reporting.
 * 
 * @return The stack frame that was at the top of the execution stack before the pop.
 */
stack_frame pop_frame_from_execution_stack();

/**
 * @brief Retrieves a stack frame by its ID.
 * 
 * This function allows access to a stack frame given its ID in the execution stack. 
 * The ID corresponds to the frame's position in the stack.
 * 
 * @param id The ID of the stack frame to retrieve.
 * 
 * @return A pointer to the stack frame at the specified ID.
 */
stack_frame *get_stack_frame_by_id(int id);

/**
 * @brief Finds a stack frame by its region index.
 * 
 * This function searches for a stack frame with a specific region index in the execution stack. 
 * If a frame with the specified region index is found, it is returned.
 * 
 * @param region_index The region index to search for.
 * 
 * @return A pointer to the stack frame that corresponds to the given region index.
 * 
 * @warning This function will terminate the program if no stack frame with the given region index is found.
 */
stack_frame *find_stack_frame_by_region_index(int region_index);

/**
 * @brief Retrieves the ID of the current stack frame.
 * 
 * This function returns the ID (index) of the current stack frame at the top of the 
 * execution stack. This is useful for tracking the current execution context.
 * 
 * @return The index of the current stack frame in the execution stack.
 */
int get_execution_stack_current_frame_id();

/**
 * @brief Prints the contents of the execution stack.
 * 
 * This function prints the details of all stack frames currently on the execution stack. 
 * It prints each stack frame's static link, dynamic link, region index, and the values of 
 * the cells in the stack. The frames are printed in a tabular format for easy readability.
 * 
 * @param out A pointer to the `FILE` stream where the stack contents will be printed.
 */
void fprintf_vm_stack(FILE *out);

#endif 

