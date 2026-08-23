#include "../../symbol_table/representation/representation_table.h"
#include "../../symbol_table/declaration/declaration_table.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../symbol_table/utility.h"
#include "../../data/region_table.h"
#include "../format/formatting.h"
#include "../../utils/utils.h"
#include "type_inference.h"

static void access_error(Node *root, const char *format, const char *detail) {
    char *text = format_access(root);
    set_error_type(&error, SEMANTIC_ERROR);
    set_error_message(&error, format, get_formatted_location(), text, detail);
    yerror(error);
    free(text);
}

/**
 * @brief Applies the index list of an A_ARRAY_ACCESS node to a type: every dimension of the
 *        array consumes one index, and the remaining indices apply to the element when it is an
 *        array itself (`g[i, j]` on an array of arrays).
 *
 * @return The type of the designated element, or NULL_VALUE after an error.
 */
static int apply_indices(Node *access_node, int type, Node *root) {
    if (get_declaration_nature(type) != TYPE_ARRAY) {
        access_error(root,
                     "Array access error at %s.\n"
                     "  '%s' is indexed, but it is of type '%s', which is not an array.\n",
                     type_name(type));
        return NULL_VALUE;
    }

    Node *index = access_node->child->child;
    while (index != NULL) {
        if (get_declaration_nature(type) != TYPE_ARRAY) {
            access_error(root,
                         "Array access error at %s.\n"
                         "  Too many indices in '%s': the element of type '%s' is not an array.\n",
                         type_name(type));
            return NULL_VALUE;
        }

        int dimensions = get_array_dimension(type);
        for (int dim = 0; dim < dimensions; dim++) {
            if (index == NULL) {
                char expected[16];
                snprintf(expected, sizeof(expected), "%d", dimensions);
                access_error(root,
                             "Array access error at %s.\n"
                             "  Insufficient indices in '%s': the array has %s dimension(s).\n",
                             expected);
                return NULL_VALUE;
            }

            int index_type = resolve_expression_type(index->child);
            if (index_type == NULL_VALUE) return NULL_VALUE;
            if (index_type != A_INTEGER_LITERAL) {
                access_error(root,
                             "Array access error at %s.\n"
                             "  An index of '%s' has type '%s', but indices must be integers.\n",
                             type_name(index_type));
                return NULL_VALUE;
            }

            index = index->sibling;
        }

        type = get_array_element_type(type);
    }

    return type;
}

int resolve_access_type(Node *access) {
    int declaration = access->index_declaration;
    int nature = get_declaration_nature(declaration);

    if (nature != TYPE_VAR && nature != TYPE_PARAM) {
        // Undeclared, or declared as something else (a type, a function)
        check_variable_definition(access->index_lexicographic);
        if (find_declaration_index(access->index_lexicographic) != NULL_VALUE) {
            access_error(access,
                         "Access error at %s.\n"
                         "  '%s' is not a variable%s.\n",
                         "");
        }
        return NULL_VALUE;
    }

    int type = get_declaration_description(declaration);
    Node *node;
    if (access->type == A_ARRAY_ACCESS) {
        type = apply_indices(access, type, access);
        if (type == NULL_VALUE) return NULL_VALUE;
        node = access->child->sibling;
    } else {
        node = access->child;
    }

    while (node != NULL) {
        if (node->type == A_ARRAY_ACCESS && node->index_lexicographic == NULL_VALUE) {
            // g[i][j]: indices applied to an element which is an array
            type = apply_indices(node, type, access);
            if (type == NULL_VALUE) return NULL_VALUE;
            node = node->child->sibling;
            continue;
        }

        // A field of a structure
        if (get_declaration_nature(type) != TYPE_STRUCT) {
            access_error(access,
                         "Field access error at %s.\n"
                         "  '%s' accesses a field of a value of type '%s', which is not a struct.\n",
                         type_name(type));
            return NULL_VALUE;
        }

        int nth_field = find_struct_field_index_lexicographic(type, node->index_lexicographic);
        if (nth_field == NULL_VALUE) {
            char detail[256];
            snprintf(detail, sizeof(detail), "the field '%s' does not exist in the struct '%s'",
                     get_lexeme(node->index_lexicographic), get_lexeme(get_declaration_lexicographic_index(type)));
            access_error(access,
                         "Field access error at %s.\n"
                         "  In '%s', %s.\n",
                         detail);
            return NULL_VALUE;
        }

        type = get_struct_nth_field_declaration(type, nth_field);
        if (node->type == A_ARRAY_ACCESS) {
            type = apply_indices(node, type, access);
            if (type == NULL_VALUE) return NULL_VALUE;
            node = node->child->sibling;
        } else {
            node = node->child;
        }
    }

    return type;
}
