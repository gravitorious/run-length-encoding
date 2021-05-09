//
// Created by Nikolas Mavrogeneiadis on 5/7/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0


typedef struct{
    char c; //character
    int count;
}run;

/*
 * print array
 */
void printArray(char *array, int size){
    int i;
    for(i = 0; i < size; i++) printf("%c", array[i]);
    printf("\n");
}

/*
 * write runs to the compressed file
 */
void writeRunsToCompressedFile(FILE *fp, run *runs, int totalRuns){
    int i;
    for(i = 0; i < totalRuns; i++){
        fseek(fp, i * sizeof(run), 0);
        fwrite(&runs[i], sizeof(runs[i]), 1, fp);
    }
}

/*
 * since we modified the compressed file with the help of the array, we move the elements from array
 * back to structs
 */
void moveRunsBackToStructs(char *newArray, int size_of_new_array, run *runs, int totalRuns){
    int i, j = 0, curent_run = 0;
    char current_char = newArray[0];
    runs[0].c = current_char;
    runs[0].count = 1;
    for(i = 1; i < size_of_new_array; i++){
        if(newArray[i] != current_char){
            j++;
            current_char = newArray[i];
            runs[j].c = current_char;
            runs[j].count = 1;
        }
        else{
            runs[j].count++;
        }
    }

}

/*
 * returns total runs from the new array
 */
int calculateRunsFromArray(char *newArray, int size){
    int i, count;
    char current_char = newArray[0];
    count = 1;

    for(i = 1; i < size; i++){
        if(newArray[i] != current_char){
            current_char = newArray[i];
            count++;
        }
    }
    return count;
}


/*
 * insert new char to array
 */
void insertNewChar(char ch, int times, int position, char *charArray, int totalChars, char *newArray){
    int i,j,k,p;
    for(i = 0; i < position; i++){ //put previous characters into new array
        newArray[i] = charArray[i];
    }
    k = i;
    printf("to i einai: %d\n", k);
    for(j = i; j < i+times; j++){ //put new char
        newArray[j] = ch;
        printf("ekteleite\n");
    }
    printf("to j einai: %d\n", j);
    for(p = k; p < totalChars; p++){
        newArray[j] = charArray[p];
        j++;
    }
}

/*
 * menu
 */

int menu(){
    int answer;
    do{
        printf("Press 1 to add new runs\n");
        printf("Press 2 to modify an existing run\n");
        printf("Press 3 to delete a run\n");
        scanf("%d", &answer);
    }while(answer != 1 && answer != 2 && answer != 3);
    return answer;
}

/*
 * put runs from structs into an array so we can modify them more easily
 */
void putRunsIntoAnArray(char *runsArray, run *runs, int totalRuns){
    int i,j = 0;
    int k = 0;
    for(i = 0; i < totalRuns; i++){
        for(j = k; j < k + runs[i].count; j++){
            runsArray[j] = runs[i].c;
        }
        k = j;
    }
}

/*
 * return total of characters
 */
int totalCharacters(run *runs, int totalRuns){
    int i, total_chars = 0;
    for(i = 0; i < totalRuns; i++){
        total_chars+=runs[i].count;
    }
    return total_chars;
}

/*
 * print runs in compressed mode
 */
void printRunsInCompressedMode(run *runs, int totalRuns){
    int i;
    for(i = 0; i < totalRuns; i++) printf("%d%c", runs[i].count, runs[i].c);
    printf("\n");
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
 * Write runs to text file
 */
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
 * opening binary file for writing
 */
int openBinaryFileForWriting(FILE **fp, char *bin_file){
    *fp = fopen(bin_file, "wb");
    if(*fp == NULL) return FALSE;
    else return TRUE;
}


/*
 * opening binary file
 */
int openBinaryFile(FILE **fp, char *bin_file){
    *fp = fopen(bin_file, "rb");
    if(*fp == NULL) return FALSE;
    else return TRUE;
}

/*
 * opening text file
 */
int openTextFile(FILE **fp, char *text_file){
    *fp = fopen(text_file, "w");
    if(*fp == NULL) return FALSE;
    else return TRUE;
}


int main(int argc, char *argv[]){
    FILE *tfp = NULL, *bfp = NULL;
    char text_file[100], bin_file[100], *runs_array, inchar, *new_array;
    run *runs;
    int total_runs, total_chars, answer, times, position, size_of_new_array, number_of_new_runs;
    if(argc == 3){
        strcpy(bin_file, argv[1]);
        strcpy(text_file, argv[2]);
    }
    else if(argc == 4 && strcmp(argv[1], "-e") == 0){
        strcpy(bin_file, argv[2]);
        strcpy(text_file, argv[3]);
        int is_opened = openBinaryFile(&bfp, bin_file);
        if(is_opened == FALSE){
            puts("File can't be opened for some reason...\n");
            exit(EXIT_FAILURE);
        }
        /*
         * put runs into an array so we can modify runs more easily
         */
        total_runs = calculateTotalRuns(bfp);
        runs = (run *)malloc(total_runs * sizeof(run));
        readRunsFromCompressedFile(bfp, runs, total_runs);
        fclose(bfp);
        total_chars = totalCharacters(runs, total_runs);
        runs_array = (char *)malloc(total_chars * sizeof(char));
        putRunsIntoAnArray(runs_array, runs, total_runs);
        printf("Your compressed file has the following format\n");
        printRunsInCompressedMode(runs, total_runs);
        printf("The total characters are: %d\n", total_chars);
        free(runs);
        answer = menu();
        if(answer == 1){
            printf("In what position do you want to add the run?\n");
            printf("Give a number between 0 and %d\n", total_chars);
            do {
                scanf("%d", &position);
            }while(position < 0 || position > total_chars);
            printf("What character do you want to add?\n");
            do{
                scanf("%c", &inchar);
            }while(inchar == '\n' || inchar == '\b'
                   || inchar == '\f' || inchar == '\n'
                   || inchar == '\r' || inchar == '\t'
                   || inchar == ' ');
            printf("How many times?\n");
            do{
                scanf("%d", &times);
            }while(times <= 0);
            size_of_new_array = times+total_chars;
            new_array = (char *)malloc(size_of_new_array * sizeof(char));
            insertNewChar(inchar, times, position, runs_array, total_chars, new_array);
            printArray(new_array, size_of_new_array);
            number_of_new_runs = calculateRunsFromArray(new_array, size_of_new_array);
            printf("number of new runs: %d", number_of_new_runs);
            runs = (run *)malloc(total_runs * sizeof(run));
            moveRunsBackToStructs(new_array, size_of_new_array, runs, number_of_new_runs);
            printRuns(runs, number_of_new_runs);
            free(new_array);
            //since we have the modified file structs, we just need to write it to binary file and text file
            int is_opened = openBinaryFileForWriting(&bfp, bin_file);
            if(is_opened == FALSE){
                puts("File can't be opened for some reason...\n");
                exit(EXIT_FAILURE);
            }
            writeRunsToCompressedFile(bfp, runs, number_of_new_runs);
            fclose(bfp);
        }
    }

    /*
     * uncompress file
     */
    int is_opened = openBinaryFile(&bfp, bin_file);
    if(is_opened == FALSE){
        puts("File can't be opened for some reason...\n");
        exit(EXIT_FAILURE);
    }
    printf("Text file %s opened successfully...\n", bin_file);
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
    is_opened = openTextFile(&tfp, text_file);
    if(is_opened == FALSE){
        puts("File can't be opened for some reason...\n");
        exit(EXIT_FAILURE);
    }
    writeRunsToUncompressedFile(tfp, runs, total_runs);
    printf("The size of the output uncompressed file is: %d bytes\n", sizeOfFile(tfp));
    fclose(bfp);

    free(runs);
    return 0;
}