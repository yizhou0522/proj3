#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include "text_parser.h"
#define MAX_SIZE 2048


void reader(struct_input unprocessedInput) {
    FILE *file_pointer;
    char *line = (char *) malloc(MAX_SIZE * sizeof(char));

    if (line == NULL) {
        fprintf(stderr, "Reader input buffer allocation of memory failed.");
        exit(EXIT_FAILURE);
    }

    graph_node *graph[MAX_SIZE];
    if(unprocessedInput.make_file_name != NULL){
        file_pointer = fopen(unprocessedInput.make_file_name, "r");
        if(!file_pointer){
            fprintf(stderr, "Failed to find %s", unprocessedInput.make_file_name);
            exit(EXIT_FAILURE);
        }
    } else {

        // Reading makefile
        file_pointer = fopen("makefile", "r");
        if (!file_pointer) {
            // In case readfile is not present. Trying reading Makefile
            file_pointer = fopen("Makefile", "r");
            // If this also fails then throw error
            if (!file_pointer) {
                fprintf(stderr, "Both makefile and Makefile do not exist");
                exit(EXIT_FAILURE);
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
                fprintf(stderr, "\n%d: <Contains NUll byte>: %s\n", curLine, line);
                exit(EXIT_FAILURE); // fix me: add exit program
            }
            line[index++] = (char) character;
        } while (character != '\n' && character != EOF && index < MAX_SIZE);

        if (index >= MAX_SIZE) {
            fprintf(stderr, "\n%d: <Over the maximum line length %d>: %s\n", curLine, MAX_SIZE, line);
            free(line);
            exit(EXIT_FAILURE);
        } else {
            line[index - 1] = '\0';
        }

        char *token;

        if (line[0] == '\t') {
            // Commands
            validateCommands(line, index, curLine);

            token = strtok(line, "\t");
            if(!token){
                fprintf(stderr, "\n%d: <Not begin with tab>: %s\n", curLine, line);
                exit(EXIT_FAILURE);
            }
            if (node == NULL) {
                fprintf(stderr, "\n%d: <Error>: %s\n", curLine, line); //fix me: not sure the error message
                exit(EXIT_FAILURE);
            } else {
                if (node->commands) {
                    appendToLL(node->commands, token);
                } else {
                    linked_list_node *llNode = createLLNode(token);
                    node->commands = llNode;
                }
            }

        } else if (line[0] == '#' || line[0] == '\0') {
            index = 0;
            free(line);
            line = (char *) malloc(MAX_SIZE * sizeof(char));
            continue;
        } else {
            validateTarget(line, index, curLine);
            // Now check if it's a target or not

            token = strtok(line, ":");
            // Line which is not comment, target or command
            if (!token) {
                fprintf(stderr, "%d: <Not target>: %s\n", curLine, line);
                exit(EXIT_FAILURE);
            }

            char *targetName = malloc(sizeof(char) * MAX_SIZE);

            strncpy(targetName, token, MAX_SIZE);
            targetName = stripWhiteSpace(targetName);

            token = strtok(NULL, " ");

            // New Target found. Create a new graph node.
            node = createGraphNode(targetName, NULL, NULL);
            graph[curNode++] = node;

            int height = 0;
            while (token != NULL) {
                if (strlen(token) > 0) {
                    height++;
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

    } while (character != EOF);

    free(line);

    if(curNode == 0){
        fprintf(stderr, "537make: <no target> stop the program.\n");
        exit(EXIT_FAILURE);
    }

    createConnections(graph, curNode);
    int cycleExist = is_cycle_found(curNode, graph);

    if (cycleExist) {
        fprintf(stderr, "\n<Cyclic dependency exists>: Terminate the program.\n");
        exit(EXIT_FAILURE);
    }

    if(unprocessedInput.targets_to_build[0] == NULL){
        bool processed = traverseAndExecute(graph[0]);
        if (!processed) {
            printf("537make: '%s' is up to date.\n", graph[0]->element);
        }
    } else {
        for (unsigned int x = 0; x < MAX_SIZE; x++) {
            if (unprocessedInput.targets_to_build[x] == NULL) {
                break;
            }
            int targetFound = 0;
            for (unsigned int i = 0; i < curNode; i++) {
                if (strcmp(graph[i]->element, unprocessedInput.targets_to_build[x]) == 0) {
                    targetFound = 1;
                    bool processed = traverseAndExecute(graph[i]);
                    if (!processed) {
                        printf("537make: '%s' is up to date.\n", graph[i]->element);
                    }
                    break;
                }
            }
            if (!targetFound) {
                fprintf(stderr, "Fail to find target %s", unprocessedInput.targets_to_build[x]);
            }
        }
    }


    if (fclose(file_pointer)) {
        fprintf(stderr, "Fail to close makefile or Makefile.");
        exit(EXIT_FAILURE);
    }
}



