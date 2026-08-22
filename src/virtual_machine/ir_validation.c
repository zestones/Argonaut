#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../lib/colors.h"

#include "../ast/ast.h"
#include "../ast/node_type.h"
#include "../symbol_table/lexeme/lexeme_table.h"
#include "../symbol_table/declaration/declaration_table.h"
#include "../data/region_table.h"
#include "../utils/utils.h"

#include "ir_validation.h"

#define NODE_TYPE_COUNT ((int) (sizeof(NodeTypeStrings) / sizeof(NodeTypeStrings[0])))

static void reject(int region, const char *format, ...) __attribute__((format(printf, 2, 3), noreturn));

static void reject(int region, const char *format, ...) {
    va_list args;

    fprintf(stderr, COLOR_RED "Corrupted IR (region %d): ", region);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr,
            ".\n  The file was not produced by argoc, or was modified: it cannot be executed." COLOR_RESET "\n");

    exit(EXIT_FAILURE);
}

static const char *name_of(const Node *node) {
    if (node == NULL) return "nothing";
    if ((int) node->type < 0 || (int) node->type >= NODE_TYPE_COUNT) return "an unknown node type";
    return NodeTypeStrings[node->type];
}

/* ---- node categories, as the grammar produces them and as the VM consumes them ------------- */

static int is_statement(int type) {
    switch (type) {
        case A_STATEMENT_LIST:
        case A_ASSIGNMENT_STATEMENT:
        case A_IF:
        case A_IF_ELSE:
        case A_IF_ELSE_IF:
        case A_WHILE:
        case A_FOR_LOOP:
        case A_BREAK:
        case A_CONTINUE:
        case A_FUNC_PROC_CALL_STATEMENT:
        case A_RETURN_STATEMENT:
        case A_PRINT_STATEMENT:
        case A_INPUT_STATEMENT: return 1;
        default: return 0;
    }
}

static int is_comparison(int type);

static int is_expression(int type) {
    switch (type) {
        case A_INTEGER_LITERAL:
        case A_FLOAT_LITERAL:
        case A_BOOLEAN_LITERAL:
        case A_CHARACTER_LITERAL:
        case A_STRING_LITERAL:
        case A_IDENTIFIER:
        case A_ADD_OP:
        case A_SUB_OP:
        case A_MUL_OP:
        case A_DIV_OP:
        case A_MODULO_OP:
        case A_NEG_OP:
        case A_ARRAY_ACCESS:
        case A_STRUCT_FIELD_ACCESS:
        case A_FUNC_PROC_CALL_STATEMENT:
        case A_CONDITION:
        case A_AND_CONDITION:
        case A_OR_CONDITION:
        case A_NOT_CONDITION: return 1;
        default: return is_comparison(type);
    }
}

static int is_comparison(int type) {
    switch (type) {
        case A_EQUAL_OP:
        case A_NOT_EQUAL_OP:
        case A_LESS_THAN_OP:
        case A_GREATER_THAN_OP:
        case A_LESS_EQUAL_OP:
        case A_GREATER_EQUAL_OP: return 1;
        default: return 0;
    }
}

/* A condition is any expression: the compiler checks that its type is bool. */
static int is_condition(int type) {
    return is_expression(type);
}

static int is_assignment(int type) {
    return type == A_VARIABLE_ASSIGNMENT || type == A_ARRAY_ASSIGNMENT || type == A_STRUCT_ASSIGNMENT;
}

static int is_lvalue(int type) {
    return type == A_IDENTIFIER || type == A_ARRAY_ACCESS || type == A_STRUCT_FIELD_ACCESS;
}

static int is_arithmetic(int type) {
    return type == A_ADD_OP || type == A_SUB_OP || type == A_MUL_OP || type == A_DIV_OP || type == A_MODULO_OP;
}

static int is_if(int type) {
    return type == A_IF || type == A_IF_ELSE || type == A_IF_ELSE_IF;
}

/* ---- checks ------------------------------------------------------------------------------ */

static void expect(int region, const Node *parent, const Node *child, int ok, const char *what) {
    if (!ok) {
        reject(region, "node %s expects %s, found %s", name_of(parent), what, name_of(child));
    }
}

static void check_argument_list(int region, const Node *list, int lvalues_only) {
    for (const Node *argument = list->child; argument != NULL; argument = argument->sibling) {
        expect(region, list, argument, argument->type == A_ARGUMENT, "an argument");
        const Node *value = argument->child;
        if (lvalues_only) {
            expect(region, argument, value, value != NULL && is_lvalue((int) value->type),
                   "a variable, array element or field");
        } else {
            expect(region, argument, value, value != NULL && is_expression((int) value->type), "an expression");
        }
    }
}

static void check_shape(int region, const Node *node) {
    int type = (int) node->type;
    const Node *c1 = node->child;
    const Node *c2 = c1 ? c1->sibling : NULL;
    const Node *c3 = c2 ? c2->sibling : NULL;
    const Node *c4 = c3 ? c3->sibling : NULL;

    if (type == A_STATEMENT_LIST) {
        for (const Node *statement = c1; statement != NULL; statement = statement->sibling) {
            expect(region, node, statement, is_statement((int) statement->type), "a statement");
        }
    } else if (is_if(type)) {
        expect(region, node, c1, c1 != NULL && is_condition((int) c1->type), "a condition");
        expect(region, node, c2, c2 != NULL && c2->type == A_STATEMENT_LIST, "a block");
        if (type == A_IF_ELSE) expect(region, node, c3, c3 != NULL && c3->type == A_STATEMENT_LIST, "an else block");
        if (type == A_IF_ELSE_IF) expect(region, node, c3, c3 != NULL && is_if((int) c3->type), "an else-if");
    } else if (type == A_WHILE) {
        expect(region, node, c1, c1 != NULL && is_condition((int) c1->type), "a condition");
        expect(region, node, c2, c2 != NULL && c2->type == A_STATEMENT_LIST, "a block");
    } else if (type == A_FOR_LOOP) {
        expect(region, node, c1, c1 != NULL && c1->type == A_VARIABLE_ASSIGNMENT, "an initialisation");
        expect(region, node, c2, c2 != NULL && is_condition((int) c2->type), "a condition");
        expect(region, node, c3, c3 != NULL && c3->type == A_VARIABLE_ASSIGNMENT, "an update");
        expect(region, node, c4, c4 != NULL && c4->type == A_STATEMENT_LIST, "a block");
    } else if (type == A_ASSIGNMENT_STATEMENT) {
        expect(region, node, c1, c1 != NULL && is_assignment((int) c1->type), "an assignment");
    } else if (type == A_VARIABLE_ASSIGNMENT) {
        // With a declaration: the assignment itself, whose child is the value. Without one: the
        // wrapper the 'for' rule puts around its initialisation and update assignments.
        if (node->index_declaration != NULL_VALUE) {
            expect(region, node, c1, c1 != NULL && is_expression((int) c1->type), "an expression");
        } else {
            expect(region, node, c1, c1 != NULL && is_assignment((int) c1->type), "an assignment");
        }
    } else if (type == A_ARRAY_ASSIGNMENT) {
        expect(region, node, c1, c1 != NULL && c1->type == A_ARRAY_ACCESS, "an array access");
        expect(region, node, c2, c2 != NULL && is_expression((int) c2->type), "an expression");
    } else if (type == A_STRUCT_ASSIGNMENT) {
        expect(region, node, c1, c1 != NULL && c1->type == A_STRUCT_FIELD_ACCESS, "a field access");
        expect(region, node, c2, c2 != NULL && is_expression((int) c2->type), "an expression");
    } else if (is_arithmetic(type) || is_comparison(type)) {
        expect(region, node, c1, c1 != NULL && is_expression((int) c1->type), "a left operand");
        expect(region, node, c2, c2 != NULL && is_expression((int) c2->type), "a right operand");
    } else if (type == A_AND_CONDITION || type == A_OR_CONDITION) {
        expect(region, node, c1, c1 != NULL && is_expression((int) c1->type), "a left condition");
        expect(region, node, c2, c2 != NULL && is_expression((int) c2->type), "a right condition");
    } else if (type == A_NOT_CONDITION || type == A_CONDITION) {
        expect(region, node, c1, c1 != NULL && is_condition((int) c1->type), "a condition");
    } else if (type == A_NEG_OP) {
        expect(region, node, c1, c1 != NULL && is_expression((int) c1->type), "an operand");
    } else if (type == A_RETURN_STATEMENT) {
        if (c1 != NULL) expect(region, node, c1, is_expression((int) c1->type), "an expression");
    } else if (type == A_ARRAY_ACCESS) {
        expect(region, node, c1, c1 != NULL && c1->type == A_ARRAY_INDEX_LIST && c1->child != NULL, "an index list");
        for (const Node *index = c1->child; index != NULL; index = index->sibling) {
            expect(region, c1, index, index->type == A_ARRAY_INDEX, "an index");
            expect(region, index, index->child, index->child != NULL && is_expression((int) index->child->type),
                   "an expression");
        }
        if (c2 != NULL)
            expect(region, node, c2, c2->type == A_STRUCT_FIELD_ACCESS || c2->type == A_ARRAY_ACCESS,
                   "a field or index access");
    } else if (type == A_STRUCT_FIELD_ACCESS) {
        if (c1 != NULL)
            expect(region, node, c1, c1->type == A_STRUCT_FIELD_ACCESS || c1->type == A_ARRAY_ACCESS,
                   "a field or array access");
    } else if (type == A_FUNC_PROC_CALL_STATEMENT) {
        if (c1 != NULL) {
            expect(region, node, c1, c1->type == A_ARGUMENT_LIST, "an argument list");
            check_argument_list(region, c1, 0);
        }
    } else if (type == A_PRINT_STATEMENT || type == A_INPUT_STATEMENT) {
        // [format] or [argument list, format]
        const Node *format = (c2 != NULL) ? c2 : c1;
        expect(region, node, format,
               format != NULL && format->type == A_FORMAT_STRING && format->index_lexicographic != NULL_VALUE,
               "a format string");
        if (c2 != NULL) {
            expect(region, node, c1, c1->type == A_ARGUMENT_LIST, "an argument list");
            check_argument_list(region, c1, type == A_INPUT_STATEMENT);
        }
    } else if (type == A_PROGRAM) {
        expect(region, node, c1, c1 != NULL && c1->type == A_DECLARATION_LIST, "a declaration list");
        if (c2 != NULL) expect(region, node, c2, c2->type == A_STATEMENT_LIST, "a statement list");
    } else if (type == A_FUNCTION_DECLARATION || type == A_PROCEDURE_DECLARATION) {
        // Any subset of [parameter list, declaration list, statement list], in that order.
        int stage = 0;
        for (const Node *part = c1; part != NULL; part = part->sibling) {
            int part_stage = part->type == A_PARAMETER_LIST     ? 1
                             : part->type == A_DECLARATION_LIST ? 2
                             : part->type == A_STATEMENT_LIST   ? 3
                                                                : 0;
            expect(region, node, part, part_stage > stage, "parameters, declarations then statements");
            stage = part_stage;
        }
    }
}

/**
 * @brief Node types whose declaration index is dereferenced by the VM and must therefore exist.
 */
static int node_requires_declaration(int type) {
    switch (type) {
        case A_IDENTIFIER:
        case A_VARIABLE_DECLARATION:
        case A_FUNC_PROC_CALL_STATEMENT:
        case A_PARAMETER:
        case A_FUNCTION_DECLARATION:
        case A_PROCEDURE_DECLARATION: return 1;
        default: return 0;
    }
}

static void validate_nodes(int region, const Node *node) {
    for (; node != NULL; node = node->sibling) {
        int type = (int) node->type;
        if (type < 0 || type >= NODE_TYPE_COUNT) {
            reject(region, "unknown node type %d", type);
        }

        const char *name = NodeTypeStrings[type];
        if (node->index_lexicographic != NULL_VALUE && !is_lexeme_loaded(node->index_lexicographic)) {
            reject(region, "node %s refers to lexeme %d, which does not exist", name, node->index_lexicographic);
        }

        int declaration = node->index_declaration;
        if (declaration != NULL_VALUE && !is_declaration_loaded(declaration)) {
            reject(region, "node %s refers to declaration %d, which does not exist", name, declaration);
        }
        if (declaration == NULL_VALUE && node_requires_declaration(type)) {
            reject(region, "node %s has no declaration", name);
        }

        if (type == A_FUNC_PROC_CALL_STATEMENT) {
            int nature = get_declaration_nature(declaration);
            if (nature != TYPE_FUNC && nature != TYPE_PROC) {
                reject(region, "call of declaration %d, which is not a function or a procedure", declaration);
            }
            int target = get_declaration_execution(declaration);
            if (!is_region_loaded(target)) {
                reject(region, "call of declaration %d targets region %d, which does not exist", declaration, target);
            }
        }

        // Every node type is checked before its children are, so check_shape can trust the
        // types it looks at; the children's own shape is checked when the walk reaches them.
        check_shape(region, node);
        validate_nodes(region, node->child);
    }
}

void validate_loaded_ir(void) {
    if (!is_region_loaded(0)) {
        reject(0, "region 0 (the program) is missing");
    }

    for (int region = 0; region < MAX_REGION_COUNT; region++) {
        if (!is_region_loaded(region)) continue;

        const Node *root = get_region_ast(region);
        if (root == NULL) {
            reject(region, "the region has no AST");
        }
        int expected = region == 0 ? A_PROGRAM : -1;
        if (expected == A_PROGRAM ? root->type != A_PROGRAM
                                  : (root->type != A_FUNCTION_DECLARATION && root->type != A_PROCEDURE_DECLARATION)) {
            reject(region, "the region root is %s", name_of(root));
        }
        validate_nodes(region, root);
    }
}
