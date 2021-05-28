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
 * delete sequence from array with char
 */
void deleteSequence(char *inputArray, int totalChars, int start, int end, char *newArray){
    int i, j;
    for(i = 0; i < start -1; i++){
        newArray[i] = inputArray[i];
    }
    for(j = end; j < totalChars; j++){
        newArray[i] = inputArray[j];
        i++;
    }
}

/*
 * replace sequence from array with char
 */
void replaceSequence(char *newArray, int start, int end, char ch){
    int i;
    for(i = start-1; i < end; i++){
        newArray[i] = ch;
    }
}

/*
 * write runs to the compressed file
 */
void writeRunsToCompressedFile(FILE *fp, run *runs, int totalRuns){
    int i;
    for(i = 0; i < totalRuns; i++){ //for each run
        fseek(fp, i * sizeof(run), 0); //each time we move i * sizeof(run) bytes to write the next run
        fwrite(&runs[i], sizeof(runs[i]), 1, fp); //write the next run
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
        if(newArray[i] != current_char){ //we have new run so we move to the next struct
            j++; //the position of current run
            current_char = newArray[i];
            runs[j].c = current_char;
            runs[j].count = 1;
        }
        else{ //if we don't have new run, we just increase the count
            runs[j].count++;
        }
    }
}

/*
 * returns total runs from the new array
 */
int calculateRunsFromArray(char *newArray, int size){
    int i, count;
    char current_char = newArray[0]; //character for each run
    count = 1;

    for(i = 1; i < size; i++){
        if(newArray[i] != current_char){ //it means that we have new array
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
    for(i = 0; i < position; i++){ //put previous characters (before the position) into new array
        newArray[i] = charArray[i];
    }
    k = i;
    for(j = i; j < i+times; j++){ //put new char as many times as "times"
        newArray[j] = ch;
    }
    for(p = k; p < totalChars; p++){ //put the rest of chars into new array
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
        printf("Press 2 to modify an existing sequence\n");
        printf("Press 3 to delete a sequence\n");
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
    for(i = 0; i < totalRuns; i++){ //for each run
        for(j = k; j < k + runs[i].count; j++){ //put all characters from run into runsArray
            runsArray[j] = runs[i].c;
        }
        k = j; //from this position we will continue to put chars into runsArray
    }
}

/*
 * return total of characters
 */
int totalCharacters(run *runs, int totalRuns){
    int i, total_chars = 0;
    for(i = 0; i < totalRuns; i++){
        total_chars+=runs[i].count; //sum of total characters
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
        total_runs = calculateTotalRuns(bfp); //number of runs from compressed file
        runs = (run *)malloc(total_runs * sizeof(run));
        readRunsFromCompressedFile(bfp, runs, total_runs); //puts runs to struct array
        fclose(bfp);
        total_chars = totalCharacters(runs, total_runs); //calculate the total of characters
        runs_array = (char *)malloc(total_chars * sizeof(char));
        putRunsIntoAnArray(runs_array, runs, total_runs); //put all characters from structs to array
        printf("Your compressed file has the following format\n");
        printRunsInCompressedMode(runs, total_runs); //print the compressed file
        printf("The total characters are: %d\n", total_chars);
        free(runs);
        answer = menu(); //print the menu
        if(answer == 1){
            printf("In what position do you want to add the run?\n");
            printf("Give a number between 0 and %d\n", total_chars);
            do {
                scanf("%d", &position); //read the position that user wants to add a run
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
            size_of_new_array = times+total_chars; //total characters with the new run
            new_array = (char *)malloc(size_of_new_array * sizeof(char));
            insertNewChar(inchar, times, position, runs_array, total_chars, new_array); //insert new character on the right position
            number_of_new_runs = calculateRunsFromArray(new_array, size_of_new_array); //calculate the new number of runs
            runs = (run *)malloc(number_of_new_runs * sizeof(run));
            moveRunsBackToStructs(new_array, size_of_new_array, runs, number_of_new_runs); //move runs back to structs
            free(new_array); //we free the memory for array because we don't need it anymore
            free(runs_array);
            //since we have the modified file structs, we just need to write it to binary file and text file
            int is_opened = openBinaryFileForWriting(&bfp, bin_file);
            if(is_opened == FALSE){
                puts("File can't be opened for some reason...\n");
                exit(EXIT_FAILURE);
            }
            writeRunsToCompressedFile(bfp, runs, number_of_new_runs); //modify the compressed file after the modification
            fclose(bfp);
        }
        else if(answer == 2){
            int start, end;
            printf("Sequence start position: [1-%d]\n", total_chars);
            do{
                scanf("%d", &start);
            }while(start < 0 || start > total_chars);
            printf("Sequence end position: [%d-%d]\n", start, total_chars);
            do{
                scanf("%d", &end);
            }while(end < start || end > total_chars);
            printf("With character you want to replace?\n");
            do{
                scanf("%c", &inchar);
            }while(inchar == '\n' || inchar == '\b'
                   || inchar == '\f' || inchar == '\n'
                   || inchar == '\r' || inchar == '\t'
                   || inchar == ' ');
            replaceSequence(runs_array, start, end, inchar);
            number_of_new_runs = calculateRunsFromArray(runs_array, total_chars);
            runs = (run *)malloc(number_of_new_runs * sizeof(run));
            moveRunsBackToStructs(runs_array, total_chars, runs, number_of_new_runs); //move runs back to structs
            free(runs_array); //we free the memory for array because we don't need it anymore
            //since we have the modified file structs, we just need to write it to binary file and text file
            int is_opened = openBinaryFileForWriting(&bfp, bin_file);
            if(is_opened == FALSE){
                puts("File can't be opened for some reason...\n");
                exit(EXIT_FAILURE);
            }
            writeRunsToCompressedFile(bfp, runs, number_of_new_runs); //modify the compressed file after the modification
            fclose(bfp);
        }
        else if(answer == 3){
            int start, end;
            char *array_after_deletion;
            printf("Sequence start position: [1-%d]\n", total_chars);
            do{
                scanf("%d", &start);
            }while(start < 0 || start > total_chars);
            printf("Sequence end position: [%d-%d]\n", start, total_chars);
            do{
                scanf("%d", &end);
            }while(end < start || end > total_chars);
            int new_size = total_chars - (end-start+1);
            array_after_deletion = (char *)malloc(new_size * sizeof(char));
            deleteSequence(runs_array, total_chars, start, end, array_after_deletion);
            number_of_new_runs = calculateRunsFromArray(array_after_deletion, new_size);
            runs = (run *)malloc(number_of_new_runs * sizeof(run));
            moveRunsBackToStructs(array_after_deletion, new_size, runs, number_of_new_runs); //move runs back to structs
            free(runs_array); //we free the memory for array because we don't need it anymore
            free(array_after_deletion);
            //since we have the modified file structs, we just need to write it to binary file and text file
            int is_opened = openBinaryFileForWriting(&bfp, bin_file);
            if(is_opened == FALSE){
                puts("File can't be opened for some reason...\n");
                exit(EXIT_FAILURE);
            }
            writeRunsToCompressedFile(bfp, runs, number_of_new_runs); //modify the compressed file after the modification
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