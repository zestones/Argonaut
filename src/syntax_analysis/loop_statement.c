#include "loop_statement.h"

// Nesting depth of the loops being parsed: 0 outside any loop. A flag would be cleared by the
// end of an inner loop, and a 'break' placed after it in the outer loop would be rejected.
static int loop_depth = 0;

int loop_context_active(void) {
    return loop_depth > 0;
}

void activate_loop_context(void) {
    loop_depth++;
}

void deactivate_loop_context(void) {
    loop_depth--;
}
