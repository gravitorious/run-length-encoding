//
// Created by Nikolas Mavrogeneiadis on 5/7/21.
//
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define UNCOMPRESSED_FILE "uncompressed_text.txt"
#define COMPRESSED_FILE "compressed_text.compressed"

typedef struct{
    char c; //character
    int count;
}run;

void writeRunsToCompressedFile(FILE *fp, run *runs, int totalRuns){
    int i;
    for(i = 0; i < totalRuns; i++){
        fseek(fp, i * sizeof(run), 0);
        fwrite(&runs[i], sizeof(runs[i]), 1, fp);
    }
}

/*
 * returns the size of the file
 */
int sizeOfFile(FILE *fp){
    int size;
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    return size;
}

/*
 * print runs
 */
void printRuns(run *runs, int totalRuns){
    int i;
    for(i = 0; i < totalRuns; i++){
        printf("Run: %d\n", i+1);
        printf("Char:%c,", runs[i].c);
        printf("Times:%d\n", runs[i].count);
    }
}

/*
 * Putting runs from file into memory
 */
void saveRuns(run *runs, FILE *fp){
    int current_run;
    char ch, char_of_current_run;
    ch = fgetc(fp);
    char_of_current_run = ch;
    current_run = 0;
    runs[current_run].count = 1;
    runs[current_run].c = char_of_current_run;
    while(!feof(fp)){
        ch = fgetc(fp);
        if(feof(fp)) break;
        if(ch == '\n' || ch == '\b' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' || ch == ' ') continue;
        if(ch == char_of_current_run){
            (runs[current_run].count)++;
        }
        else{
            current_run++;
            char_of_current_run = ch;
            runs[current_run].count = 1;
            runs[current_run].c = char_of_current_run;
        }
    }
}

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
        if(ch != char_of_current_run && ch != '\n' && ch != '\b'
                                     && ch != '\f' && ch != '\n'
                                     && ch != '\r' && ch != '\t'
                                     && ch != ' '){

            count++;
            char_of_current_run = ch;
        }
    }
    return count;
}

/*
 * opening binary file
 */
int openBinaryFile(FILE **fp){
    *fp = fopen(COMPRESSED_FILE, "wb");
    if(*fp == NULL) return FALSE;
    else return TRUE;
}

/*
 * opening text file
 */
int openTextFile(FILE **fp){
    *fp = fopen(UNCOMPRESSED_FILE, "r");
    if(*fp == NULL) return FALSE;
    else return TRUE;
}

int main(int argc, char *argv[]){
    FILE *tfp = NULL, *bfp = NULL;
    run *runs;
    int is_opened = openTextFile(&tfp);
    if(is_opened == FALSE){
        puts("File can't be opened for some reason...\n");
        exit(EXIT_FAILURE);
    }
    printf("Text file %s opened successfully...\n", UNCOMPRESSED_FILE);
    int total_runs = countRuns(tfp);
    rewind(tfp);
    if(total_runs == -1){
        printf("File is empty...\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Total runs: %d\n", total_runs);
    }
    runs = (run *)malloc(total_runs * sizeof(run));
    if(runs == NULL){
        printf("Unexpected problem with memory...\n");
        exit(EXIT_FAILURE);
    }
    saveRuns(runs, tfp);
    printRuns(runs, total_runs);
    printf("The size of the input uncompressed file is: %d bytes\n", sizeOfFile(tfp));

    fclose(tfp);

    is_opened = openBinaryFile(&bfp);
    if(is_opened == FALSE){
        puts("Binary file can't be opened for some reason...\n");
        exit(EXIT_FAILURE);
    }
    writeRunsToCompressedFile(bfp, runs, total_runs);
    fclose(bfp);
    printf("The size of the output compressed file is: %d bytes\n", sizeOfFile(bfp));
    return 0;
}