//
// Created by Anshu on 11/2/18.
// Co-author Arpit Jain
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "reader.h"
#include "validator.h"


void reader(struct_input unprocessedInput) {
    FILE *file_pointer;
    char *line = (char *) malloc(MAX_SIZE * sizeof(char));

    if (line == NULL) {
        fprintf(stderr, "Unable to allocate memory for reader input buffer");
        exit(EXIT_FAILURE);
    }

    graph_node *graphNodeArray[MAX_SIZE];
    if(unprocessedInput.make_file_name != NULL){
        file_pointer = fopen(unprocessedInput.make_file_name, "r");
        if(!file_pointer){
            fprintf(stderr, "Could not find %s", unprocessedInput.make_file_name);
            exit(EXIT_FAILURE);
        }
    } else {

        // Reading Makefile
        file_pointer = fopen("makefile", "r");
        if (!file_pointer) {
            // In case readfile is not present. Trying reading Makefile
            file_pointer = fopen("Makefile", "r");
            // If this also fails then throw error
            if (!file_pointer) {
                fprintf(stderr, "Could not find makefile or Makefile");
                exit(EXIT_FAILURE);
            }
        }
    }
    int lineNo = 0;
    unsigned int curNode = 0;
    unsigned int index = 0;
    // Read line by line the contents of the file
    graph_node *node = NULL;
    int c;
    do {
        lineNo++;
        // Concatente everything line;
        do {
            c = fgetc(file_pointer);
            if(c == '\0'){
                fprintf(stderr, "Null character encountered at line No %d. Exiting the program", lineNo);
            }
            line[index++] = (char) c;
        } while (c != '\n' && c != EOF && index < MAX_SIZE);

        if (index >= MAX_SIZE) {
            fprintf(stderr, "THIS LINE EXHAUSTED THE BUFFER SIZE %d. Terminating the program\n", MAX_SIZE);
            free(line);
            exit(EXIT_FAILURE);
        } else {
            line[index - 1] = '\0';
        }

        char *token;

        if (line[0] == '\t') {
            // Commands
            validateCommands(line, index, lineNo);

            token = strtok(line, "\t");
            if(!token){
                fprintf(stderr, "\n%d Invalid line : %s\n", lineNo, line);
                exit(EXIT_FAILURE);
            }
            if (node == NULL) {
                fprintf(stderr, "\n%d Invalid line : %s\n", lineNo, line);
                exit(EXIT_FAILURE);
            } else {
                if (!node->commands) {
                    linked_list_node *llNode = createLLNode(token);
                    node->commands = llNode;
                } else {
                    appendToLL(node->commands, token);
                }
            }

        } else if (line[0] == '#' || line[0] == '\0') {
            index = 0;
            free(line);
            line = (char *) malloc(MAX_SIZE * sizeof(char));
            continue;
        } else {
            validateTarget(line, index, lineNo);
            // Now check if it's a target or not

            token = strtok(line, ":");
            // Line which is not comment, target or command
            if (!token) {
                fprintf(stderr, "%d: Invalid line : %s\n", lineNo, line);
                exit(EXIT_FAILURE);
            }

            char *targetName = malloc(sizeof(char) * MAX_SIZE);

            strncpy(targetName, token, MAX_SIZE);
            targetName = stripWhiteSpace(targetName);

            token = strtok(NULL, " ");

            // New Target found. Create a new graph node.
            node = createGraphNode(targetName, NULL, NULL);
            graphNodeArray[curNode++] = node;

            int total_dep = 0;
            while (token != NULL) {
                if (strlen(token) > 0) {
                    total_dep++;
                    if (!node->dependencies) {
                        linked_list_node *llNode = createLLNode(token);
                        node->dependencies = llNode;
                    } else {
                        appendToLL(node->dependencies, token);
                    }
                }

                token = strtok(NULL, " ");
            }
        }
        index = 0;
        free(line);
        line = (char *) malloc(MAX_SIZE * sizeof(char));

    } while (c != EOF);

    free(line);

    if(curNode == 0){
        fprintf(stderr, "537make: * No targets.  Stop.\n");
        exit(EXIT_FAILURE);
    }

    createConnections(graphNodeArray, curNode);
    int isCycleFound = is_cycle_found(curNode, graphNodeArray);

    if (isCycleFound) {
        fprintf(stderr, "\nCyclic dependency found. Terminating.\n");
        exit(EXIT_FAILURE);
    }

    if(unprocessedInput.targets_to_build[0] == NULL){
        bool executed = traverseAndExecute(graphNodeArray[0]);
        if (!executed) {
            printf("537make: '%s' is up to date.\n", graphNodeArray[0]->element);
        }
    } else {
        for (unsigned int x = 0; x < MAX_SIZE; x++) {
            if (unprocessedInput.targets_to_build[x] == NULL) {
                break;
            }
            int targetFound = 0;
            for (unsigned int i = 0; i < curNode; i++) {
                if (strcmp(graphNodeArray[i]->element, unprocessedInput.targets_to_build[x]) == 0) {
                    targetFound = 1;
                    bool executed = traverseAndExecute(graphNodeArray[i]);
                    if (!executed) {
                        printf("537make: '%s' is up to date.\n", graphNodeArray[i]->element);
                    }
                    break;
                }
            }
            if (!targetFound) {
                fprintf(stderr, "Specified target %s not found", unprocessedInput.targets_to_build[x]);
            }
        }
    }


    if (fclose(file_pointer)) {
        fprintf(stderr, "Failed to close makefile or Makefile");
        exit(EXIT_FAILURE);
    }
}

