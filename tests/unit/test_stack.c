#include "test.h"

#include "utils/stack.h"

/* The generic stack stores malloc'ed copies; pop() hands the copy back to the caller,
 * who owns it, and destroy_stack() frees whatever is left. */

TEST(stack_starts_empty) {
    Stack s = construct_stack();
    ASSERT_TRUE(is_empty(s));
    ASSERT_FALSE(is_full(s));
    ASSERT_EQ_INT(0, size(s));
}

TEST(stack_push_pop_is_lifo) {
    Stack s = construct_stack();
    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++)
        push(&s, &values[i], sizeof(int));
    ASSERT_EQ_INT(3, size(s));

    int *top = (int *) peek(s);
    ASSERT_EQ_INT(30, *top);

    for (int i = 2; i >= 0; i--) {
        int *v = (int *) pop(&s);
        ASSERT_EQ_INT(values[i], *v);
        free(v);
    }
    ASSERT_TRUE(is_empty(s));
}

TEST(stack_get_and_update_by_index) {
    Stack s = construct_stack();
    int a = 1, b = 2, c = 3;
    push(&s, &a, sizeof(int));
    push(&s, &b, sizeof(int));
    push(&s, &c, sizeof(int));

    ASSERT_EQ_INT(1, *(int *) get_stack_value(s, 0));
    ASSERT_EQ_INT(3, *(int *) get_stack_value(s, 2));

    int replacement = 42;
    update_stack(&s, 1, &replacement, sizeof(int));
    ASSERT_EQ_INT(42, *(int *) get_stack_value(s, 1));
    ASSERT_EQ_INT(3, size(s));

    destroy_stack(&s);
    ASSERT_TRUE(is_empty(s));
}

TEST(stack_grows_beyond_its_initial_capacity) {
    Stack s = construct_stack();
    for (int i = 0; i < 1000; i++)
        push(&s, &i, sizeof(int));
    ASSERT_EQ_INT(1000, size(s));
    ASSERT_EQ_INT(999, *(int *) peek(s));
    ASSERT_EQ_INT(0, *(int *) get_stack_value(s, 0));
    ASSERT_EQ_INT(500, *(int *) get_stack_value(s, 500));
    destroy_stack(&s);
}

TEST(stack_copy_is_deep) {
    Stack src = construct_stack();
    int a = 7, b = 8;
    push(&src, &a, sizeof(int));
    push(&src, &b, sizeof(int));

    Stack dst = construct_stack();
    stack_cpy(&dst, src, sizeof(int));
    ASSERT_EQ_INT(2, size(dst));
    ASSERT_EQ_INT(8, *(int *) peek(dst));

    /* Mutating the copy must not touch the source. */
    int other = 99;
    update_stack(&dst, 1, &other, sizeof(int));
    ASSERT_EQ_INT(8, *(int *) get_stack_value(src, 1));
    ASSERT_EQ_INT(99, *(int *) get_stack_value(dst, 1));

    destroy_stack(&src);
    destroy_stack(&dst);
}
