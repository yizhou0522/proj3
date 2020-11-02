//
// Created by Arpit Jain on 11/1/18.
// Co-author Anshu Verma
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"

#define MAX_SIZE 2048

linked_list_node *createLLNode(char *element) {

    linked_list_node* llNode = (linked_list_node *) malloc(sizeof(linked_list_node));

    if(!llNode){
        fprintf(stderr, "Could not allocate memory for LinkedList data");
        exit(EXIT_FAILURE);
    }

    llNode->element = malloc(MAX_SIZE * sizeof(char));
    strncpy(llNode->element, element, MAX_SIZE);
    llNode->next = NULL;
    return llNode;
}

void appendToLL(linked_list_node *node, char *element) {

    if (node == NULL) {
        fprintf(stderr, "Null node passed to appendToLL function");
        exit(EXIT_FAILURE);
    }

    linked_list_node* newNode = createLLNode(element);

    linked_list_node* tempHead = node;
    while (tempHead -> next != NULL) {
        tempHead = tempHead->next;
    }

    tempHead->next = newNode;
    newNode->next = NULL;
}

