#ifndef LEXEME_TABLE_H
#define LEXEME_TABLE_H


#include "../utils/hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LEXEME_LENGTH 100    

typedef struct Lexeme {
    char lexeme[MAX_LEXEME_LENGTH];
    int length;                      
    int lexicographic_code;          
    struct Lexeme *next;             
} Lexeme;

void init_lexeme_table();

int insert_lexeme(const char* lexeme);

Lexeme* find_lexeme(const char* lexeme);

void print_lexeme_table();

void free_lexeme_table();

#endif // LEXEME_TABLE_H
