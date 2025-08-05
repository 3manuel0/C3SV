#include "csv.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

csv_data * load_csv_data(char * file_name){
    FILE * file = fopen(file_name, "rb");

    if(file == NULL){
        perror("Can't open file");
        return NULL;
    }

    csv_data * csv_ = malloc(sizeof(csv_data));

    size_t size;

    fseek(file, 0, SEEK_END);

    size = ftell(file);

    rewind(file);

    char * csv_in_mem = load_csv_file_to_memory(csv_, file, &size);
    char ch = 0;
    size_t k = 0;
    size_t lines = 0;
    size_t current_row_index = 0;

    csv_->head = malloc(csv_->numcols * sizeof(char *));
    csv_->types = malloc(csv_->numcols * sizeof(data_types));
    for(int i = 0; i < size; i++){
        if(csv_in_mem[i] == ',' || i == size - 1){
            if(current_row_index < csv_->numcols){
                // printf("k = %zu current_row_index = %zu\n", k, csv_->numrows);
                char * s = malloc((k + 1) * sizeof(char));
                for(int j = 0; j < k; j++){
                    s[j] = csv_in_mem[i - k + j];
                }
                // printf(" string:%s %zu %zu\n", s, k, current_row_index);
                csv_->head[current_row_index]= s;
                current_row_index++;
                k = 0;
            }else{
                char * s = malloc((k + 1) * sizeof(char));
                for(int j = 0; j < k; j++){
                    s[j] = csv_in_mem[i - k + j];
                }
                parse_param(csv_, s, &current_row_index);
                k = 0;
                current_row_index++;
            }
        }else{
            k++;
        }
        // printf("%c", csv_in_mem[i]);
    }

    printf("number_of_strs: %zu\n", csv_->numrows);
    // rewind(file);
    // ch = 0;                     

    // size_t current_index = 0;

    // while( ch != '\n' && ch != '\r'){
    //     ch = fgetc(file);
    //     if(ch == ',' || ch == '\n' || ch == '\r'){
    //         printf("%c %d", ch, ch);
    //         char * s = malloc((i + 1) * sizeof(char));
            
    //         fseek(file, -i - 1, SEEK_CUR);
    //         for(int j = 0; j < i; j++){
    //             ch = fgetc(file);
    //             s[j] = ch;
    //         }
    //         printf(" string:%s %d\n", s,s[0]);
    //         csv_->head[current_index]= s;
    //         current_index++;
    //         i = 0;
    //         ch = fgetc(file);
    //         printf("%c %d", ch, ch);
    //         ch = fgetc(file);
    //         printf("%c %d", ch, ch);

    //     }else{
    //         i++;
    //     }
        
    // }

    // size_t current_file_pos = ftell(file);
    // printf("\nftell : %zu\n", current_file_pos);
    // int b = current_file_pos + 1;
    // while(b < size){
    //     ch = fgetc(file);
    //     if(ch == '\n'){
    //         lines++;
    //     }
    //     b++;
    // }

    // printf("ftell : %zu lines: %zu\n", ftell(file), lines);
    // fseek(file, current_file_pos, SEEK_SET);
    // printf("ftell : %zu\n", ftell(file));
    // csv_->numcols = lines;
    // // csv_->data = malloc(sizeof(void *) * lines);
    // i = 0;
    // size_t current_row_index = 0;
    // size_t current_col_index = 0;
    // // b = 0;
    // lines = 0;

    // while(b < size){
    //     ch = fgetc(file);
    //     if(ch == '\n' || ch == '\r'){
    //         lines++;
    //         current_row_index = 0;
    //     }
    //     if(ch == ',' || ch == '\n' || ch == '\r'){
    //         char * s = calloc((i + 1) , sizeof(char));
    //         // char s[255] = {0};
    //         fseek(file, -i - 1, SEEK_CUR);
    //         for(int j = 0; j < i; j++){
    //             ch = fgetc(file);
    //             s[j] = ch;
    //         }
    //         // csv_->head[current_index]= s;
    //         if(i > 0)
    //             parse_param(csv_, s, &current_row_index);
    //         free(s);
    //         current_row_index ++;
    //         i = 0;
    //         ch = fgetc(file);
    //     }else{
    //         i++;
    //     }
    //     // printf(".......................");
    //     b++;
    // }
    // printf("\n%zu\n", lines);
    printf("printing data............\n");
    printf("[");
    for(int i = 0; i < csv_->numcols; i++){
        printf("%s,", csv_->head[i]);
    }
    printf("]\n");

    printf("printing types............\n");
    for(int i = 0; i < csv_->numcols; i++){
        print_type(csv_->types[i]);
    }

    return csv_;
    
}



int main(){
    load_csv_data("test.csv");
    return 0;
}


void parse_param(csv_data * csv_, char * param, size_t * current_index){
    // printf("%s\n", param);
    // if(*current_index >= csv_->numrows)return;
    size_t c = 0;
    char is_not_numb = 0;

    if(param[0] == '-'){
        c++;
    }

    while(param[c]){
        if(param[c] > 57 || param[c] < '0'){
            is_not_numb = 1;
            break;
        }
        c++;
    }
    printf("%d %zu %zu\n", *current_index < (csv_->numcols * 2), *current_index, csv_->numcols * 2);
    size_t temp = *current_index - csv_->numcols;
    if(temp < csv_->numcols){
        if(is_not_numb){
            csv_->types[temp] = str_;
        }else{
            csv_->types[temp] = int_;
        }
        // printf("index = %zu\n", ( temp));
    }
    // printf("current_index: %zu csv_->numcols  %zu\n", *current_index, csv_->numrows);
    if ((*current_index - csv_->numcols) > csv_->numcols - 1) return;
    switch (csv_->types[temp]) {
        case str_:
            printf("string : %s\n", param);
            break;
        case float_:
            printf("string : %s\n", param);
            break;
        case int_:
            printf("int : %d\n ", atoi(param));
            break;
        case boolean_:
            printf("string : %s\n", param);
            break;
        default:
            printf("string : %s\n", param);
            break;
    }


}

char * load_csv_file_to_memory(csv_data * csv_, FILE * f, size_t  * len){
    
    size_t byte_len = *len;
    char * a = malloc(byte_len);
    if(a == NULL){
        perror("error in allocation\n");
        exit(-1);
    }
    char ch;
    char fline = 0;
    size_t i;
    for(i = 0; i < byte_len; i++){
        ch = fgetc(f);
        if(ch == EOF){
            break;
        }
        if(ch == '\n' || ch == '\r'){
            fline = 1;
            csv_->numrows++;
        }
        if(ch == ',' && !fline){
            csv_->numcols++;
        }
        // TODO fix this mess
        while(ch == '\r' || ch == '\n' ){
            // if(ch == '\n'){
            //     a[i] = ',';
            // }
            ch = ',';
        }
        a[i] = ch;
        // printf("%c", a[i]);
    }
    *len = i;
    csv_->numcols++;
    printf("numrows:%zu numcols:%zu byte_len: %zu\n", csv_->numrows, csv_->numcols, byte_len);
    return a;
}


void print_type(data_types t){
    // str_,
    // float_,
    // int_,
    // boolean_
    switch ((int)t) {
        case str_:
            printf("string\n");
            break;
        case float_:
            printf("float\n");
            break;
        case int_:
            printf("int\n");
            break;
        case boolean_:
            printf("boolean \n");
            break;
        default:
            printf("Uknown type\n");
            break;
    }
}