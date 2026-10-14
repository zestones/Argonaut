#include "test.h"

#include "utils/numeric_parser.h"

TEST(numeric_parser_accepts_integer_values) {
    int value = 0;
    ASSERT_TRUE(parse_integer_text("0", &value));
    ASSERT_EQ_INT(0, value);
    ASSERT_TRUE(parse_integer_text("-2147483648", &value));
    ASSERT_EQ_INT(-2147483647 - 1, value);
    ASSERT_TRUE(parse_integer_text("2147483647", &value));
    ASSERT_EQ_INT(2147483647, value);
    ASSERT_TRUE(parse_integer_text("42", NULL));
}

TEST(numeric_parser_rejects_invalid_integers) {
    int value = 42;
    ASSERT_FALSE(parse_integer_text(NULL, &value));
    ASSERT_FALSE(parse_integer_text("", &value));
    ASSERT_FALSE(parse_integer_text("12oops", &value));
    ASSERT_FALSE(parse_integer_text("2147483648", &value));
    ASSERT_FALSE(parse_integer_text("-2147483649", &value));
    ASSERT_EQ_INT(42, value);
}

TEST(numeric_parser_accepts_real_values) {
    double value = 0.0;
    ASSERT_TRUE(parse_real_text("-2.5", &value));
    ASSERT_TRUE(value == -2.5);
    ASSERT_TRUE(parse_real_text("1e3", &value));
    ASSERT_TRUE(value == 1000.0);
    ASSERT_TRUE(parse_real_text("2.5", NULL));
}

TEST(numeric_parser_rejects_invalid_reals) {
    double value = 42.0;
    ASSERT_FALSE(parse_real_text(NULL, &value));
    ASSERT_FALSE(parse_real_text("", &value));
    ASSERT_FALSE(parse_real_text("2.5oops", &value));
    ASSERT_FALSE(parse_real_text("nan", &value));
    ASSERT_FALSE(parse_real_text("inf", &value));
    ASSERT_FALSE(parse_real_text("3.5e38", &value));
    ASSERT_EQ_INT(42, value);
}
