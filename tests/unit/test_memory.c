#include "test.h"

#include "utils/memory.h"

TEST(checked_memory_helpers_allocate_resize_and_duplicate) {
    int *values = xmalloc(2 * sizeof(*values));
    values[0] = 12;
    values[1] = 34;

    values = xrealloc(values, 3 * sizeof(*values));
    values[2] = 56;
    ASSERT_EQ_INT(12, values[0]);
    ASSERT_EQ_INT(34, values[1]);
    ASSERT_EQ_INT(56, values[2]);
    free(values);

    char *copy = xstrdup("Argonaut");
    ASSERT_EQ_STR("Argonaut", copy);
    free(copy);

    void *zero_sized = xmalloc(0);
    ASSERT_NOT_NULL(zero_sized);
    free(zero_sized);
}
