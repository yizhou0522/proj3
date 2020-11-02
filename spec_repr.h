//
// Created by Anshu on 11/3/18.
// Co-author Anshu Verma
//

// #include "constants.h"
#ifndef INC_537MAKE_VALIDATOR_H
#define INC_537MAKE_VALIDATOR_H

#endif //INC_537MAKE_VALIDATOR_H


void checkTarget(char *line, unsigned int size, int lineNo);

char *deleteSpace(char *str);

void checkCmd(char *line, unsigned int size, int lineNo);

typedef struct depNode {
    char* element;
    struct depNode* next;
} dep_node;

dep_node *create(char *element);

void add(dep_node *node, char *element);
