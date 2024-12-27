#include "parser.h"
#include "../bin/y.tab.h"

/**
 * @brief Writes all key data tables to the specified output stream.
 * 
 * This function is responsible for printing the contents of various compiler/interpreter tables,
 * such as the lexeme table, hash table, declaration table, representation table, and region table.
 * It ensures that the program's internal data structures are accessible for debugging or logging.
 * 
 * @param out A pointer to the `FILE` stream where the tables will be written.
 */
static void write_tables_to(FILE* out) {
    fprintf_lexeme_table(out);
    fprintf_hash_table(out);
    
    fprintf_declaration_table(out);
    fprintf_representation_table(out);

    fprintf_region_table(out);
}

/**
 * @brief Initializes base types in the lexicographic and declaration table.
 * 
 * This function defines fundamental types (`int`, `float`, `bool`, `char`) in the lexeme and 
 * declaration tables. These types are crucial for enabling type-checking and other operations 
 * within the language. Additionally, it starts the first region for declarations.
 */
static void initialize_base_types() {
    int lexicographic_index = 0;

    lexicographic_index = insert_lexeme("int");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("float");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("bool");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("char");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    start_region();
}

/**
 * @brief Initializes all required tables for the interpreter or compiler.
 * 
 * Depending on the mode of execution (compilation or interpretation), this function sets up
 * the hash table, lexeme table, declaration table, representation table, and region table. 
 * In compilation mode, it also initializes the base types.
 * 
 * @param m The mode of operation (e.g., COMPILATION or INTERPRETATION).
 */
static void initialize_tables(Mode m) {
    init_hash_table();
    init_lexeme_table();
    
    init_declaration_table();
    init_representation_table();

    init_region_table();

    if (m == COMPILATION) initialize_base_types();
}

void ylog(int log) {
    if (!log) return;
    const char *filename = "log.txt";
    FILE *output = fopen(filename, "w");
    if (!output) {
        fprintf(stderr, "Error opening log file %s\n", filename);
        return;
    }

    fprintf(stdout, COLOR_GREEN "Logging enabled. Exporting tables to %s\n" COLOR_RESET, filename);
    write_tables_to(output);
    fclose(output);
}

void ydebug(int debug) {
    if (!debug) return;
    fprintf(stdout, COLOR_GREEN "Verbose mode enabled. Printing tables and ast...\n" COLOR_RESET);
    write_tables_to(stdout);
}

int yyrun(Mode m) {
    initialize_tables(m);
    int parse_result = yyparse(); 

    if (m == INTERPRETATION) interpret();
    return parse_result;
}
