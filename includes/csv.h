#ifndef CSV_H
#define CSV_H
#include "lib3man.h"
#include <stddef.h>

#ifdef _WIN32
    #include <BaseTsd.h>
    typedef SSIZE_T ssize_t;
#else
    #include <unistd.h>
#endif

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

CSV *load_csv(char *file_name);// load with types (only int and float 64bit)

CSV *create_csv(); // creates an empty csv struct in memory

void csv_free(CSV *csv); // free csv in memory

void csv_print_head(const CSV *csv);// prints head

void csv_print_row(const void *row, data_types * row_types, size_t numcolumns);

void csv_print_types(const CSV *csv);

void csv_print_column_from_string(const CSV *csv, string_view column_name);

void csv_write_file(const char *filename, const CSV *csv); // write a csv file 

ssize_t csv_get_column_index(const CSV *csv, string_view name); // returns -1 if it doesn't find the column name

i32 csv_write_json(const CSV *csv, const char *filename);// writes a json files from a csv in memory

i64 csv_get_int_by_name(const CSV *csv, size_t row, string_view col_name);

f64 csv_get_float_by_name(const CSV *csv,size_t row, string_view col_name);

string_view csv_get_sv_by_name(const CSV *csv, size_t row, string_view col_name);

void print_type(data_types t);

#endif