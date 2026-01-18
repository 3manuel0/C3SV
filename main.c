#include "includes/csv.h"
#include "includes/lib3man.h"

int main(){
    CSV *csv = load_csv("customers-100.csv");
    // CSV *csv = load_csv("test.csv");
    if(csv == NULL)return 1;
    // free_csv(csv);
    csv_print_head(csv);
    // csv_print_row(csv->data[5], csv->numcols);
    for(size_t i = 0; i < csv->numrows; i++){
        csv_print_row(csv->data[i], csv->numcols);
    }
    printf("numcols: %zu  %zu\n", csv->numcols, csv->numrows);
    sv to_look_for = sv_from_lit("Last Name");
    csv_print_column_from_string(csv , to_look_for);
    csv_free(csv);
    return 0;
}

