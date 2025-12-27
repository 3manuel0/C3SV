#include "includes/csv.h"
#include "includes/lib3man.h"
#include <assert.h>
#include <stdlib.h>

// global arenas

CSV *load_csv(char *file_name){
    FILE * csv_f = fopen(file_name, "r");

    if(csv_f == NULL){
        perror("Can't open file");
        return NULL;
    }

    CSV * csv = create_csv();

    if(csv == NULL){
        perror("csv creation failed, problem allocating memory");
        return NULL;
    }

    fseek(csv_f, 0, SEEK_END);

    Arena arena = create_Arena(ftell(csv_f) + KiB(2)); 
    size_t temp_file_size = ftell(csv_f);
    u8 * csv_mem = arena_Alloc(&arena, temp_file_size);

    if(csv_mem == NULL){
        perror("Error, Allocation Failed");
        return NULL;
    }

    printf("%zu %zu\n", ftell(csv_f) + KiB(2), ftell(csv_f));

    rewind(csv_f);
    // rows left -> right
    // columns top -> bottom
    csv_to_memory(csv_mem, csv_f, temp_file_size, &csv->numrows, &csv->numcols);

    if(csv_create_header(csv, csv_mem)){
        return  NULL;
    }

    csv_print_head(csv);
    // for(size_t i = 0; i < temp_file_size; i++){
    //     printf("%c", csv_mem[i]);
    // }
    return csv;
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

void csv_to_memory(u8 *mem, FILE *file, size_t size, size_t *numrows, size_t *numcolumns){
    u8 is_next_line = 0;
    for(size_t i = 0; i < size; i++){
        u8 ch = fgetc(file);
        if(is_next_line == 0){
            if(ch == ',' || ch == '\n') (*numrows)++;
        }
        if(ch == '\n'){
            (*numcolumns)++;
            is_next_line = 1;
        }
        *mem = ch;
        mem++;
    }
    (*numcolumns)--;
    if(*(--mem) != '\n' && *mem != '\n')(*numcolumns)++; 
    printf("%c %d\n", *(mem-1), *(mem-1));
}

CSV *create_csv(){
    CSV * csv = malloc(sizeof(CSV));
    if(csv == NULL) return NULL;
    csv->gl_arena = malloc(sizeof(ArenaList));
    if(csv->gl_arena == NULL) return NULL;
    csv->gl_arena->arena = create_Arena(MiB(5));
    if(csv->gl_arena->arena.memory == NULL) return NULL;
    csv->gl_arena->next = NULL;
    csv->gl_arena_head = csv->gl_arena;
    return csv;
}


int csv_create_header(CSV *csv, u8 *mem){
    csv->head = arenaList_Alloc(csv->gl_arena, sizeof(string) * csv->numrows);
    printf("numrows = %zu | numcols = %zu\n", csv->numrows, csv->numcols);
    assert(csv->numcols > 0);
    size_t count = 0;
    size_t current_row = 0;
    int i = 0;
    for(; mem[i] != '\n'; i++){
        if(mem[i] == ','){
            printf("%c %zu %c\n" , mem[i], count, mem[i - count]);
            // csv->head[current_row] = arenaList_Alloc(arenas, sizeof(string));
            csv->head[current_row].str = arenaList_Alloc(csv->gl_arena, count);
            csv->head[current_row].len = count;
            memcpy(csv->head[current_row].str, &mem[i - count], count);
            count = 0;
            current_row++;
        }else{
            count++;
        }
    }
    // add the last row before the \n
    csv->head[current_row].str = arenaList_Alloc(csv->gl_arena, count);
    csv->head[current_row].len = count;
    memcpy(csv->head[current_row].str, &mem[i - count], count);
    // string_println(csv->head[i]);
    return 0;
}

void csv_print_head(CSV *csv){
    write(1, "[", 1);
    for(size_t i = 0; i < csv->numrows; i++){
        string_print(csv->head[i]);
        if(i < csv->numrows - 1)
            write(1, ", ", 2);
    }
    write(1, "]", 1);
    write(1, "\n", 1);
}

void csv_free(CSV *csv){
    // arenas_free((Arenas*)csv);
    // TODO : FREE MEMORY
    arenaList_free(csv->gl_arena_head);
    free(csv);
}