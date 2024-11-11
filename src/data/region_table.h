#ifndef __REGION_TABLE_H__
#define __REGION_TABLE_H__

#include <stdlib.h>
#include <stdio.h>

#include "stack_region.h"


#define MAX_REGION_COUNT 50

typedef struct Region {
    int size;
    int nis;
    int ast; // TODO: Abstract Syntax Tree
} Region;

Region construct_region(int size, int nis);

void init_region_table();

void start_region();

void end_region();


int get_region_size(int index);

int get_region_nis();

void update_region_size(int index, int size);

int get_current_region_index();


void print_region_table();


#endif // __REGION_TABLE_H__

