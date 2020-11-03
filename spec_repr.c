//
// Created by Anshu on 11/3/18.
// Co-author Anshu Verma
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "spec_repr.h"
#define MAX 2048

dep_node *create(char *element) {

    dep_node* depNode = (dep_node *) malloc(sizeof(dep_node));

    if(!depNode){
        fprintf(stderr, "Could not allocate memory for dependencylist \n");
        exit(1);
    }

    depNode->element = malloc(MAX * sizeof(char));
    strncpy(depNode->element, element, MAX);
    depNode->next = NULL;
    return depNode;
}

void add(dep_node *node, char *element) {

    if (node == NULL) {
        fprintf(stderr, "Null node passed to add function\n");
        exit(1);
    }

    dep_node* newNode = create(element);

    dep_node* tempHead = node;
    while (tempHead -> next != NULL) {
        tempHead = tempHead->next;
    }

    tempHead->next = newNode;
    newNode->next = NULL;
}


void checkTarget(char *line, unsigned int size, int lineNo) {
    if (size == 0) {
        fprintf(stderr, "%d <Invalid index> : %s", lineNo, line);
        exit(1);
    }
    if (line[0] == ' ') {
        fprintf(stderr, "%d <No tab at the start of command line> : %s\n", lineNo, line);
        exit(1);
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
                fprintf(stderr, "%d <No colon after the target> : %s\n", lineNo, line);
                exit(1);
            }
            // If target name is missing
            if(countColon > 0 && countChar == 0){
                fprintf(stderr, "%d <Target name missing> : %s\n", lineNo, line);
                exit(1);
            }
            countChar++;
        }
    }
    if(countChar == 0){
        fprintf(stderr, "%d <No colon after the target> : %s\n", lineNo, line);
        exit(1);
    }
    if (countColon != 1) {
        fprintf(stderr, "%d <No colon after the target> : %s\n", lineNo, line);
        exit(1);
    }
}

char *deleteSpace(char *str) {
    char *newStr = malloc(MAX * sizeof(char));
    int i = 0;
    while (*str != '\0') {
        if (*str != ' ') {
            *(newStr + i++) = *str;
        }
        str++;

    }
    return newStr;
}

void checkCmd(char *line, unsigned int size, int lineNo) {
    if (size == 0) {
        fprintf(stderr, "%d <Invalid index> : %s\n", lineNo, line);
        exit(1);
    }
    int countTab = 0;
    for (unsigned int i = 0; i < size; i++) {
        if (line[i] == '\t')
            countTab++;
    }

    if(countTab != 1){
        fprintf(stderr, "%d <No tab start in command line> : %s\n", lineNo, line);
        exit(1);
    }
}
