#ifndef __LOOP_STATEMENT_H__
#define __LOOP_STATEMENT_H__

/**
 * Checks if the loop context is currently active.
 * @return 1 if inside a loop, 0 otherwise.
 */
int loop_context_active();

/**
 * Activates the loop context to indicate entry into a loop.
 */
void activate_loop_context();

/**
 * Deactivates the loop context to indicate exit from a loop.
 */
void deactivate_loop_context();

#endif // __LOOP_STATEMENT_H__
