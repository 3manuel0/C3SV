#include "includes/csv.h"
#include "includes/lib3man.h"

int main(){
    CSV *csv = load_csv("test2.csv");
    if(csv == NULL) return 1;
    // free_csv(csv);
    csv_print_head(csv);
    csv_print_row(csv->data[5], csv->numcols);
    csv_print_row(csv->data[0], csv->numcols);
    printf("%zu \n", sizeof(CSV));
    string to_look_for = string_from_lit("country");
    csv_print_column_from_string(csv , to_look_for);
    csv_free(csv);
    return 0;
}

