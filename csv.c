#include "includes/csv.h"
#include "includes/lib3man.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// TODO: FINISH LIB3MAN TO PARSE VALUES USING TYPES 

// ***************************************************************************
    void csv_to_memory(u8 *mem, FILE *file, size_t size, size_t *numcolumns,
                    size_t *numrows);

    int csv_parse_head(CSV *csv, u8 *mem);

    u8 *csv_parse_row(ArenaList *arena, string_view *csv_row, data_types *csv_data, u8 *mem);

    data_types get_type(string_view *sv);

    void csv_parse_with_types(CSV *csv);

    int csv_parse(CSV *csv, u8 *mem);
// ****************************************************************************

// TODO: ADD TYPES 
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

    printf("size + 2kib=%zu size=%zu\n", ftell(csv_f) + KiB(2), ftell(csv_f));

    rewind(csv_f);
    // columns left -> right
    // rows top -> bottom
    csv_to_memory(csv_mem, csv_f, temp_file_size, &csv->numcols, &csv->numrows);
    if(csv->numcols == 0 || csv->numrows == 0){
        perror("Error, csv Parcing Failed!");
        return NULL;
    }
    
    if(csv_parse_head(csv, csv_mem)){
        return  NULL;
    }

    
    // csv_print_head(csv);
    // for(size_t i = 0; i < temp_file_size; i++){
        //     printf("%c", csv_mem[i]);
        // }
    // printf("%zu\n", csv->numrow);
    csv_parse(csv, csv_mem);
    // printf("string: ");
    csv_parse_with_types(csv);
    // sv_print((string_view *)csv->data[0]);
    return csv;
}

void print_type(data_types t){
    // str_,
    // float_,
    // int_,
    // boolean_
    switch ((int)t) {
        case string_:
            fwrite("string\n", 1, 7, stdout);
            break;
        case float64_:
            fwrite("float\n", 1, 6, stdout);
            break;
        case int64_:
            fwrite("integer\n", 1, 8, stdout);
            break;
        case boolean_:
            fwrite("boolean\n", 1, 8, stdout);
            break;
        default:
            fwrite("Uknown type\n", 1, 12, stdout);
            break;
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
    // printf("%c %d\n", *(mem), *(mem));
}

CSV *create_csv(){
    CSV *csv = malloc(sizeof(CSV));
    if(csv == NULL) return NULL;
    csv->gl_arena = create_ArenaList(MiB(250));
    if(csv->gl_arena == NULL) return NULL;
    if(csv->gl_arena->arena.memory == NULL) return NULL;
    csv->gl_arena->next = NULL;
    csv->gl_arena_head = csv->gl_arena;
    return csv;
}


int csv_parse_head(CSV *csv, u8 *mem){
    csv->head = arenaList_Alloc(csv->gl_arena, sizeof(sv) * csv->numcols);
    printf("numcolumn = %zu | numrow = %zu\n", csv->numcols, csv->numrows);
    assert(csv->numrows > 0);
    csv_parse_row(csv->gl_arena, csv->head, NULL,mem);
    // string_println(csv->head[i]);
    return 0;
}

void csv_print_head(const CSV *csv){
    csv_print_row(csv->head, csv->numcols);
}

void csv_free(CSV *csv){
    arenaList_free(csv->gl_arena_head);
    free(csv);
}

u8 *csv_parse_row(ArenaList *arena, string_view *csv_row, data_types *csv_types, u8 *mem){
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
            // checking types
            if(csv_types != NULL){
                if(csv_types[current_column] != string_)
                    csv_types[current_column] = get_type(&csv_row[current_column]);
            }
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
    // checking types
    if(csv_types != NULL){
        if(csv_types[current_column] != string_)
            csv_types[current_column] = get_type(&csv_row[current_column]);
    }
    i++;
    return &mem[i];
}

data_types get_type(string_view *sv){
    if(sv_to_int64(sv, NULL)){
        return int64_;
    }else if(sv_to_float64(sv, NULL)){
        return float64_;
    }
    return string_;
}

int csv_parse(CSV *csv, u8 *mem){
    // skip the head the first row that has only the names
    while(*(mem++) != '\n');

    // adding types for now we parse all the rows as strings
    csv->types = arenaList_Alloc(csv->gl_arena, sizeof(data_types *) * csv->numcols);
    // for(size_t i = 0; i < csv->numrows; i++){
    //     csv->types[i] = string_;
    // }

    // reading the data
    csv->data = arenaList_Alloc(csv->gl_arena, sizeof(sv **) * csv->numrows);
    for(size_t i = 0; i < csv->numrows; i++){
        csv->data[i] = arenaList_Alloc(csv->gl_arena, sizeof(sv) * csv->numcols);
        mem = csv_parse_row(csv->gl_arena, csv->data[i], csv->types, mem);
    }
    
    return 0;
}

void csv_print_row(const string_view * row, size_t numcolumn){
    // fwrite(1, "[ ", 2);
    fwrite("[ ", 1, 2, stdout);
    for(size_t i = 0; i < numcolumn; i++){
        sv_print(&row[i]);
        if(i < numcolumn - 1)
            // write(1, ", ", 2);
            fwrite(", ", 1, 2, stdout);

    }
    // fwrite(1, " ]\n", 3);
    fwrite(" ]\n", 1, 3, stdout);
}

void csv_print_types(const CSV *csv){
    fwrite("[ ", 1, 2, stdout);
    for(size_t i = 0; i < csv->numcols; i++){
        switch ((int)csv->types[i]) {
            case string_:
                fwrite("string", 1, 7, stdout);
                break;
            case float64_:
                fwrite("float", 1, 6, stdout);
                break;
            case int64_:
                fwrite("integer", 1, 8, stdout);
                break;
            case boolean_:
                fwrite("boolean", 1, 8, stdout);
                break;
            default:
                fwrite("Uknown type", 1, 12, stdout);
                break;
        }
        if(i < csv->numcols - 1)
            // write(1, ", ", 2);
            fwrite(", ", 1, 2, stdout);

    }
    // fwrite(1, " ]\n", 3);
    fwrite(" ]\n", 1, 3, stdout);
}

void csv_print_column_from_string(const CSV *csv, string_view column_name){

    ssize_t index = csv_get_column_index(csv, column_name);
    
    if(index < 0){
        fprintf(stderr, "column not found\n");
        return;
    }
    // write(1, "[ ", 2);
    fwrite("[ ", 1, 2, stdout);
    for(size_t i = 0; i < csv->numrows; i++){
        sv_print(&((string_view **)csv->data)[i][index]);
        if(i < csv->numrows - 1)
            // write(1, ", ", 2);
            fwrite(", ", 1, 2, stdout);
    }
    // write(1, " ]\n", 3);
    fwrite(" ]\n", 1, 3, stdout);
}

ssize_t csv_get_column_index(const CSV *csv, string_view name){
    for(size_t i = 0; i < csv->numcols; i++){
        if(sv_cmp(&name, &csv->head[i])){
            return i;
        }
    }

    return -1;
}

// for now this only writes strings no type inference
void csv_write_file(const char *filename, const CSV *csv){
    assert(csv != NULL && filename != NULL);
    FILE *f = fopen(filename, "wb");
    for(size_t i = 0; i < csv->numcols; i++){
        fwrite(csv->head[i].str, 1, csv->head[i].len,f);
        fwrite(",", 1, 1,f);
    }
    fwrite("\n", 1, 1, f);
    for(size_t i = 0; i < csv->numrows; i++){

        for(size_t j = 0; j < csv->numcols; j++){
            fwrite(((sv **)csv->data)[i][j].str, 1, ((sv **)csv->data)[i][j].len,f);
            fwrite(",", 1, 1,f);
        }

        fwrite("\n", 1, 1, f);
    }
}

// TODO : FINISH PARSING 
void csv_parse_with_types(CSV *csv){
    for(size_t i = 0; i < csv->numrows; i++){
        for(size_t j = 0; j < csv->numcols; j++){
            string_view sv = ((string_view **)csv->data)[i][j];
            switch ((i64)csv->types[j]) {
                case string_:
                    printf("string : ");
                    sv_println(&sv);
                    break;
                case int64_: {
                    i64 t = 0;
                    sv_to_int64(&sv, &t);
                    printf("integer : %ld\n", t);
                    break;
                }
                case float64_ : {
                    f64 t = 0;
                    sv_to_float64(&sv, &t);
                    printf("float : %lf\n", t);
                    break;
                }
                case boolean_:
                    fwrite("boolean\n", 1, 8, stdout);
                    break;
                default:
                    fwrite("Uknown type\n", 1, 12, stdout);
                    break;
            }
        }
    }
}