#include "compiler.h"

#include "../data/region_table.h"
#include "../ir/text_ir.h"
#include "../symbol_table/declaration/declaration_table.h"
#include "../symbol_table/lexeme/lexeme_table.h"
#include "../utils/errors.h"

extern FILE *yyin;
int yyparse(void);

static void initialize_base_types(void) {
    int lexicographic_index = insert_lexeme("int");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("float");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("bool");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("char");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    start_region();
}

int compile_source(FILE *source, const char *source_name) {
    initialize_text_ir();
    initialize_base_types();
    initialize_diagnostic_state(source_name);
    yyin = source;
    yyparse();
    return get_error_count();
}

void write_compiled_ir(FILE *out) {
    write_text_ir(out);
}

void fprintf_compiler_state(FILE *out) {
    fprintf_text_ir_state(out);
}
