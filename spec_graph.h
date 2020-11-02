//
// Created by Arpit Jain on 11/1/18.
// Co-author Anshu Verma
//

#ifndef INC_537MAKE_GRAPHNODE_H
#define INC_537MAKE_GRAPHNODE_H

#endif //INC_537MAKE_GRAPHNODE_H


#ifndef INC_537MAKE_VALIDATOR_H
#define INC_537MAKE_VALIDATOR_H
#include "spec_repr.h"
#endif


// #ifndef INC_537MAKE_CONSTANTS_H
// #define INC_537MAKE_CONSTANTS_H
// #include "constants.h"
// #endif

typedef struct graphNode {

    //TODO: Change it to target name
    char* element;
    int nodeNo;
    dep_node* dependencies;
    dep_node* commands;
    struct graphNode** children;
} graph_node;

graph_node* createGraphNode(char *element, dep_node* dependencies, dep_node* commands);

graph_node* createConnections(graph_node* graphArray[], unsigned int size);

int is_cycle_found(unsigned int size, graph_node *graphNodeArray[]);

typedef struct struct_input {

    char* make_file_name;
    char** targets_to_build;

} struct_input;

struct_input get_default_input_arg();

