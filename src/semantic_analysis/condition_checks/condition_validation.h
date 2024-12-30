#ifndef __CONDITION_VALIDATION_H__
#define __CONDITION_VALIDATION_H__

/**
 * @brief Validates the type of a condition expression.
 * 
 * This function ensures that the condition provided for a control structure (e.g., if, while) 
 * is a valid boolean expression. If the condition is not a boolean, an error is logged with 
 * detailed information about the type mismatch and the location of the issue.
 * 
 * @param condition A pointer to the `Node` representing the condition expression.
 */
void check_condition(Node *condition);

#endif // __CONDITION_VALIDATION_H__