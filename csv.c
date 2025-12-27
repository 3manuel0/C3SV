#include "includes/csv.h"
#include "includes/lib3man.h"
#include <assert.h>


void csv_print_line(string * line, size_t numrows){
    write(1, "[", 1);
    for(size_t i = 0; i < numrows; i++){
        string_print(line[i]);
        if(i < numrows - 1)
            write(1, ", ", 2);
    }
    write(1, "]", 1);
    write(1, "\n", 1);
}

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

    size_t temp_file_size = ftell(csv_f);
    u8 * csv_mem = calloc(temp_file_size + KiB(2), sizeof(u8));

    if(csv_mem == NULL){
        perror("Error, Allocation Failed");
        return NULL;
    }

    printf("%zu %zu\n", ftell(csv_f) + KiB(2), ftell(csv_f));

    rewind(csv_f);
    // rows left -> right
    // columns top -> bottom
    csv_to_memory(csv_mem, csv_f, temp_file_size, &csv->numrows, &csv->numcols);

    if(csv_parce_head(csv, csv_mem)){
        return  NULL;
    }

    
    // csv_print_head(csv);
    // for(size_t i = 0; i < temp_file_size; i++){
        //     printf("%c", csv_mem[i]);
        // }
    // printf("%zu\n", csv->numcols);
    csv_parse(csv, csv_mem);
    printf("string: ");
    string_print(*(string *)&csv->data[0]);
    for(size_t i = 0; i < csv->numcols; i++)
        csv_print_line(csv->data[i], csv->numrows);
    return csv;
}


void print_type(data_types t){
    // str_,
    // float_,
    // int_,
    // boolean_
    switch ((int)t) {
        case string_:
            puts("string");
            break;
        case float64_:
            puts("float");
            break;
        case int64_:
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
        return  float64_;
    }else if(number_of_points == 0 && !is_not_numb){
        return  int64_;
    }else{
        return  string_;
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
    csv->gl_arena = create_ArenaList(MiB(5));
    if(csv->gl_arena == NULL) return NULL;
    if(csv->gl_arena->arena.memory == NULL) return NULL;
    csv->gl_arena->next = NULL;
    csv->gl_arena_head = csv->gl_arena;
    return csv;
}


int csv_parce_head(CSV *csv, u8 *mem){
    csv->head = arenaList_Alloc(csv->gl_arena, sizeof(string) * csv->numrows);
    printf("numrows = %zu | numcols = %zu\n", csv->numrows, csv->numcols);
    assert(csv->numcols > 0);
    csv_parce_line(csv->gl_arena, csv->head, mem);
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
    arenaList_free(csv->gl_arena_head);
    free(csv);
}

u8 *csv_parce_line(ArenaList *arena, string *csv_line, u8 *mem){
    size_t count = 0;
    size_t current_row = 0;
    int i = 0;
    for(; mem[i] != '\n' && mem[i] != 0; i++){
        if(mem[i] == ','){
            // printf("%c %zu %c\n" , mem[i], count, mem[i - count]);
            // csv->head[current_row] = arenaList_Alloc(arenas, sizeof(string));
            csv_line[current_row].str = arenaList_Alloc(arena, count);
            csv_line[current_row].len = count;
            memcpy(csv_line[current_row].str, &mem[i - count], count);
            // string_print(csv_line[current_row]);
            count = 0;
            current_row++;
        }else{
            count++;
        }
    }
    // add the last row that's before the \n
    csv_line[current_row].str = arenaList_Alloc(arena, count);
    csv_line[current_row].len = count;
    memcpy(csv_line[current_row].str, &mem[i - count], count);
    i++;
    return &mem[i];
}

int csv_parse(CSV *csv, u8 *mem){
    while(*(mem++) != '\n');
    printf("string : %s %d %c\n", mem, *mem, *mem);
    csv->data = arenaList_Alloc(csv->gl_arena, sizeof(string **) * csv->numcols);
    for(size_t i = 0; i < csv->numcols; i++){
        // TODO : Fix this mess
        csv->data[i] = arenaList_Alloc(csv->gl_arena, sizeof(string*) * csv->numrows);
        mem = csv_parce_line(csv->gl_arena, csv->data[i], mem);
        printf("########## arena_size:%zu max:%zu\n", csv->gl_arena->arena.cur_size, csv->gl_arena->arena.capacity);
        // string_println(csv->data[i][0]);
        printf("%s %s\n", mem ,csv->data[i][0].str);
        printf("##########\n");
    }
    return 0;
}