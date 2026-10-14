#include "test.h"

#include "ast/node_type.h"
#include "virtual_machine/interpreter/expression/expression.h"

TEST(comparisons_share_the_same_operations_across_scalar_types) {
    int one = 1;
    int two = 2;
    vm_cell integer_one = construct_vm_cell(INTEGER, &one);
    vm_cell integer_two = construct_vm_cell(INTEGER, &two);
    ASSERT_TRUE(resolve_comparison(A_LESS_THAN_OP, integer_one, integer_two).value.boolean);

    double real_one = 1.5;
    double real_two = 1.5;
    vm_cell real_left = construct_vm_cell(REAL, &real_one);
    vm_cell real_right = construct_vm_cell(REAL, &real_two);
    ASSERT_TRUE(resolve_comparison(A_EQUAL_OP, real_left, real_right).value.boolean);

    char character_one = 'a';
    char character_two = 'b';
    vm_cell character_left = construct_vm_cell(CHARACTER, &character_one);
    vm_cell character_right = construct_vm_cell(CHARACTER, &character_two);
    ASSERT_TRUE(resolve_comparison(A_NOT_EQUAL_OP, character_left, character_right).value.boolean);
}
