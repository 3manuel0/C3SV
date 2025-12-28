#ifndef CSV_H
#define CSV_H
#include "lib3man.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { string_ = 1, float64_, int64_, boolean_ } data_types;

typedef struct {
  string *head;
  data_types *types;
  string **data;
  size_t numrows;
  size_t numcols;
  ArenaList *gl_arena;
  ArenaList *gl_arena_head;
} CSV;

CSV *load_csv(char *file_name);

CSV *create_csv();

void csv_to_memory(u8 *mem, FILE *file, size_t size, size_t *numrows,
                   size_t *numcolumns);

int csv_parce_head(CSV *csv, u8 *mem);

u8 *csv_parce_column(ArenaList *arena, string *csv_column, u8 *mem);

int csv_parse(CSV *csv, u8 *mem);

void csv_free(CSV *csv);

data_types get_type(char *s);

void csv_print_head(CSV *csv);

void csv_print_column(string *column, size_t numrows);

void print_type(data_types t);

#endif