#ifndef DECLARATION_TABLE_H
#define DECLARATION_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_DECLARATION_COUNT 1000

typedef enum
{
    TYPE_B,
    TYPE_C,
    TYPE_D,
    TYPE_E
} Nature;

typedef struct {
    Nature nature;
    int next;             
    int region;
    int description;
    int execution;
} Declaration;


void init_declaration_table();

void insert_declaration(Nature nature, int next, int region, int description, int execution);

void print_declaration_table();

static inline char *nature_to_string(Nature nature)
{
    switch (nature)
    {
    case TYPE_B:
        return "TYPE_B";
    case TYPE_C:
        return "TYPE_C";
    case TYPE_D:
        return "TYPE_D";
    case TYPE_E:
        return "TYPE_E";
    default:
        return "UNKNOWN";
    }
}

#endif // DECLARATION_TABLE_H
