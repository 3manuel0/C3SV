#include "includes/csv.h"
#include "includes/lib3man.h"
#include <stddef.h>

int main(){
    CSV *csv = load_csv("customers-100.csv");
    // CSV *csv = load_csv("test.csv");
    if(csv == NULL){
        printf("failed to read csv\n");
        return 1;
    }
    // free_csv(csv);
    csv_print_head(csv);
    // csv_print_row(csv->data[5], csv->numcols);
    for(size_t i = 0; i < csv->numrows; i++){
        csv_print_row(csv->data[i], csv->numcols);
    }
    
    printf("numcols: %zu  %zu\n", csv->numcols, csv->numrows);
    sv to_look_for = sv_from_lit("Phone 2");
    csv_print_column_from_string(csv , to_look_for);

    // changing a column name 
    sv t = sv_from_lit("Testing");
    csv->head[2] = t;

    // writing the file
    csv_write_file("out.csv", csv);

    // writing all the types
    for(size_t i = 0; i < csv->numcols; i++)
        print_type(csv->types[i]);

    csv_free(csv);
    return 0;
}

