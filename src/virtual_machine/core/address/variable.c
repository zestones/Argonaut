#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../interpreter/expression/expression.h"

#include "../../stack_management/stack_management.h"
#include "../execution.h"

#include "address_calculation.h"


int get_variable_address(int index_declaration) {
    int region_declaration = get_declaration_region(index_declaration);
    int execution_offset = get_declaration_execution(index_declaration);
    int offset = (region_declaration > 0) ? 1 : 0;

    return execution_offset - (region_declaration + offset);
}