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

#define MAX 2048

bool processGraph(graph_node* root) {

    if (root->children[0] == NULL) {
        return processNode(root);
    }
    unsigned int i=0;

    while(i < MAX) {
        if (root->children[i] == NULL) {
            break;
        }
        processGraph(root->children[i]);
        i++;
    }

    return processNode(root);
}

bool checkTime(struct timespec lhs, struct timespec rhs) {

    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec < rhs.tv_nsec;
    else
        return lhs.tv_sec < rhs.tv_sec;
}

bool checkExecution(graph_node* root) {

    if(root->dependencies!= NULL) {

        struct stat targetStat;
        FILE *fptr = fopen(root->element, "r");

        if (!fptr) {
            return true;
        } else {

            fstat(fileno(fptr), &targetStat);
            fclose(fptr);
            struct timespec targetLmd = targetStat.st_mtim;

            dep_node* dep = root->dependencies;
            while(dep) {
                struct stat depStat;
                FILE *depPtr = fopen(dep->element, "r");
                if (!depPtr) {
                    fprintf(stderr, "537make: No rule to make target '%s', needed by '%s'.  Stop.\n",
                            dep->element, root->element);
                    exit(1);
                } else {
                    fstat(fileno(depPtr), &depStat);
                    fclose(depPtr);
                    if(checkTime(targetLmd, depStat.st_mtim)) {
                        return true;
                    }
                }
                dep = dep->next;
            }
            return false;
        }
    }
    return true;
}

bool processNode(graph_node* root) {

    if(checkExecution(root)) {
        dep_node* tmp = root->commands;
        while (tmp != NULL) {
            pid_t pid = fork();
            if (pid == -1) { // Error, failed to fork()
                fprintf(stderr, "Failed to fork(). Terminating at once.\n");
                exit(1);
            }

            else if (pid > 0) { // Parent process
                int status;
                waitpid(pid, &status, 0);
                if (status != 0) {
                    fprintf(stderr, "537make: recipe for target '%s' failed\n", root->element);
                    fprintf(stderr, "537make: *** [%s] Error %d\n", root->element, status);
                    exit(1);
                }
                tmp = tmp->next;
            }

            else if (pid == 0) { // Child process

                bool Iredirection, Oredirection = false;
                char* fIdir;
                char* fOredir;

                char copy[MAX];
                strncpy(copy, tmp->element, MAX);
                char *argv[MAX];
                for(unsigned int i = 0; i < MAX; i++) {
                    argv[i] = NULL;
                }
                int i = 0;
                char *split = strtok(copy, " ");
                while (split) {

                    if (strcmp(split, "<") == 0) {
                        // If input redirection detected, read the file for input redirection.
                        Iredirection = true;
                        fIdir = strtok(NULL, " ");
                        split = strtok(NULL, " ");
                        i++;
                    } else if (strcmp(split, ">") == 0) {
                        // If output redirection detected, read the file for output redirection.
                        Oredirection = true;
                        fOredir = strtok(NULL, " ");
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

                if (Iredirection) {

                    int in = open(fIdir, O_RDONLY);

                    // Replacing standard input with input file
                    dup2(in, STDIN_FILENO);

                    // Closing unused file descriptors
                    close(in);
                }

                if (Oredirection) {

                    int out = open(fOredir, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

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
                fprintf(stderr, "PID can not be negative. Exiting now..");
                _exit(0);
            }
        }

        return true;
    }

    return false;
}
