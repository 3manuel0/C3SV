#ifndef CSV_H
#define CSV_H
#include "lib3man.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { string_ = 1, float64_, int64_, boolean_ } data_types;

typedef struct {
  sv *head;
  data_types *types;
  void **data; // maybe void * is better for infering the types later
  size_t numrows;
  size_t numcols;
  ArenaList *gl_arena;
  ArenaList *gl_arena_head;
} CSV;

CSV *load_csv(char *file_name);

CSV *create_csv();

void csv_to_memory(u8 *mem, FILE *file, size_t size, size_t *numcolumns,
                   size_t *numrows);

int csv_parce_head(CSV *csv, u8 *mem);

u8 *csv_parce_row(ArenaList *arena, sv *csv_row, u8 *mem);

int csv_parse(CSV *csv, u8 *mem);

void csv_free(CSV *csv);

data_types get_type(char *s);

void csv_print_head(CSV *csv);

void csv_print_row(sv *row, size_t numcolumns);

void csv_print_column_from_string(CSV *csv, sv column_name);

size_t csv_get_column_index(CSV *csv, sv name, int *is_failed);

void print_type(data_types t);

#endif