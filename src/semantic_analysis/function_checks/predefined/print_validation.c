#include "../common/format_validation.h"
#include "predefined.h"

void check_print_proc_argument_list(Node *format_string, Node *argument_list) {
    validate_format_arguments(format_string, argument_list, PRINT_FORMAT_ARGUMENTS);
}
