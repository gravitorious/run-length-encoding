//
// Created by Nikolas Mavrogeneiadis on 5/7/21.
//
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define UNCOMPRESSED_FILE "uncompressed_text.txt"

typedef struct{
    char c; //character
    int count;
}run;


/*
 * counting runs of the uncompressed file
 */
int countRuns(FILE *fp){
    int count = 0;
    char ch, char_of_current_run;
    ch = fgetc(fp);
    if(feof(fp)) return -1;
    char_of_current_run = ch;
    count++;
    while(!feof(fp)){
        ch = fgetc(fp);
        if(feof(fp)) break;
        if(ch != char_of_current_run){
            count++;
            char_of_current_run = ch;
        }
    }
    return count;
}

/*
 * opening file
 */
int openFile(FILE **fp){
    *fp = fopen(UNCOMPRESSED_FILE, "r");
    if(*fp == NULL) return FALSE;
    else return TRUE;
}

int main(int argc, char *argv[]){
    FILE *fp = NULL;
    int is_opened = openFile(&fp);
    if(is_opened == FALSE){
        puts("File can't be opened for some reason...\n");
        exit(EXIT_FAILURE);
    }
    printf("File %s opened successfully...\n", UNCOMPRESSED_FILE);
    int total_runs = countRuns(fp);
    if(total_runs == -1){
        printf("File is empty...\n");
        exit(EXIT_FAILURE);
    }


    return 0;
}
