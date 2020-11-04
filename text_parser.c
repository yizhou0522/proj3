////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (text_parser.c)
// Other Files:      (main.c, prog_exec.c, prog_exec.h, spec_graph.c,
//                    spec_graph.h, spec_repr.c, spec_repr.h, text_parser.h,
//                    makefile)
//
// Semester:         CS 537 Fall 2020
// Instructor:       Barton Miller
// Author:           (Yizhou Liu, Yixing TU)
// Email:            (liu773@wisc.edu, ytu26@wisc.edu)
// CS Login:         (yizhou, yixingt)
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include "text_parser.h"
#define MAX 2048

/*
 * This class looks for and reads the makefile then parse the lines in the
 * makefile. Also build the specification graph based on the dependences of the
 * targets found in the makefile.
 */
void parser(input parser_input) {
    FILE *file_pointer;
    char *line = (char *) malloc(MAX * sizeof(char));

    if (line == NULL) {
        fprintf(stderr, "<Reader input buffer allocation of memory failed.>");
        exit(1);
    }

    graph_node *graph[MAX];
    if(parser_input.name != NULL){
        file_pointer = fopen(parser_input.name, "r");
        if(!file_pointer){
            fprintf(stderr, "<Failed to find %s>", parser_input.name);
            exit(1);
        }
    } else {

        // Look for makefile then Makefile. If both are not present, print the error message.
        file_pointer = fopen("makefile", "r");
        if (!file_pointer) {
            file_pointer = fopen("Makefile", "r");
            if (!file_pointer) {
                fprintf(stderr, "<Both makefile and Makefile do not exist>");
                exit(1);
            }
        }
    }
    int curLine = 0;
    unsigned int curNode = 0;
    unsigned int index = 0;

    // Check each line in the makefile. Print error message when violation of makefile format occurs.
    graph_node *node = NULL;
    int character;
    do {
        curLine++;
        do {
            character = fgetc(file_pointer);
            if(character == '\0'){
                fprintf(stderr, "%d: <Contains NUll byte>: %s\n", curLine, line);
                exit(1);
            }
            line[index++] = (char) character;
        } while (character != '\n' && character != EOF && index < MAX);

        if (index >= MAX) {
            fprintf(stderr, "%d: <Exceeds the max line number %d>: %s\n", curLine, MAX, line);
            free(line);
            exit(1);
        } else {
            line[index - 1] = '\0';
        }

        char *token;

        if (line[0] == '\t') {
            checkCmd(line, index, curLine);
            token = strtok(line, "\t");
            if(!token){
                fprintf(stderr, "\n%d: <Not begin with tab>: %s\n", curLine, line);
                exit(1);
            }
            if (node == NULL) {
                fprintf(stderr, "\n%d: <Error>: %s\n", curLine, line);
                exit(1);
            } else {
                if (node->commands) {
                    add(node->commands, token);
                } else {
                    dep_node *llNode = create(token);
                    node->commands = llNode;
                }
            }
        } else if (line[0] == '#' || line[0] == '\0') {
            index = 0;
            free(line);
            line = (char *) malloc(MAX * sizeof(char));
            continue;
        } else {
            checkTarget(line, index, curLine);

            token = strtok(line, ":");
            if (!token) {
                fprintf(stderr, "%d: <Not target>: %s\n", curLine, line);
                exit(1);
            }

            char *file = malloc(sizeof(char) * MAX);

            strncpy(file, token, MAX);

            file = deleteSpace(file);

            token = strtok(NULL, " ");

            node = createGraphNode(file, NULL, NULL);
            graph[curNode++] = node;
            
            //Build the dependencies between nodes
            int height = 0;
            while (token != NULL) {
                if (strlen(token) > 0) {
                    height++;
                    if (!node->dependencies) {
                        dep_node *llNode = create(token);
                        node->dependencies = llNode;
                    } else {
                        add(node->dependencies, token);
                    }
                }

                token = strtok(NULL, " ");
            }
        }
        index = 0;
        free(line);
        line = (char *) malloc(MAX * sizeof(char));

    } while (character != EOF);

    free(line);

    if(curNode == 0){
        fprintf(stderr, "537make: <no target> stop the program.\n");
        exit(1);
    }

    //Build the graph
    constructGraph(graph, curNode);
    int cycleExist = isCycle(curNode, graph);

    if (cycleExist) {
        fprintf(stderr, "\n <Cycle detected>: Exit the program.\n");
        exit(1);
    }

    if(parser_input.spec_targets[0] == NULL){
        bool processed = processGraph(graph[0]);
        if (!processed) {
            printf("537make: '%s' is up to date.\n", graph[0]->value);
        }
    } else {
        for (unsigned int x = 0; x < MAX; x++) {
            if (parser_input.spec_targets[x] == NULL) {
                break;
            }
            int targetFound = 0;
            for (unsigned int i = 0; i < curNode; i++) {
                if (strcmp(graph[i]->value, parser_input.spec_targets[x]) == 0) {
                    targetFound = 1;
                    bool processed = processGraph(graph[i]);
                    if (!processed) {
                        printf("537make: '%s' is up to date.\n", graph[i]->value);
                    }
                    break;
                }
            }
            if (!targetFound) {
                fprintf(stderr, "Fail to find target %s", parser_input.spec_targets[x]);
            }
        }
    }

    if (fclose(file_pointer)) {
        fprintf(stderr, "Fail to close makefile or Makefile.");
        exit(1);
    }
}



