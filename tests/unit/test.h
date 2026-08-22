/*
 * Minimal unit-test harness for Argonaut (no external dependency).
 *
 *   #include "test.h"
 *   TEST(stack_push_pop) {
 *       ...
 *       ASSERT_EQ_INT(3, size(stack));
 *   }
 *
 * Tests register themselves at load time; `tests/unit/main.c` runs them all and
 * returns non-zero if any test fails. `TEST_XFAIL(name)` declares a test that is
 * expected to fail (documented bug): it is reported as XFAIL, and as an error
 * (XPASS) once the bug is fixed — remove the XFAIL marker then.
 */
#ifndef ARGO_UNIT_TEST_H
#define ARGO_UNIT_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*argo_test_fn)(void);

typedef struct argo_test {
    const char *name;
    argo_test_fn fn;
    int expect_fail;
    struct argo_test *next;
} argo_test;

void argo_register_test(argo_test *test);
void argo_test_fail(const char *file, int line, const char *message);

#define ARGO_TEST_IMPL(name, xfail)                                                        \
    static void name(void);                                                                \
    static argo_test name##_entry = {#name, name, xfail, NULL};                            \
    __attribute__((constructor)) static void name##_register(void) {                       \
        argo_register_test(&name##_entry);                                                 \
    }                                                                                      \
    static void name(void)

#define TEST(name) ARGO_TEST_IMPL(name, 0)
#define TEST_XFAIL(name) ARGO_TEST_IMPL(name, 1)

#define FAIL(message)                                                                      \
    do {                                                                                   \
        argo_test_fail(__FILE__, __LINE__, message);                                       \
        return;                                                                            \
    } while (0)

#define ASSERT_TRUE(expr)                                                                  \
    do {                                                                                   \
        if (!(expr)) FAIL("assertion failed: " #expr);                                     \
    } while (0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

#define ASSERT_EQ_INT(expected, actual)                                                    \
    do {                                                                                   \
        long argo_e_ = (long)(expected), argo_a_ = (long)(actual);                         \
        if (argo_e_ != argo_a_) {                                                          \
            char argo_buf_[256];                                                           \
            snprintf(argo_buf_, sizeof argo_buf_, "%s == %s: expected %ld, got %ld",       \
                     #expected, #actual, argo_e_, argo_a_);                                \
            FAIL(argo_buf_);                                                               \
        }                                                                                  \
    } while (0)

#define ASSERT_EQ_STR(expected, actual)                                                    \
    do {                                                                                   \
        const char *argo_e_ = (expected), *argo_a_ = (actual);                             \
        if (argo_e_ == NULL || argo_a_ == NULL || strcmp(argo_e_, argo_a_) != 0) {         \
            char argo_buf_[512];                                                           \
            snprintf(argo_buf_, sizeof argo_buf_, "%s == %s: expected \"%s\", got \"%s\"", \
                     #expected, #actual, argo_e_ ? argo_e_ : "(null)",                     \
                     argo_a_ ? argo_a_ : "(null)");                                        \
            FAIL(argo_buf_);                                                               \
        }                                                                                  \
    } while (0)

#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != NULL)
#define ASSERT_NULL(ptr) ASSERT_TRUE((ptr) == NULL)

#endif /* ARGO_UNIT_TEST_H */
