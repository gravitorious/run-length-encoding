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

void writeRunsToUncompressedFile(FILE *fp, run *runs, int totalRuns){
    int i, j;
    for(i = 0; i < totalRuns; i++){
        for(j = 0; j < runs[i].count; j++){
            fprintf(fp, "%c", runs[i].c);
        }
    }
}

/*
 * put runs from compressed file into memory
 */
void readRunsFromCompressedFile(FILE *fp, run *runs, int totalRuns){
    int i;
    for(i = 0; i < totalRuns; i++){
        fseek(fp, i * sizeof(run), 0);
        fread(&runs[i], sizeof(runs[i]), 1, fp);
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
 * returns the number of total runs
 */
int calculateTotalRuns(FILE *fp){
    int size = sizeOfFile(fp);
    return (size/sizeof(run));
}

/*
 * opening binary file
 */
int openBinaryFile(FILE **fp){
    *fp = fopen(COMPRESSED_FILE, "rb");
    if(*fp == NULL) return FALSE;
    else return TRUE;
}

/*
 * opening text file
 */
int openTextFile(FILE **fp){
    *fp = fopen(UNCOMPRESSED_FILE, "w");
    if(*fp == NULL) return FALSE;
    else return TRUE;
}

int main(int argc, char *argv[]){
    FILE *tfp = NULL, *bfp = NULL;
    run *runs;
    int total_runs;
    int is_opened = openBinaryFile(&bfp);
    if(is_opened == FALSE){
        puts("File can't be opened for some reason...\n");
        exit(EXIT_FAILURE);
    }
    printf("Text file %s opened successfully...\n", COMPRESSED_FILE);
    total_runs = calculateTotalRuns(bfp);
    printf("%d total runs readed from input file...\n", total_runs);
    runs = (run *)malloc(total_runs * sizeof(run));
    if(runs == NULL){
        printf("Unexpected problem with memory...\n");
        exit(EXIT_FAILURE);
    }
    readRunsFromCompressedFile(bfp, runs, total_runs);
    printRuns(runs, total_runs);
    printf("The size of the input compressed file is: %d bytes\n", sizeOfFile(bfp));
    fclose(bfp);
    is_opened = openTextFile(&tfp);
    if(is_opened == FALSE){
        puts("File can't be opened for some reason...\n");
        exit(EXIT_FAILURE);
    }
    writeRunsToUncompressedFile(tfp, runs, total_runs);
    printf("The size of the output uncompressed file is: %d bytes\n", sizeOfFile(tfp));
    fclose(bfp);
    return 0;
}