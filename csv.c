#include "includes/csv.h"
#include "includes/lib3man.h"
#include <stdio.h>

// global arenas
Arenas arenas = {0};
Arenas * arenas_head = &arenas;

CSV *load_csv(char *file_name){
    FILE * csv_f = fopen(file_name, "r");
    
    arenas = (Arenas){.arena = create_Arena(MiB(5)), .next = NULL};
    
    if(csv_f == NULL){
        perror("Can't open file");
        return NULL;
    }

    fseek(csv_f, 0, SEEK_END);

    Arena arena = create_Arena(ftell(csv_f) + KiB(2)); 
    size_t temp_file_size = ftell(csv_f);
    u8 * csv_mem = arena_Alloc(&arena, temp_file_size);

    if(csv_f == NULL){
        perror("Error, Allocation Failed");
        return NULL;
    }

    printf("%zu %zu", ftell(csv_f) + KiB(2), ftell(csv_f));

    rewind(csv_f);

    csv_to_memory(csv_mem, csv_f, temp_file_size);

    for(size_t i = 0; i < temp_file_size; i++){
        printf("%c", csv_mem[i]);
    }

    printf("address = %p\n", arena.address);
    return NULL;
}


void print_type(data_types t){
    // str_,
    // float_,
    // int_,
    // boolean_
    switch ((int)t) {
        case str_:
            puts("string");
            break;
        case float_:
            puts("float");
            break;
        case int_:
            puts("int");
            break;
        case boolean_:
            puts("boolean");
            break;
        default:
            puts("Uknown type");
            break;
    }
}



void append_strs(string *a, string *b){
    size_t newlen = a->len + b->len;
    char * temp = malloc(newlen);
    size_t offst = 0;

    for(; offst < a->len; offst++){
        temp[offst] = a->str[offst];
    }

    for(size_t i = 0; i < b->len; i++){
        temp[offst] = b->str[i];
    }

    free(b->str);
    b->len = 0;
    b = NULL;

    free(a->str);
    a->str = temp;
    a->len = newlen;
}


data_types get_type(char * s){
    size_t c = 0;
    i8 is_not_numb = 0;
    i32 number_of_points = 0; 

    if(s[0] == '-'){
        c++;
    }

    while(s[c]){
        if(s[c] != '.' && (s[c] > '9' || s[c] < '0')){
            is_not_numb = 1;
            break;
        }
        if(s[c] == '.') number_of_points++;
        c++;
    }

    if(number_of_points == 1 && !is_not_numb){
        return  float_;
    }else if(number_of_points == 0 && !is_not_numb){
        return  int_;
    }else{
        return  str_;
    }
}

void csv_to_memory(u8 * mem, FILE * file, size_t size){
    for(size_t i = 0; i < size; i++){
        u8 ch = fgetc(file);
        *mem = ch;
        mem++;
    }
    // printf("address from func = %p\n", mem);
}