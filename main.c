#include "includes/csv.h"

int main(){
    CSV * csv = load_csv("test1.csv");
    csv ++;
    // free_csv(csv);
    return 0;
}

