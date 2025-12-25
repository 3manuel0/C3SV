#ifndef CSV_H
#define CSV_H
#include "lib3man.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { str_ = 1, float_, int_, boolean_ } data_types;

typedef struct {
  char **head;
  data_types *types;
  void **data;
  size_t numrows;
  size_t numcols;
} CSV;

CSV *load_csv(char *file_name);

void csv_to_memory(u8 *mem, FILE *file, size_t size);

void print_type(data_types t);

data_types get_type(char *s);

#endif