#include "test.h"

#include "utils/text_buffer.h"

TEST(text_buffer_starts_empty) {
    TextBuffer buffer = construct_text_buffer();
    ASSERT_NULL(buffer.data);
    ASSERT_EQ_INT(0, buffer.length);
    ASSERT_EQ_INT(0, buffer.capacity);

    char *text = release_text_buffer(&buffer);
    ASSERT_EQ_STR("", text);
    ASSERT_NULL(buffer.data);
    ASSERT_EQ_INT(0, buffer.length);
    ASSERT_EQ_INT(0, buffer.capacity);
    free(text);
}

TEST(text_buffer_appends_text_bytes_and_characters) {
    TextBuffer buffer = construct_text_buffer();
    text_buffer_append(&buffer, "Argo");
    text_buffer_append_bytes(&buffer, "nautical", 4);
    text_buffer_append_character(&buffer, '!');

    char *text = release_text_buffer(&buffer);
    ASSERT_EQ_STR("Argonaut!", text);
    free(text);
}

TEST(text_buffer_grows_and_can_be_reused) {
    TextBuffer buffer = construct_text_buffer();
    for (int i = 0; i < 100; i++)
        text_buffer_append(&buffer, "0123456789");
    ASSERT_EQ_INT(1000, buffer.length);

    char *text = release_text_buffer(&buffer);
    ASSERT_EQ_INT(1000, strlen(text));
    ASSERT_EQ_STR("0123456789", text + 990);
    free(text);

    text_buffer_append(&buffer, "reused");
    destroy_text_buffer(&buffer);
    ASSERT_NULL(buffer.data);
    ASSERT_EQ_INT(0, buffer.length);
    ASSERT_EQ_INT(0, buffer.capacity);
}
