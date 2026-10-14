#include "text_ir.h"

#include "../data/region_table.h"
#include "../symbol_table/declaration/declaration_table.h"
#include "../symbol_table/hash/hash_table.h"
#include "../symbol_table/lexeme/lexeme_table.h"
#include "../symbol_table/representation/representation_table.h"
#include "../utils/ir_version.h"

void initialize_text_ir(void) {
    init_hash_table();
    init_lexeme_table();
    init_declaration_table();
    init_representation_table();
    init_region_table();
}

void write_text_ir(FILE *out) {
    fprintf(out, "ARGONAUT_IR %d\n", ARGONAUT_IR_VERSION);
    write_lexeme_table(out);
    write_declaration_table(out);
    write_representation_table(out);
    write_region_table(out);
}

void fprintf_text_ir_state(FILE *out) {
    fprintf_lexeme_table(out);
    fprintf_hash_table(out);
    fprintf_declaration_table(out);
    fprintf_representation_table(out);
    fprintf_region_table(out);
}
