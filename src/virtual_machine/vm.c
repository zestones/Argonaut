#include "vm.h"

#include "../ir/text_ir.h"
#include "../utils/errors.h"
#include "interpreter/interpreter.h"
#include "ir_validation.h"
#include "stack_management/stack_management.h"

extern FILE *yyin;
int yyparse(void);

void execute_ir(FILE *input, const char *input_name) {
    initialize_text_ir();
    initialize_diagnostic_state(input_name);
    yyin = input;
    yyparse();
    validate_loaded_ir();
    interpret();
}

void fprintf_vm_state(FILE *out) {
    fprintf_text_ir_state(out);
    fprintf_vm_stack(out);
}
