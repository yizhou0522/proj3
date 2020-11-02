//
// Created by Arpit Jain on 11/3/18.
// Co-author Anshu Verma
//

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <printf.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include "prog_exec.h"

#define MAX_SIZE 2048

bool traverseAndExecute(graph_node* root) {

    if (root->children[0] == NULL) {
        return executeNodeCommands(root);
    }

    for (unsigned int i = 0; i < MAX_SIZE; ++i) {
        if (root->children[i] == NULL) {
            break;
        }
        traverseAndExecute(root->children[i]);
    }

    return executeNodeCommands(root);
}

bool isLHSLessThanRHS(struct timespec lhs, struct timespec rhs) {

    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec < rhs.tv_nsec;
    else
        return lhs.tv_sec < rhs.tv_sec;
}

bool commandExecutionRequired(graph_node* root) {

    if(root->dependencies != NULL) {

        struct stat targetStat;
        FILE *targetPointer = fopen(root->element, "r");

        if (!targetPointer) {
            // Target file does not exists
            return true;
        } else {

            fstat(fileno(targetPointer), &targetStat);
            fclose(targetPointer);
            struct timespec targetLmd = targetStat.st_mtim;

            linked_list_node* dependencies = root->dependencies;
            while(dependencies) {
                struct stat dependencyStat;
                FILE *dependencyPointer = fopen(dependencies->element, "r");

                if (!dependencyPointer) {
                    // Dependency file does not exists - THROW ERROR
                    fprintf(stderr, "537make: *** No rule to make target '%s', needed by '%s'.  Stop.\n",
                            dependencies->element, root->element);
                    exit(1);
                } else {
                    fstat(fileno(dependencyPointer), &dependencyStat);
                    fclose(dependencyPointer);
                    struct timespec dependencyLmd = dependencyStat.st_mtim;

                    if(isLHSLessThanRHS(targetLmd, dependencyLmd)) {
                        return true;
                    }
                }

                dependencies = dependencies->next;
            }

            return false;
        }
    }

    return true;
}

bool executeNodeCommands(graph_node* root) {

    if(commandExecutionRequired(root)) {
        linked_list_node* temphead = root->commands;
        while (temphead != NULL) {
            pid_t pid = fork();
            if (pid == -1) { // Error, failed to fork()
                fprintf(stderr, "Failed to fork(). Terminating at once.\n");
                exit(1);
            }

            else if (pid > 0) { // Parent process
                int status;
                waitpid(pid, &status, 0);
                if (status != EXIT_SUCCESS) {

                    fprintf(stderr, "537make: recipe for target '%s' failed\n", root->element);
                    fprintf(stderr, "537make: *** [%s] Error %d\n", root->element, status);
                    exit(1);
                }
                temphead = temphead->next;
            }

            else if (pid == 0) { // Child process

                bool inputRedirection = false;
                bool outputRedirection = false;
                char* file_name_input_direction;
                char* file_name_output_redirection;

                char copiedCmd[MAX_SIZE];
                strncpy(copiedCmd, temphead->element, MAX_SIZE);
                char *argv[MAX_SIZE];
                for(unsigned int i = 0; i < MAX_SIZE; i++) {
                    argv[i] = NULL;
                }
                int i = 0;
                char *split = strtok(copiedCmd, " ");
                while (split) {

                    if (strcmp(split, "<") == 0) {
                        // If input redirection detected, read the file for input redirection.
                        inputRedirection = true;
                        file_name_input_direction = strtok(NULL, " ");
                        split = strtok(NULL, " ");
                        i++;
                    } else if (strcmp(split, ">") == 0) {
                        // If output redirection detected, read the file for output redirection.
                        outputRedirection = true;
                        file_name_output_redirection = strtok(NULL, " ");
                        split = strtok(NULL, " ");
                        i++;
                    } else {
                        argv[i] = split;
                        i++;
                        split = strtok(NULL, " ");
                    }
                }
                argv[i] = NULL;

                if (argv[0] == NULL) {
                    fprintf(stderr, "NULL passed as a command for execution\n");
                    exit(1);
                }

                if (inputRedirection) {

                    int in = open(file_name_input_direction, O_RDONLY);

                    // Replacing standard input with input file
                    dup2(in, STDIN_FILENO);

                    // Closing unused file descriptors
                    close(in);
                }

                if (outputRedirection) {

                    int out = open(file_name_output_redirection, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

                    // Replacing standard output with output file
                    dup2(out, STDOUT_FILENO);

                    // Closing unused file descriptors
                    close(out);
                }

                execvp(argv[0], argv);
                // The exec() functions only return if an error has occurred.
                // The return value is -1, and errno is set to indicate the error.

                exit(1);
            }

            else { // IMPOSSIBLE ZONE
                fprintf(stderr, "PID can not be negative. Terminating at once.");
                _exit(EXIT_SUCCESS);
            }
        }

        return true;
    }

    return false;
}
