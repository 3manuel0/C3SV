#ifndef CSV_H
#define CSV_H
#include "lib3man.h"

typedef enum { string_ = 1, float64_, int64_, boolean_ } data_types;

typedef struct {
  string_view *head;
  data_types *types;
  void **data; // maybe void * is better for infering the types later
  size_t numrows;
  size_t numcols;
  ArenaList *gl_arena;
  ArenaList *gl_arena_head;
} CSV;

CSV *load_csv(char *file_name);

CSV *create_csv();

void csv_free(CSV *csv);

void csv_print_head(const CSV *csv);

void csv_print_row(const void *row, data_types * row_types, size_t numcolumns);

void csv_print_types(const CSV *csv);

void csv_print_column_from_string(const CSV *csv, string_view column_name);

void csv_write_file(const char *filename, const CSV *csv);

ssize_t csv_get_column_index(const CSV *csv, string_view name); // returns -1 if it fails

void print_type(data_types t);

#endif