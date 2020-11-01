//
// Created by Arpit Jain on 11/1/18.
// Co-author Anshu Verma
//

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#endif //PROJECT_NODE_H

typedef struct llnode {
    char* element;
    struct llnode* next;
} linked_list_node;

linked_list_node *createLLNode(char *element);

void appendToLL(linked_list_node *node, char *element);

linked_list_node *getElement(linked_list_node *node, int index);