#include "hash.h"

int hash_function(const char* lexeme, int length) {
    int hash = 0;
    for (int i = 0; i < length; i++) {
        hash += lexeme[i];
    }

    return hash % HASH_TABLE_SIZE;
}
