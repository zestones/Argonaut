#ifndef REPRESENTATION_TABLE_H
#define REPRESENTATION_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REPRESENTATION_COUNT 1000

void init_representation_table();

int insert_representation(int value);

void update_representation(int index, int value);

int get_representation_value(int index);

void print_representation_table();

#endif // REPRESENTATION_TABLE_H
