#include "test.h"

#include "ast/node_type.h"
#include "symbol_table/declaration/declaration_table.h"
#include "symbol_table/hash/hash_table.h"
#include "symbol_table/lexeme/lexeme_table.h"
#include "symbol_table/representation/representation_table.h"
#include "symbol_table/utility.h"
#include "utils/index_validation.h"

/* The symbol tables are process-wide singletons whose init_* functions are not re-entrant:
 * init_lexeme_table() memsets the rows to -1 (non-terminated strings) but keeps the size
 * counter, so calling it twice makes the next insert_lexeme() read past the table (ASan
 * global-buffer-overflow with strict_string_checks=1). They are therefore initialised once
 * here, and the tests only rely on relative properties (an index returned earlier stays valid). */
static void ensure_tables_initialised(void) {
    static int done = 0;
    if (done) return;
    init_hash_table();
    init_lexeme_table();
    init_declaration_table();
    init_representation_table();
    done = 1;
}

TEST(hash_function_is_deterministic_and_in_range) {
    int h1 = hash_function("fibonacci", 9);
    int h2 = hash_function("fibonacci", 9);
    ASSERT_EQ_INT(h1, h2);
    ASSERT_TRUE(h1 >= 0 && h1 < HASH_TABLE_SIZE);
    /* A lexeme made of characters with a negative `char` value must still hash in range. */
    int h3 = hash_function("\xe9\xe8", 2);
    ASSERT_TRUE(h3 >= 0 && h3 < HASH_TABLE_SIZE);
}

TEST(table_index_validation_accepts_boundary_indices) {
    validate_table_index("Unit-test table", 0, 2);
    validate_table_index("Unit-test table", 1, 2);
}

TEST(hash_table_insert_then_lookup) {
    ensure_tables_initialised();
    insert_hash("alpha", 12);
    ASSERT_EQ_INT(12, get_hash_value(hash_function("alpha", 5)));
}

TEST(lexeme_table_returns_same_index_for_duplicates) {
    ensure_tables_initialised();
    int first = insert_lexeme("unit_test_lexeme_a");
    int second = insert_lexeme("unit_test_lexeme_b");
    ASSERT_TRUE(second > first);
    ASSERT_EQ_INT(first, insert_lexeme("unit_test_lexeme_a"));
    ASSERT_EQ_STR("unit_test_lexeme_a", get_lexeme(first));
    ASSERT_EQ_STR("unit_test_lexeme_b", get_lexeme(second));
}

TEST(lexeme_table_keeps_literal_text_verbatim) {
    ensure_tables_initialised();
    int s = insert_lexeme("\"hello world\\n\"");
    ASSERT_EQ_STR("\"hello world\\n\"", get_lexeme(s));
    int c = insert_lexeme("'x'");
    ASSERT_EQ_STR("'x'", get_lexeme(c));
    int n = insert_lexeme("-42");
    ASSERT_EQ_STR("-42", get_lexeme(n));
}

TEST(representation_table_insert_update_get) {
    ensure_tables_initialised();
    int i = insert_representation(5);
    int j = insert_representation(6);
    ASSERT_EQ_INT(i + 1, j);
    ASSERT_EQ_INT(5, get_representation_value(i));
    ASSERT_EQ_INT(6, get_representation_value(j));
    update_representation(i, 50);
    ASSERT_EQ_INT(50, get_representation_value(i));
}

TEST(function_and_procedure_parameter_counts_follow_their_representation_layouts) {
    ensure_tables_initialised();

    int function_lexeme = insert_lexeme("unit_test_function");
    int function_representation = insert_representation(A_INTEGER_LITERAL);
    insert_representation(2);
    insert_declaration(function_lexeme, TYPE_FUNC, 0, function_representation, 1);
    ASSERT_EQ_INT(2, get_func_proc_parameter_count(function_lexeme));

    int procedure_lexeme = insert_lexeme("unit_test_procedure");
    int procedure_representation = insert_representation(3);
    insert_declaration(procedure_lexeme, TYPE_PROC, 0, procedure_representation, 1);
    ASSERT_EQ_INT(3, get_func_proc_parameter_count(procedure_lexeme));
}
