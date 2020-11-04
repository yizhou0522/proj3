////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (spec_graph.h)
// Other Files:      (main.c, prog_exec.c, prog_exec.h, spec_graph.c,
//                    spec_repr.c, spec_repr.h, text_parser.c, text_parser.h,
//                    makefile)
//
// Semester:         CS 537 Fall 2020
// Instructor:       Barton Miller
// Author:           (Yizhou Liu, Yixing TU)
// Email:            (liu773@wisc.edu, ytu26@wisc.edu)
// CS Login:         (yizhou, yixingt)
////////////////////////////////////////////////////////////////////////////////
#include "spec_repr.h"

typedef struct graphNode {
    char* value;
    int num;
    dep_node* dependencies;
    dep_node* commands;
    struct graphNode** children;
} graph_node;

graph_node* createGraphNode(char *element, dep_node* dependencies, dep_node* commands);

graph_node* constructGraph(graph_node* graphArray[], unsigned int size);

int isCycle(unsigned int size, graph_node *graphNodeArray[]);

typedef struct input {

    char* name;
    char** spec_targets;

} input;

input init_input();

