//
// Created by Anshu on 11/3/18.
// Co-author Anshu Verma
//

#include <stdio.h>
#include <stdlib.h>
#include "validator.h"
#define MAX_SIZE 2048

void validateTarget(char *line, unsigned int size, int lineNo) {
    if (size == 0) {
        fprintf(stderr, "%d Invalid line : %s", lineNo, line);
        exit(EXIT_FAILURE);
    }
    if (line[0] == ' ') {
        fprintf(stderr, "%d Invalid line : %s", lineNo, line);
        exit(EXIT_FAILURE);
    }
    int countColon = 0;
    int countSpace = 0;
    int countChar = 0;
    for (unsigned int i = 0; i < size; i++) {
        if (line[i] == ':') {
            countColon++;
        } else if(line[i] == ' '){
            countSpace++;
        } else{
            // a b : d e f <= To fail these cases
            if(countSpace > 0 && countColon == 0){
                fprintf(stderr, "%d Invalid line : %s", lineNo, line);
                exit(EXIT_FAILURE);
            }
            // If target name is missing
            if(countColon > 0 && countChar == 0){
                fprintf(stderr, "%d Invalid line : %s", lineNo, line);
                exit(EXIT_FAILURE);
            }
            countChar++;
        }
    }
    if(countChar == 0){
        fprintf(stderr, "%d Invalid line : %s", lineNo, line);
        exit(EXIT_FAILURE);
    }
    if (countColon != 1) {
        fprintf(stderr, "%d Invalid line : %s", lineNo, line);
        exit(EXIT_FAILURE);
    }
}

char *stripWhiteSpace(char *str) {
    char *newStr = malloc(MAX_SIZE * sizeof(char));
    int i = 0;
    while (*str != '\0') {
        if (*str != ' ') {
            *(newStr + i++) = *str;
        }
        str++;

    }
    return newStr;
}

void validateCommands(char *line, unsigned int size, int lineNo) {
    if (size == 0) {
        fprintf(stderr, "%d Invalid line : %s", lineNo, line);
        exit(EXIT_FAILURE);
    }
    int countTab = 0;
    for (unsigned int i = 0; i < size; i++) {
        if (line[i] == '\t')
            countTab++;
    }

    if(countTab != 1){
        fprintf(stderr, "%d Invalid line : %s", lineNo, line);
        exit(EXIT_FAILURE);
    }
}
