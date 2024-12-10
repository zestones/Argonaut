#ifndef __NODE_TYPE_H__
#define __NODE_TYPE_H__

// Define the node types
#define FOREACH_NODE_TYPE(NODE)             \
    NODE(A_INTEGER_LITERAL)                 \
    NODE(A_FLOAT_LITERAL)                   \
    NODE(A_BOOLEAN_LITERAL)                 \
    NODE(A_CHARACTER_LITERAL)               \
    NODE(A_STRING_LITERAL)                  \
    NODE(A_FORMAT_STRING)                   \
    NODE(A_PROGRAM)                         \
    NODE(A_CONDITION)                       \
    NODE(A_AND_CONDITION)                   \
    NODE(A_OR_CONDITION)                    \
    NODE(A_NOT_CONDITION)                   \
    NODE(A_NOT_EXPRESSION)                  \
    NODE(A_EQUAL_OP)                        \
    NODE(A_NOT_EQUAL_OP)                    \
    NODE(A_LESS_THAN_OP)                    \
    NODE(A_GREATER_THAN_OP)                 \
    NODE(A_LESS_EQUAL_OP)                   \
    NODE(A_GREATER_EQUAL_OP)                \
    NODE(A_ADD_OP)                          \
    NODE(A_SUB_OP)                          \
    NODE(A_MUL_OP)                          \
    NODE(A_DIV_OP)                          \
    NODE(A_DECLARATION_LIST)                \
    NODE(A_VARIABLE_DECLARATION)            \
    NODE(A_FUNCTION_DECLARATION)            \
    NODE(A_PROCEDURE_DECLARATION)           \
    NODE(A_TYPE_DECLARATION)                \
    NODE(A_TYPE_ARRAY_DECLARATION)          \
    NODE(A_TYPE_STRUCT_DECLARATION)         \
    NODE(A_STRUCT_FIELD_LIST)               \
    NODE(A_STRUCT_FIELD)                    \
    NODE(A_IDENTIFIER)                      \
    NODE(A_STATEMENT_LIST)                  \
    NODE(A_ASSIGNMENT_STATEMENT)            \
    NODE(A_LOOP_STATEMENT)                  \
    NODE(A_FUNC_PROC_CALL_STATEMENT)        \
    NODE(A_RETURN_STATEMENT)                \
    NODE(A_VARIABLE_ASSIGNMENT)             \
    NODE(A_ARRAY_ASSIGNMENT)                \
    NODE(A_STRUCT_ASSIGNMENT)               \
    NODE(A_IF)                              \
    NODE(A_IF_ELSE)                         \
    NODE(A_WHILE)                           \
    NODE(A_ARRAY_ACCESS)                    \
    NODE(A_ARRAY_INDEX_LIST)                \
    NODE(A_ARRAY_INDEX)                     \
    NODE(A_STRUCT_FIELD_ACCESS)             \
    NODE(A_PRINT_STATEMENT)                 \
    NODE(A_INPUT_STATEMENT)                 \
    NODE(A_ARGUMENT_LIST)                   \
    NODE(A_ARGUMENT)                        \
    NODE(A_PARAMETER_LIST)                  \
    NODE(A_PARAMETER)

// Generate the enum values
#define GENERATE_ENUM(ENUM) ENUM,
typedef enum {
    FOREACH_NODE_TYPE(GENERATE_ENUM)
} NodeType;
#undef GENERATE_ENUM

// Generate the string representations
#define GENERATE_STRING(STRING) #STRING,
static const char *NodeTypeStrings[] = {
    FOREACH_NODE_TYPE(GENERATE_STRING)
};
#undef GENERATE_STRING

#endif // __NODE_TYPE_H__
