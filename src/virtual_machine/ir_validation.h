#ifndef __IR_VALIDATION_H__
#define __IR_VALIDATION_H__

/**
 * @brief Checks the consistency of the tables and ASTs loaded from an IR file.
 *
 * The VM trusts the IR it executes: every node type must exist, every lexeme and declaration
 * index must point to a loaded row, every call must target a function or procedure whose
 * region exists, and region 0 (the program) must be present. A file that violates one of
 * these rules is reported as corrupted and the VM exits with EXIT_FAILURE instead of reading
 * out of bounds or following dangling indices.
 */
void validate_loaded_ir(void);

#endif // __IR_VALIDATION_H__
