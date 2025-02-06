/**
 * @file lexeme_table.h
 * @brief Header file for managing a table of lexemes.
 *
 * This file contains the definitions and function declarations for handling
 * a table of lexemes, which are sequences of characters representing tokens
 * in a lexical analysis process.
 */

#ifndef LEXEME_TABLE_H
#define LEXEME_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LEXEME_LENGTH 100    // Maximum length of a lexeme.
#define MAX_LEXEME_COUNT 500     // Maximum number of lexemes in the table.


/**
 * @struct Lexeme
 * @brief Represents a lexical token with its attributes.
 * 
 * The `Lexeme` struct is used to store information about a lexical token,
 * including its text representation (lexeme), its length, and a reference to the
 * next lexeme or position in a list or table.
 */
typedef struct {
    char lexeme[MAX_LEXEME_LENGTH];  // The character array holding the lexeme's text.
    int length;                      // The length of the lexeme.
    int next;                        // The index of the next lexeme that has the same hash code.
} Lexeme;

/**
 * @brief Constructs a new lexeme with the given character sequence, length, and next index.
 *
 * @param lexeme The character sequence of the lexeme.
 * @param length The length of the lexeme.
 * @param next The index of the next lexeme in the table.
 * @return A new Lexeme object.
 */
Lexeme construct_lexeme(const char* lexeme, int length, int next);

/**
 * @brief Initializes the lexeme table.
 *
 * This function sets up the lexeme table, preparing it for the insertion
 * of lexemes.
 */
void init_lexeme_table();

/**
 * @brief Inserts a lexeme into the lexeme table.
 *
 * This function adds a new lexeme to the lexeme table.
 *
 * @param lexeme The lexeme to be inserted.
 * @return The index of the inserted lexeme, or -1 if the insertion fails.
 */
int insert_lexeme(const char* lexeme);

/**
 * @brief Inserts a lexeme row into the lexeme table.
 *
 * This function adds a new lexeme to the lexeme table at the specified index.
 *
 * @param index The index at which to insert the lexeme.
 * @param lexeme The lexeme to be inserted.
 * @param length The length of the lexeme.
 * @param next The index of the next lexeme in the table.
 */
void insert_lexeme_row(int index, const char* lexeme, int length, int next);

/**
 * @brief Retrieves the lexeme at the given index.
 *
 * This function returns the lexeme stored at the specified index in the lexeme table.
 *
 * @param index The index of the lexeme to retrieve.
 * @return The lexeme at the given index.
 */
char *get_lexeme(int index);

/**
 * @brief Exports the lexeme table to a file.
 *
 * This function writes the contents of the lexeme table to a file.
 *
 * @param filename The name of the file to write the lexeme table to.
 */
void export_lexeme_table(const char* filename);

/**
 * @brief Prints the contents of the lexeme table.
 *
 * This function outputs the current state of the lexeme table, displaying
 * all stored lexemes and their associated information.
 */
void fprintf_lexeme_table(FILE* out);

#endif // LEXEME_TABLE_H
