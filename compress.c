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


int countChars(){

}

int openFile(FILE *fp){
    fp = fopen(UNCOMPRESSED_FILE, "r");
    if(fp == NULL) return FALSE;
    else return TRUE;
}



int main(int argc, char *argv[]){
    FILE *fp;
    int is_opened = openFile(fp);
    if(is_opened == FALSE){
        puts("File can't be opened for some reason");
        exit(1);
    }
    printf("File %s opened successfully", UNCOMPRESSED_FILE);

    return 0;
}
