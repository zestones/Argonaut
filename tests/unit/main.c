/* Unit-test runner: links against the compiler/VM objects (without the generated parsers). */
#include "test.h"

#include "utils/errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The global diagnostic state normally lives in the generated parser. */
Error error;

static argo_test *registry = NULL;
static int current_failed = 0;

void argo_register_test(argo_test *test) {
    /* Keep registration order (constructor order within a file is source order). */
    argo_test **tail = &registry;
    while (*tail != NULL) tail = &(*tail)->next;
    test->next = NULL;
    *tail = test;
}

void argo_test_fail(const char *file, int line, const char *message) {
    current_failed = 1;
    fprintf(stderr, "    %s:%d: %s\n", file, line, message);
}

int main(int argc, char **argv) {
    const char *filter = (argc > 1) ? argv[1] : NULL;
    int run = 0, passed = 0, failed = 0, xfailed = 0, xpassed = 0;

    for (argo_test *t = registry; t != NULL; t = t->next) {
        if (filter != NULL && strstr(t->name, filter) == NULL) continue;

        run++;
        current_failed = 0;
        error.type = NO_ERROR;
        error.line = 1;
        error.column = 1;
        error.filename = "unit-test";
        t->fn();

        if (t->expect_fail) {
            if (current_failed) {
                xfailed++;
                printf("[XFAIL] %s\n", t->name);
            } else {
                xpassed++;
                printf("[XPASS] %s  <- unexpectedly passes: remove the TEST_XFAIL marker\n", t->name);
            }
        } else if (current_failed) {
            failed++;
            printf("[FAIL]  %s\n", t->name);
        } else {
            passed++;
            printf("[PASS]  %s\n", t->name);
        }
    }

    printf("\n%d tests: %d passed, %d failed, %d expected failures, %d unexpected passes\n",
           run, passed, failed, xfailed, xpassed);
    return (failed > 0 || xpassed > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
