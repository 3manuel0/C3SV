#ifndef CSV_H
#define CSV_H
#include "lib3man.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { str_ = 1, float_, int_, boolean_ } data_types;

typedef struct {
  string *head;
  data_types *types;
  void **data;
  size_t numrows;
  size_t numcols;
  ArenaList *gl_arena;
  ArenaList *gl_arena_head;
} CSV;

CSV *load_csv(char *file_name);

CSV *create_csv();

void csv_to_memory(u8 *mem, FILE *file, size_t size, size_t *numrows,
                   size_t *numcolumns);

int csv_create_header(CSV *csv, u8 *mem);

void print_type(data_types t);

void csv_print_head(CSV *csv);

void csv_free(CSV *csv);

data_types get_type(char *s);

#endif