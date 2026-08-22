#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../data/region_table.h"
#include "declaration_table.h"

#include "../../utils/utils.h"
#include "../../utils/stack.h"

static int find_declaration_index_in_region_by_nature(int tlex_index, int region, Nature nature_filter) {
    Declaration *declaration_table = get_declaration_table();
    int index = tlex_index;
    int last_match = NULL_VALUE;

    // Several declarations of the same name and nature may exist in the region (overloads):
    // the last one in the chain wins.
    while (index != NULL_VALUE) {
        if (declaration_table[index].region == region &&
            (nature_filter == (Nature) NULL_VALUE || declaration_table[index].nature == nature_filter)) {
            last_match = index;
        }

        index = declaration_table[index].next;
    }

    return last_match;
}

static int find_declaration_in_stack(int tlex_index, int nature_filter) {
    if (is_declaration_base_type(tlex_index)) return tlex_index;

    // Innermost region first: walk the region stack from its top.
    Stack region_stack = get_region_stack();
    for (int i = size(region_stack) - 1; i >= 0; i--) {
        int current_region = *(int *) get_stack_value(region_stack, i);
        int index = find_declaration_index_in_region_by_nature(tlex_index, current_region, nature_filter);
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
        if (declaration_table[index].nature == TYPE_FUNC && declaration_table[index].execution == region_index) {
            return index; // Found the function with the matching region
        }

        index++;
    }

    return NULL_VALUE;
}
