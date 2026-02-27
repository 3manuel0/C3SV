#include "includes/csv.h"
#include "includes/lib3man.h"
#include <stddef.h>

int main(){
    // CSV *csv = load_csv("customers-100.csv");
    CSV *csv = load_csv("test1.csv");
    if(csv == NULL){
        printf("failed to read csv\n");
        return 1;
    }
    // free_csv(csv);

    csv_print_head(csv);
    // csv_print_row(csv->data[5], csv->numcols);
    for(size_t i = 0; i < csv->numrows; i++){
        csv_print_row(csv->data[i], csv->types,csv->numcols);
    }
    
    printf("numcols: %zu  %zu\n", csv->numcols, csv->numrows);
    string_view to_look_for = sv_from_lit("Phone 2");
    csv_print_column_from_string(csv , to_look_for);

    // changing a column name 
    sv t = sv_from_lit("Testing");
    csv->head[2] = t;

    ((f64**)csv->data)[0][2] = 15.0;
    // TODO: add typing to writing csv
    // writing the file
    csv_write_file("out.csv", csv);

    // writing all the types
    csv_print_types(csv);
    printf("%zu", (i64)csv_sum_column(csv, sv_from_lit("index")));
    csv_free(csv);
    return 0;
}

