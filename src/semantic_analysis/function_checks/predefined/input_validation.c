#include "../common/format_validation.h"
#include "predefined.h"

void check_input_proc_argument_list(Node *format_string, Node *argument_list) {
    validate_format_arguments(format_string, argument_list, INPUT_FORMAT_ARGUMENTS);
}
