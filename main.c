#include "includes/csv.h"

int main(){
    CSV *csv = load_csv("test.csv");
    if(csv == NULL) return 1;
    // free_csv(csv);
    csv_print_head(csv);
    for(size_t i = 0; i < csv->numcols; i++)
        csv_print_column(csv->data[i], csv->numrows);
    printf("%zu \n", sizeof(CSV));
    csv_free(csv);
    return 0;
}

