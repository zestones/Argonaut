#ifndef HASH_H
#define HASH_H

#define HASH_TABLE_SIZE 32    


/**
 * @brief Hash function to generate a hash code for a given lexeme.
 * 
 * @param lexeme : The lexeme to hash.
 * @param length : The length of the lexeme.
 * @return int : The hash code.
 */
int hash_function(const char* lexeme, int length);

#endif // HASH_H
