#include "../../lib/table_printer.h"
#include "../../lib/colors.h" 

#include "../symbol_table/representation_table.h"
#include "../symbol_table/declaration_table.h"
#include "func_proc_manager.h"
#include "../utils/utils.h"

static func_proc_manager_context context;

void declaration_proc_start() {
    context.index_number_of_parameters_representation = insert_representation(context.number_of_parameters);
    insert_declaration_func(context.index_func_proc_name_lexicographic, context.index_number_of_parameters_representation);
}

void declaration_proc_end() {
    update_representation(context.index_number_of_parameters_representation, context.number_of_parameters);
}