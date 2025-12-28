#include "includes/csv.h"

int main(){
    CSV *csv = load_csv("test.csv");
    if(csv == NULL) return 1;
    // free_csv(csv);
    csv_print_head(csv);
    for(size_t i = 0; i < csv->numrows; i++)
        csv_print_row(csv->data[i], csv->numcols);
    printf("%zu \n", sizeof(CSV));
    csv_free(csv);
    return 0;
}

