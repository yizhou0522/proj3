//
// Created by Anshu on 11/3/18.
// Co-author Anshu Verma
//

// #include "constants.h"
#ifndef INC_537MAKE_VALIDATOR_H
#define INC_537MAKE_VALIDATOR_H

#endif //INC_537MAKE_VALIDATOR_H


void validateTarget(char *line, unsigned int size, int lineNo);

char *stripWhiteSpace(char *str);

void validateCommands(char *line, unsigned int size, int lineNo);

typedef struct llnode {
    char* element;
    struct llnode* next;
} dep_node;

dep_node *createLLNode(char *element);

void appendToLL(dep_node *node, char *element);

dep_node *getElement(dep_node *node, int index);