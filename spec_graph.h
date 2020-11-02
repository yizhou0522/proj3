//
// Created by Arpit Jain on 11/1/18.
// Co-author Anshu Verma
//


#include "spec_repr.h"


typedef struct graphNode {
    char* element;
    int nodeNo;
    dep_node* dependencies;
    dep_node* commands;
    struct graphNode** children;
} graph_node;

graph_node* createGraphNode(char *element, dep_node* dependencies, dep_node* commands);

graph_node* constructGraph(graph_node* graphArray[], unsigned int size);

int isCycle(unsigned int size, graph_node *graphNodeArray[]);

typedef struct input {

    char* make_file_name;
    char** targets_to_build;

} input;

input init_input();

