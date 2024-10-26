#ifndef LEXEME_TABLE_H
#define LEXEME_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LEXEME_LENGTH 100    
#define MAX_LEXEME_COUNT 500

typedef struct {
    char lexeme[MAX_LEXEME_LENGTH];
    int length;                      
    int next;             
} Lexeme;

void init_lexeme_table();

int insert_lexeme(const char* lexeme);

void print_lexeme_table();

#endif // LEXEME_TABLE_H
