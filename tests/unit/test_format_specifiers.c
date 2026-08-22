#include "test.h"

#include "ast/node_type.h"
#include "utils/utils.h"
#include "semantic_analysis/function_checks/common/format_specifiers.h"

TEST(count_format_specifiers_counts_supported_ones_only) {
    ASSERT_EQ_INT(0, count_format_specifiers("hello"));
    ASSERT_EQ_INT(2, count_format_specifiers("%d and %f"));
    ASSERT_EQ_INT(4, count_format_specifiers("%d%f%s%c"));
    /* %x is not a supported specifier and must not be counted. */
    ASSERT_EQ_INT(1, count_format_specifiers("%x %d"));
}

TEST(get_nth_format_specifier_walks_in_order) {
    const char *fmt = "a=%d b=%f c=%c d=%s";
    ASSERT_EQ_INT('d', get_nth_format_specifier(fmt, 0));
    ASSERT_EQ_INT('f', get_nth_format_specifier(fmt, 1));
    ASSERT_EQ_INT('c', get_nth_format_specifier(fmt, 2));
    ASSERT_EQ_INT('s', get_nth_format_specifier(fmt, 3));
    ASSERT_EQ_INT('\0', get_nth_format_specifier(fmt, 4));
}

TEST(strip_quotes_removes_surrounding_double_quotes) {
    char *s = strip_quotes("\"hello\"");
    ASSERT_NOT_NULL(s);
    ASSERT_EQ_STR("hello", s);
    free(s);

    char *empty = strip_quotes("\"\"");
    ASSERT_NOT_NULL(empty);
    ASSERT_EQ_STR("", empty);
    free(empty);

    /* Not a quoted literal: nothing to strip. */
    ASSERT_NULL(strip_quotes("hello"));
}

TEST(percent_percent_is_not_a_specifier) {
    ASSERT_EQ_INT(0, count_format_specifiers("100%%"));
    ASSERT_EQ_INT(1, count_format_specifiers("%%%d%%"));
    ASSERT_EQ_INT('d', get_nth_format_specifier("%%%d%%", 0));
    ASSERT_EQ_INT('\0', get_nth_format_specifier("%%", 0));
}

TEST(format_specifier_compatibility_matrix) {
    ASSERT_TRUE(is_compatible_with_format_specifier(A_INTEGER_LITERAL, 'd'));
    ASSERT_TRUE(is_compatible_with_format_specifier(A_FLOAT_LITERAL, 'f'));
    ASSERT_TRUE(is_compatible_with_format_specifier(STRING_LITERAL_TYPE, 's'));
    ASSERT_FALSE(is_compatible_with_format_specifier(4, 's')); /* 4 = the first user type, not a string */
    ASSERT_TRUE(is_compatible_with_format_specifier(A_CHARACTER_LITERAL, 'c'));
    ASSERT_FALSE(is_compatible_with_format_specifier(A_INTEGER_LITERAL, 'f'));
    ASSERT_TRUE(is_compatible_with_format_specifier(A_BOOLEAN_LITERAL, 'd')); /* prints 0 or 1 */
    ASSERT_TRUE(is_compatible_with_format_specifier(A_BOOLEAN_LITERAL, 'b'));
    ASSERT_FALSE(is_compatible_with_format_specifier(A_INTEGER_LITERAL, 'b'));
    ASSERT_EQ_INT(1, count_format_specifiers("%b"));
    ASSERT_FALSE(is_compatible_with_format_specifier(A_INTEGER_LITERAL, 'x'));
}
