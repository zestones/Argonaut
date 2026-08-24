#ifndef ARGONAUT_VM_H
#define ARGONAUT_VM_H

#include <stdio.h>

/**
 * @brief Loads, validates, and executes a transitional text IR stream.
 *
 * @param input The input stream containing the serialized IR.
 * @param input_name The input name retained by diagnostics.
 */
void execute_ir(FILE *input, const char *input_name);

/**
 * @brief Prints the loaded IR state and the current VM stack for diagnostics.
 *
 * @param out The output stream that receives the VM state.
 */
void fprintf_vm_state(FILE *out);

#endif // ARGONAUT_VM_H
