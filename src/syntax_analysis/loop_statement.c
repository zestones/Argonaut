#include "loop_statement.h"

// Static variable to track loop context.
// 0 means not in a loop, 1 means in a loop.
static int loop_context = 0;

int loop_context_active() { return loop_context; }

void activate_loop_context() { loop_context = 1; }

void deactivate_loop_context() { loop_context = 0; }
