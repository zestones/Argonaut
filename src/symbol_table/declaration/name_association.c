#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../data/region_table.h"
#include "declaration_table.h"

#include "../../utils/utils.h"
#include "../../utils/stack.h"

static int find_declaration_index_in_region_by_nature(int tlex_index, int region, int nature_filter) {
    Declaration *declaration_table = get_declaration_table();    
    int index = tlex_index;
    
    Stack overload_stack = construct_stack();
    int null_value = NULL_VALUE;
    push(&overload_stack, &null_value, sizeof(int));

    while (index != NULL_VALUE) {
        if (declaration_table[index].region == region && 
            (nature_filter == NULL_VALUE || declaration_table[index].nature == nature_filter)
        ) {
            push(&overload_stack, &index, sizeof(int));
        }
        
        index = declaration_table[index].next;
    }

    // return the last declaration in the stack
    // if there are multiple declarations with the same nature
    // in the same region
    return *(int *) pop(&overload_stack);
}

static int find_declaration_in_stack(int tlex_index, int nature_filter) {
    int index = tlex_index;
    if (is_declaration_base_type(tlex_index)) return index;

    Stack tmp_stack = construct_stack();
    stack_cpy(&tmp_stack, get_region_stack(), sizeof(int));

    while (!is_empty(tmp_stack)) {
        int current_region = *(int *) pop(&tmp_stack);
        index = find_declaration_index_in_region_by_nature(tlex_index, current_region, nature_filter);
        if (index != NULL_VALUE) return index;
    }

    return NULL_VALUE;
}

int find_declaration_index(int tlex_index) {
    return find_declaration_in_stack(tlex_index, NULL_VALUE);
}

int find_declaration_index_by_nature(int tlex_index, Nature nature) {
    return find_declaration_in_stack(tlex_index, nature);
}

int find_function_index_by_region(int region_index) {
    Declaration *declaration_table = get_declaration_table();
    int index = 4; // Start at the first entry in the declaration table

    // TODO: optimize this search
    while (index != MAX_DECLARATION_COUNT) {
        if (declaration_table[index].nature == TYPE_FUNC && 
            declaration_table[index].execution == region_index) {
            return index; // Found the function with the matching region
        }

        index++;
    }

    return NULL_VALUE;
}
