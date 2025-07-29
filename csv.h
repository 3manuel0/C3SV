#ifndef CSV
#define CSV
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum{
    str_,
    float_,
    int_
}data_types;


typedef struct csv_data{
    char ** head ;
    data_types * types;
    void ** data;
    size_t numrows;
    size_t numcols;  
}csv_data;


csv_data * load_csv(char * file_name);

#endif