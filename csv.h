#ifndef CSV
#define CSV
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef enum{
    str_,
    float_,
    int_,
    boolean_
}data_types;


typedef struct csv_data{
    char ** head ;
    data_types * types;
    void *** data;
    size_t numrows;
    size_t numcols;  
}csv_data;


csv_data * load_csv_data(char * file_name);

char * load_csv_file_to_memory(csv_data * csv_, FILE * f, size_t  * byte_len);

void parse_param(csv_data * csv_, char * param, size_t * current_index);

#endif