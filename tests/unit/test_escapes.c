#include "test.h"

#include "lexer/escapes.h"

TEST(decode_escapes_translates_the_c_escapes) {
    const char *bad = NULL;
    char *s = decode_escapes("a\\tb\\nc\\\\d\\\"e\\'f", 16, &bad);
    ASSERT_NOT_NULL(s);
    ASSERT_EQ_STR("a\tb\nc\\d\"e'f", s);
    free(s);
}

TEST(decode_escapes_rejects_unknown_sequences) {
    const char *text = "x\\qy";
    const char *bad = NULL;
    ASSERT_NULL(decode_escapes(text, 4, &bad));
    ASSERT_TRUE(bad == text + 1);
}

TEST(escape_text_round_trips_through_unescape_text) {
    const char *original = "tab\tnew\nline\\back \"quoted\"";
    char *escaped = escape_text(original);
    ASSERT_EQ_STR("tab\\tnew\\nline\\\\back \"quoted\"", escaped);
    char *back = unescape_text(escaped);
    ASSERT_EQ_STR(original, back);
    free(escaped);
    free(back);
}
