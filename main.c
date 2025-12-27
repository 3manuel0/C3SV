#include "includes/csv.h"

int main(){
    CSV *csv = load_csv("test1.csv");
    if(csv == NULL) return 1;
    // free_csv(csv);
    csv_print_head(csv);
    printf("%zu \n", sizeof(CSV));
    csv_free(csv);
    return 0;
}

