#include "csv.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

csv_data * load_csv(char * file_name){
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

    char ch = 0;

    int i = 0;

    size_t lines = 0;

    while(ch != '\n' && ch != '\r'){
        ch = fgetc(file);
        if(ch == ',' || ch == '\n' || ch == '\r'){
            csv_->numrows++;
        }
    }

    printf("number_of_strs: %zu\n", csv_->numrows);
    rewind(file);
    ch = 0;                     
    csv_->head = malloc(csv_->numrows * sizeof(char *));
    size_t current_index = 0;

    while( ch != '\n' && ch != '\r'){
        ch = fgetc(file);
        if(ch == ',' || ch == '\n' || ch == '\r'){
            char * s = malloc((i + 1) * sizeof(char));
            fseek(file, -i - 1, SEEK_CUR);
            for(int j = 0; j < i; j++){
                ch = fgetc(file);
                s[j] = ch;
            }
            csv_->head[current_index]= s;
            current_index++;
            i = 0;
            ch = fgetc(file);
            lines++;
        }else{
            i++;
        }
    }
    size_t current_file_pos = ftell(file);
    printf("ftell : %zu\n", ftell(file));
    fseek(file, -1, SEEK_END);
    printf("ftell : %zu\n", ftell(file));
    fseek(file, current_file_pos, SEEK_SET);
    printf("ftell : %zu\n", ftell(file));
    csv_->data = malloc(sizeof(void *) * (lines - 1));
    // while( ch != '\n' && ch != '\r'){
        
    // }
    // printf("\n%zu\n", lines);
    printf("printing data............\n");
    printf("[");
    for(int i = 0; i < csv_->numrows; i++){
        printf("%s,", csv_->head[i]);
    }
    printf("]\n");
    return csv_;
}

int main(){
    load_csv("customers-100.csv");
    return 0;
}


