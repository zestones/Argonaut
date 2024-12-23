#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../interpreter/expression/expression.h"

#include "../../stack_management/stack_management.h"
#include "../execution.h"

#include "address_calculation.h"


int get_variable_address(int index_declaration) {
    int region_declaration = get_declaration_region(index_declaration);
    int nis_declaration    = get_region_nis(region_declaration);
    int nis_utilisation    = get_region_nis(peek_region());
    int execution_offset   = get_declaration_execution(index_declaration);

    // BC + nis_utilisation - nis_declaration + execution_offset
    return nis_utilisation - nis_declaration + execution_offset;
}