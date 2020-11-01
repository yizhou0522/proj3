//
// Created by Arpit Jain on 11/1/18.
// Co-author Anshu Verma
//

#ifndef INC_537MAKE_GRAPHNODE_H
#define INC_537MAKE_GRAPHNODE_H

#endif //INC_537MAKE_GRAPHNODE_H

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H
#include "linked_list.h"
#endif

#ifndef INC_537MAKE_CONSTANTS_H
#define INC_537MAKE_CONSTANTS_H
#include "constants.h"
#endif

typedef struct graphNode {

    //TODO: Change it to target name
    char* element;
    int nodeNo;
    linked_list_node* dependencies;
    linked_list_node* commands;
    struct graphNode** children;
} graph_node;

graph_node* createGraphNode(char *element, linked_list_node* dependencies, linked_list_node* commands);

graph_node* createConnections(graph_node* graphArray[], unsigned int size);

int is_cycle_found(unsigned int size, graph_node *graphNodeArray[]);