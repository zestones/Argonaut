#ifndef DECLARATION_TABLE_H
#define DECLARATION_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DECLARATION_COUNT 1000 
#define MAX_NATURE_LENGTH 50

typedef struct {
    char nature[MAX_NATURE_LENGTH];                      
    int next;             
    int region;
    int description;
    int execution;
} Declaration;

void init_declaration_table();

void insert_declaration();

void print_declaration_table();

#endif // DECLARATION_TABLE_H
