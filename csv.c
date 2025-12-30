#include "includes/csv.h"
#include "includes/lib3man.h"
#include <assert.h>

CSV *load_csv(char *file_name){
    FILE * csv_f = fopen(file_name, "r");

    if(csv_f == NULL){
        fprintf(stderr, "Error, Can't open file or wrong path!");
        return NULL;
    }

    CSV * csv = create_csv();

    if(csv == NULL){
        fprintf(stderr, "Error, csv Object Allocation Failed!");
        return NULL;
    }

    fseek(csv_f, 0, SEEK_END);

    size_t temp_file_size = ftell(csv_f);
    u8 * csv_mem = calloc(temp_file_size + KiB(2), sizeof(u8));

    if(csv_mem == NULL){
        fprintf(stderr, "Error, Memory Allocation Failed");
        return NULL;
    }

    printf("%zu %zu\n", ftell(csv_f) + KiB(2), ftell(csv_f));

    rewind(csv_f);
    // columns left -> right
    // rows top -> bottom
    csv_to_memory(csv_mem, csv_f, temp_file_size, &csv->numcols, &csv->numrows);
    if(csv->numcols == 0 || csv->numrows == 0){
        perror("Error, csv Parcing Failed!");
        return NULL;
    }
    
    if(csv_parce_head(csv, csv_mem)){
        return  NULL;
    }

    
    // csv_print_head(csv);
    // for(size_t i = 0; i < temp_file_size; i++){
        //     printf("%c", csv_mem[i]);
        // }
    // printf("%zu\n", csv->numrow);
    csv_parse(csv, csv_mem);
    // printf("string: ");
    string_print(*(string *)&csv->data[0]);
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

void csv_to_memory(u8 *mem, FILE *file, size_t size, size_t *numcolumn, size_t *numrows){
    u8 is_next_line = 0;
    for(size_t i = 0; i < size; i++){
        u8 ch = fgetc(file);
        if(ch == '\r'){
            ch = fgetc(file);
            size--;
        }
        if(is_next_line == 0){
            if(ch == ',' || ch == '\n') (*numcolumn)++;
        }
        if(ch == '\n'){
            (*numrows)++;
            is_next_line = 1;
        }
        *mem = ch;
        mem++;
    }
    (*numrows)--;
    if(*(--mem) != '\n' && *mem != '\n')(*numrows)++; 
    printf("%c %d\n", *(mem), *(mem));
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
    csv->head = arenaList_Alloc(csv->gl_arena, sizeof(string) * csv->numcols);
    printf("numcolumn = %zu | numrow = %zu\n", csv->numcols, csv->numrows);
    assert(csv->numrows > 0);
    csv_parce_row(csv->gl_arena, csv->head, mem);
    // string_println(csv->head[i]);
    return 0;
}

void csv_print_head(CSV *csv){
    csv_print_row(csv->head, csv->numcols);
}

void csv_free(CSV *csv){
    arenaList_free(csv->gl_arena_head);
    free(csv);
}

u8 *csv_parce_row(ArenaList *arena, string *csv_row, u8 *mem){
    size_t count = 0;
    size_t current_column = 0;
    u8 is_quotes = false; // checking if test in quotations to not split using the ,
    int i = 0;
    for(; mem[i] != '\n' && mem[i] != 0; i++){
        if(mem[i] == '"' && is_quotes){
            is_quotes = false;
        }else if(mem[i] == '"'){
            is_quotes = true;
        }
        if(mem[i] == ',' && !is_quotes){
            // printf("%c %zu %c\n" , mem[i], count, mem[i - count]);
            // csv->head[current_column] = arenaList_Alloc(arenas, sizeof(string));
            csv_row[current_column].str = arenaList_Alloc(arena, count);
            csv_row[current_column].len = count;
            memcpy(csv_row[current_column].str, &mem[i - count], count);
            // string_print(csv_row[current_column]);
            // printf("len: %zu\n", csv_row[current_column].len );
            count = 0;
            current_column++;
        }else{
            count++;
        }
    }
    // add the last column that's before the \n
    csv_row[current_column].str = arenaList_Alloc(arena, count);
    csv_row[current_column].len = count;
    memcpy(csv_row[current_column].str, &mem[i - count], count);
    i++;
    return &mem[i];
}

int csv_parse(CSV *csv, u8 *mem){
    while(*(mem++) != '\n');
    csv->data = arenaList_Alloc(csv->gl_arena, sizeof(string **) * csv->numrows);
    for(size_t i = 0; i < csv->numrows; i++){
        csv->data[i] = arenaList_Alloc(csv->gl_arena, sizeof(string) * csv->numcols);
        mem = csv_parce_row(csv->gl_arena, csv->data[i], mem);
        // printf("########## arena_size:%zu max:%zu\n", csv->gl_arena->arena.cur_size, csv->gl_arena->arena.capacity);
        // string_println(csv->data[i][0]);
        // // printf("%s %s\n", mem ,csv->data[i][0].str);
        // printf("##########\n");
    }
    return 0;
}

void csv_print_row(string * row, size_t numcolumn){
    write(1, "[ ", 2);
    for(size_t i = 0; i < numcolumn; i++){
        string_print(row[i]);
        if(i < numcolumn - 1)
            write(1, "| ", 2);
    }
    write(1, " ]\n", 3);
}

void csv_print_column_from_string(CSV *csv, string column_name){
    int is_failed = 0;
    size_t index = csv_get_column_index(csv, column_name, &is_failed);
    if(is_failed){
        fprintf(stderr, "column not found\n");
        return;
    }
    write(1, "[ ", 2);
    for(size_t i = 0; i < csv->numrows; i++){
        string_print((*(string**)&csv->data[i])[index]);
        if(i < csv->numrows - 1)
            write(1, ", ", 2);
    }
    write(1, " ]\n", 3);
}

size_t csv_get_column_index(CSV *csv, string name, int *is_failed){
    for(size_t i = 0; i < csv->numcols; i++){
        if(string_cmp(&name, &csv->head[i])){
            return i;
        }
    }
    *is_failed = true;
    return 0;
}