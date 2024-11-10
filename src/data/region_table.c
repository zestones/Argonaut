#include "../../lib/table_printer.h"
#include "../../lib/colors.h" 

#include "../utils/utils.h"
#include "region_table.h"

static Region region_table[MAX_REGION_COUNT];
static int region_table_size = 0;

Region construct_region(int size, int nis) {
    Region new_region;

    new_region.size = size;
    new_region.nis = nis;

    return new_region;
}

void init_region_table() {
    memset(region_table, NULL_VALUE, sizeof(region_table));
}

void insert_region(int nis) {
    if (region_table_size >= MAX_REGION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Region table is full\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    region_table[region_table_size] = construct_region(NULL_VALUE, nis);
    region_table_size++;
}