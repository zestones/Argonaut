#ifndef DECLARATION_TABLE_H
#define DECLARATION_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_DECLARATION_COUNT 1000

typedef enum
{
    TYPE_BASE,
    TYPE_VAR,
    TYPE_STRUCT,
    TYPE_ARRAY,
    TYPE_PROC,
    TYPE_FUNC
} Nature;

typedef struct {
    Nature nature;
    int next;             
    int region;
    int description;
    int execution;
} Declaration;


void init_declaration_table();

void insert_declaration(int index, Nature nature, int region, int description, int execution);

void insert_declaration_var(int lexeme_lexicographic_index, int type_lexicographic_index);

void insert_declaration_struct(int lexeme_lexicographic_index, int type_lexicographic_index);

void print_declaration_table();

static inline char *nature_to_string(Nature nature)
{
    switch (nature)
    {
    case TYPE_BASE:
        return "TYPE_BASE";
    case TYPE_VAR:
        return "TYPE_VAR";
    case TYPE_STRUCT:
        return "TYPE_STRUCT";
    case TYPE_ARRAY:
        return "TYPE_ARRAY";
    default:
        return "UNKNOWN";
    }
}

#endif // DECLARATION_TABLE_H
