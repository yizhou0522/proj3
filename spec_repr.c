////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (spec_repr.c)
// Other Files:      (main.c, prog_exec.c, prog_exec.h, spec_graph.c,
//                    spec_graph.h, spec_repr.h, text_parser.c, text_parser.h,
//                    makefile)
//
// Semester:         CS 537 Fall 2020
// Instructor:       Barton Miller
// Author:           (Yizhou Liu, Yixing TU)
// Email:            (liu773@wisc.edu, ytu26@wisc.edu)
// CS Login:         (yizhou, yixingt)
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "spec_repr.h"
#define MAX 2048

/*
 * This class checkes the executable files and create linked list node if it is
 * valid.
 */

/*
 * This method is used to create linked list node and returns the pointer to the
 * new created node.
 */
dep_node *create(char *value) {

    dep_node* depNode = (dep_node *) malloc(sizeof(dep_node));

    if(!depNode){
        fprintf(stderr, "<Could not allocate memory for dependencylist> \n");
        exit(1);
    }

    depNode->value = malloc(MAX * sizeof(char));
    strncpy(depNode->value, value, MAX);
    depNode->next = NULL;
    return depNode;
}

/*
 * This method is used to add nodes to the graph structure.
 */
void add(dep_node *node, char *value) {

    if (node == NULL) {
        fprintf(stderr, "<Null node passed to add function>\n");
        exit(1);
    }

    dep_node* newNode = create(value);

    dep_node* tempHead = node;
    while (tempHead -> next != NULL) {
        tempHead = tempHead->next;
    }

    tempHead->next = newNode;
    newNode->next = NULL;
}

/*
 * This method check if the target executable file is valid. Print error
 * message when necessary.
 */
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
            if(countSpace > 0 && countColon == 0){
                fprintf(stderr, "%d <No colon after the target> : %s\n", lineNo, line);
                exit(1);
            }
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

/*
 * This method delete the unnecessary space in a line
 */
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

/*
 * This method is used to check if the command arguments are valid.
 */
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
