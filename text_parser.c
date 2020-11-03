#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include "text_parser.h"
#define MAX 2048


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

        // Reading makefile
        file_pointer = fopen("makefile", "r");
        if (!file_pointer) {
            // In case readfile is not present. Trying reading Makefile
            file_pointer = fopen("Makefile", "r");
            // If this also fails then throw error
            if (!file_pointer) {
                fprintf(stderr, "<Both makefile and Makefile do not exist>");
                exit(1);
            }
        }
    }
    int curLine = 0;
    unsigned int curNode = 0;
    unsigned int index = 0;
    // Read line by line the contents of the file
    graph_node *node = NULL;
    int character;
    do {
        curLine++;
        // Concatente everything line;
        do {
            character = fgetc(file_pointer);
            if(character == '\0'){
                fprintf(stderr, "%d: <Contains NUll byte>: %s\n", curLine, line);
                exit(1); // fix me: add exit program
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
            // Commands
            checkCmd(line, index, curLine);

            token = strtok(line, "\t");
            if(!token){
                fprintf(stderr, "\n%d: <Not begin with tab>: %s\n", curLine, line);
                exit(1);
            }
            if (node == NULL) {
                fprintf(stderr, "\n%d: <Error>: %s\n", curLine, line); //fix me: not sure the error message
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
            // Now check if it's a target or not

            token = strtok(line, ":");
            // Line which is not comment, target or command
            if (!token) {
                fprintf(stderr, "%d: <Not target>: %s\n", curLine, line);
                exit(1);
            }

            char *targetName = malloc(sizeof(char) * MAX);

            strncpy(targetName, token, MAX);


            targetName = deleteSpace(targetName);

            token = strtok(NULL, " ");

            // New Target found. Create a new graph node.
            node = createGraphNode(targetName, NULL, NULL);
            graph[curNode++] = node;

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



