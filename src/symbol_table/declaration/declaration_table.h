/**
 * @file declaration_table.h
 * @brief Header file for managing a table of declarations in a compiler.
 * 
 * This file contains the definitions and function prototypes for managing
 * a table of declarations, which includes various types such as variables,
 * structures, arrays, procedures, functions, and parameters.
 */

#ifndef DECLARATION_TABLE_H
#define DECLARATION_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_DECLARATION_COUNT 1000 // Maximum number of declarations allowed in the table

/**
 * @enum Nature
 * @brief Enumeration for the nature of a declaration. The nature of a declaration
 * determines the type of the declaration, such as a variable, structure, array,
 * procedure, function, or parameter.
 */
typedef enum
{
    TYPE_BASE,   // Base type
    TYPE_VAR,    // Variable type
    TYPE_STRUCT, // Structure type
    TYPE_ARRAY,  // Array type
    TYPE_PROC,   // Procedure type
    TYPE_FUNC,   // Function type
    TYPE_PARAM   // Parameter type
} Nature;

/**
 * @struct Declaration
 * @brief Structure representing a declaration. A declaration includes information
 * about its nature, the index of the next declaration, the region of the declaration,
 * the description of the declaration, and the execution information of the declaration.
 */
typedef struct {
    Nature nature;      // Nature of the declaration
    int next;           // Index of the next declaration
    int region;         // Region of the declaration
    int description;    // Description of the declaration
    int execution;      // Execution information of the declaration
} Declaration;

/**
 * @brief Initializes the declaration table.
 */
void init_declaration_table();

/**
 * @brief Inserts a declaration into the table.
 * 
 * @param index Index at which to insert the declaration.
 * @param nature Nature of the declaration.
 * @param region Region of the declaration.
 * @param description Description of the declaration.
 * @param execution Execution information of the declaration.
 */
void insert_declaration(int index, Nature nature, int region, int description, int execution);

/**
 * @brief Inserts a variable declaration into the table.
 * 
 * @param index Index at which to insert the declaration.
 * @param region Region of the declaration.
 * @param description Description of the declaration.
 * @param execution Execution information of the declaration.
 */
void insert_declaration_var(int index, int region, int description, int execution);

/**
 * @brief Inserts an array declaration into the table.
 * 
 * @param index Index at which to insert the declaration.
 * @param region Region of the declaration.
 * @param description Description of the declaration.
 */
void insert_declaration_array(int index, int region, int description);

/**
 * @brief Inserts a structure declaration into the table.
 * 
 * @param index Index at which to insert the declaration.
 * @param region Region of the declaration.
 * @param description Description of the declaration.
 */
void insert_declaration_struct(int index, int region, int description);

/**
 * @brief Inserts a function declaration into the table.
 * 
 * @param index Index at which to insert the declaration.
 * @param region Region of the declaration.
 * @param description Description of the declaration.
 */
void insert_declaration_func(int index, int region, int description);

/**
 * @brief Inserts a procedure declaration into the table.
 * 
 * @param index Index at which to insert the declaration.
 * @param region Region of the declaration.
 * @param description Description of the declaration.
 */
void insert_declaration_proc(int index, int region, int description);

/**
 * @brief Inserts a parameter declaration into the table.
 * 
 * @param index Index at which to insert the declaration.
 * @param region Region of the declaration.
 * @param description Description of the declaration.
 * @param execution Execution information of the declaration.
 */
void insert_declaration_param(int index, int region, int description, int execution);

/**
 * @brief Inserts a declaration row into the table.
 * 
 * @param index Index at which to insert the declaration.
 * @param nature Nature of the declaration.
 * @param next Index of the next declaration.
 * @param region Region of the declaration.
 * @param description Description of the declaration.
 * @param execution Execution information of the declaration.
 */
void insert_declaration_row(int index, Nature nature, int next, int region, int description, int execution);

/**
 * @brief Finds the index of a declaration with the specified lexicographic index.
 * This function is used to ensure that a declaration with the specified lexicographic
 * index exists in the table.
 * 
 * If the declaration does not exist, it means that the declaration is using an undeclared
 * variable, type, which is an error.
 * 
 * The search is performed by exploring the stack of regions and checking each region for
 * the declaration.
 * 
 * @param index Index to search for.
 * @return Index of the declaration if found, otherwise -1.
 */
int find_declaration_index(int index);

/**
 * @brief Finds the index of a declaration with the specified lexicographic index and nature.
 * This function is used to ensure that a declaration with the specified lexicographic
 * index and nature exists in the table.
 * 
 * If the declaration does not exist, it means that the declaration is using an undeclared
 * variable, type, which is an error.
 * 
 * The search is performed by exploring the stack of regions and checking each region for
 * the declaration.
 * 
 * @param index Index to search for.
 * @param nature Nature of the declaration.
 * @return Index of the declaration if found, otherwise -1.
 */
int find_declaration_index_by_nature(int tlex_index, Nature nature);

/**
 * @brief Gets the nature of a declaration.
 * 
 * @param index Index of the declaration.
 * @return Nature of the declaration.
 */
int get_declaration_nature(int index);

/**
 * @brief Gets the description of a declaration.
 * 
 * @param index Index of the declaration.
 * @return Description of the declaration.
 */
int get_declaration_description(int index);

/**
 * @brief Gets the region of a declaration.
 * 
 * @param index Index of the declaration.
 * @return Region of the declaration.
 */
int get_declaration_region(int index);

/**
 * @brief Gets the execution information of a declaration.
 * 
 * @param index Index of the declaration.
 * @return Execution information of the declaration.
 */
int get_declaration_execution(int index);

/**
 * @brief Gets the lexicographic index of a declaration.
 * The objective is to find the first declaration in the chain of declarations
 * that is the genitor of the declaration. Because the first declaration index in the
 * chain is linked to the lexicographic index, this way we are able to find the lexeme of 
 * of type that is inside the overflown zone.
 * 
 * Why do we need this? It is used only to further improve the error messages.
 * A declaration of variable can have a type that is stored in the overflown zone.
 * Making it impossible to get the lexeme out of the declaration index. So we need to
 * find the first declaration in the chain that is linked to the lexeme.
 * 
 * @param index Index of the declaration.
 * @return Lexicographic index of the declaration.
 */
int get_declaration_lexicographic_index(int index);

/**
 * @brief Gets the declaration table.
 * 
 * @return Pointer to the declaration table.
 */
Declaration *get_declaration_table();

/**
 * @brief Updates the execution information of a declaration.
 * 
 * @param index Index of the declaration.
 * @param execution New execution information.
 */
void update_declaration_execution(int index, int execution);

/**
 * @brief Exports the declaration table to a file.
 * 
 * @param filename Name of the file to write the declaration table to.
 */
void export_declaration_table(const char* filename);

/**
 * @brief Prints the declaration table.
 * 
 * @param out File stream to write the declaration table to.
 */
void fprintf_declaration_table(FILE* out);

/**
 * @brief Converts a nature to a string representation.
 * 
 * @param nature Nature to convert.
 * @return String representation of the nature.
 */
static inline char *nature_to_string(Nature nature)
{
    switch (nature)
    {
        case TYPE_BASE: return "TYPE_BASE";
        case TYPE_VAR:  return "TYPE_VAR";
        case TYPE_STRUCT: return "TYPE_STRUCT";
        case TYPE_ARRAY: return "TYPE_ARRAY";
        case TYPE_PROC: return "TYPE_PROC";
        case TYPE_FUNC: return "TYPE_FUNC";
        case TYPE_PARAM: return "TYPE_PARAM";

        default: return "UNKNOWN";
    }
}

static inline char *nature_to_string_short(Nature nature)
{
    switch (nature)
    {
        case TYPE_BASE: return "base";
        case TYPE_VAR:  return "variable";
        case TYPE_STRUCT: return "structure";
        case TYPE_ARRAY: return "array";
        case TYPE_PROC: return "procedure";
        case TYPE_FUNC: return "function";
        case TYPE_PARAM: return "parameter";

        default: return "UNKNOWN";
    }
}

/**
 * @brief Checks if a declaration is a base type.
 * 
 * @param index Index of the declaration.
 * @return 1 if the declaration is a base type, otherwise 0.
 */
int is_declaration_base_type(int index);

#endif // DECLARATION_TABLE_H
