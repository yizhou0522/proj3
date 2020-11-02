//
// Created by Arpit Jain on 11/1/18.
// Co-author Anshu Verma
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "spec_graph.h"

#define MAX 2048


graph_node* createGraphNode(char *element, dep_node* dependencies, 
dep_node* commands) {

    graph_node* graphNode = (graph_node *) malloc(sizeof(graph_node));

    if(!graphNode){
        fprintf(stderr, "Could not allocate memory for LinkedList data");
        exit(1);
    }
    graphNode->element = element;
    graphNode->dependencies = dependencies;
    graphNode->commands = commands;
    graphNode->children = (graph_node**) malloc(MAX * sizeof(graphNode));
    return graphNode;
}

graph_node* createConnections(graph_node* graphArray[], unsigned int size){
    graph_node *root = NULL;

    for(unsigned int i = 0;i<size;i++){
        graphArray[i]->nodeNo = i;
    }
    for(unsigned int i = 0;i<size;i++){
        if(!root){
            root = graphArray[i];
        }
        dep_node *llNode = graphArray[i]->dependencies;
        int currChildCount = 0;
        while(llNode){
            int isfile = 0;
            int isTraget = 0;
            FILE *dependencyPointer = fopen(llNode->element, "r");
            if (dependencyPointer) {
                isfile = 1;
                fclose(dependencyPointer);
            }
             for (unsigned int j = 0; j < size; j++) {
                    if (strcmp(llNode->element, graphArray[j]->element) == 0) {
                        graphArray[i]->children[currChildCount++] = graphArray[j];
                        isTraget = 1;
                        break;
                    }
                }

            if(isfile == 0 && isTraget == 0){
                printf("537make: *** No rule to make target '%s', needed by '%s'.  Stop.\n",
                       llNode->element, graphArray[i]->element);
                exit(1);
            }
            llNode = llNode->next;
        }
    }
    return root;
}

int cyclic_util(int nodeNo, int visited[], int stack[], graph_node *graphNodeArray[], graph_node* node ){
    if(!visited[nodeNo]){
        visited[nodeNo] = 1;
        stack[nodeNo] = 1;
    }
    for(unsigned int i=0;i<MAX && node->children[i] != NULL;i++){
        if (!visited[node->children[i]->nodeNo] && cyclic_util(node->children[i]->nodeNo, visited, stack, graphNodeArray, graphNodeArray[nodeNo]->children[i])) {
            fprintf(stderr, "%s <= ", node->children[i]->element);
            return 1;
        }
        else if (stack[node->children[i]->nodeNo]) {
            fprintf(stderr, "%s <= ", node->children[i]->element);
            return 1;
        }
    }
    stack[node->nodeNo] = 0;
    return 0;
}

int is_cycle_found(unsigned int graphSize, graph_node *graphNodeArray[]){

    int visited[graphSize];
    int stack[graphSize];

    for(unsigned int i=0;i<graphSize;i++ ){
        visited[i] = 0;
        stack[i] = 0;
    }

    for(unsigned int i=0;i<graphSize;i++){
        if(cyclic_util(graphNodeArray[i]->nodeNo, visited, stack, graphNodeArray, graphNodeArray[i])){
            fprintf(stderr, " %s ", graphNodeArray[i]->element);
            return 1;
        }
    }
    return 0;
}

struct_input get_default_input_arg() {

    struct_input defaultInputArg;
    defaultInputArg.make_file_name = NULL;
    defaultInputArg.targets_to_build = (char **) malloc(MAX * sizeof(char *));
    defaultInputArg.targets_to_build[0] = NULL;
    return defaultInputArg;
}

