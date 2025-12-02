#ifndef CSV
#define CSV
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef float f32;
typedef double f64;

typedef enum { str_ = 1, float_, int_, boolean_ } data_types;

typedef struct {
  char *str;
  size_t len;
} str_t;

typedef union {
  long int i;
  double f;
  char *s;
  bool b;
} data_u;

typedef struct csv_data {
  char **head;
  data_types *types;
  data_u **data;
  size_t numrows;
  size_t numcols;
} csv_data;

csv_data *load_csv_data(char *file_name);

i8 *load_csv_file_to_memory(csv_data *csv_, FILE *f, size_t *byte_len);

void parse_param(csv_data *csv_, char *param, size_t *current_index);

void append_strs(str_t *a, str_t *b);

str_t str_t_from_const(const char *s);

void print_type(data_types t);

data_types get_type(char *s);

#endif