#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../interpreter/expression/expression.h"

#include "../../stack_management/stack_management.h"
#include "../execution.h"

#include "address_calculation.h"

int get_parameter_address(int index_declaration) {
    int region_declaration = get_declaration_region(index_declaration); 
    int nis_declaration = get_region_nis(region_declaration);
    int execution_offset = get_declaration_execution(index_declaration);

    return execution_offset - (nis_declaration + 1);
}

int get_variable_address(int index_declaration) {
    int region_declaration = get_declaration_region(index_declaration);
    int execution_offset = get_declaration_execution(index_declaration);
    int nis_declaration = get_region_nis(region_declaration);

    if (region_declaration == 0) {
        return execution_offset;
    } else {
        return execution_offset - (nis_declaration + 1);
    }

}