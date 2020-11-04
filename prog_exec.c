////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (prog_exec.c)
// Other Files:      (main.c, prog_exec.h, spec_graph.c, spec_graph.h,
//                    spec_repr.c, spec_repr.h, text_parser.c, text_parser.h,
//                    makefile)
//
// Semester:         CS 537 Fall 2020
// Instructor:       Barton Miller
// Author:           (Yizhou Liu, Yixing TU)
// Email:            (liu773@wisc.edu, ytu26@wisc.edu)
// CS Login:         (yizhou, yixingt)
////////////////////////////////////////////////////////////////////////////////
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

/*
 * This class is used to create process, run each process, and wait for
 * processes to complete.
 */

//this function is used to process graph
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

//this function compares the time of two parameters
bool checkTime(struct timespec timespec_1, struct timespec timespec_2) {

    if (timespec_1.tv_sec == timespec_2.tv_sec)
        return timespec_1.tv_nsec < timespec_2.tv_nsec;
    else
        return timespec_1.tv_sec < timespec_2.tv_sec;
}

//this function checks potential problems during execution
bool checkExecution(graph_node* root) {

    if(root->dependencies!= NULL) {

        struct stat targetStat;
        FILE *fptr = fopen(root->value, "r");

        if (!fptr) {
            return true;
        } else {

            fstat(fileno(fptr), &targetStat);
            fclose(fptr);
            struct timespec targetLmd = targetStat.st_mtim;

            dep_node* dep = root->dependencies;
            while(dep) {
                struct stat depStat;
                FILE *depPtr = fopen(dep->value, "r");
                if (!depPtr) {
                    fprintf(stderr, 
              "<537make: No rule to make target '%s', needed by '%s'. Exit.>\n",
                            dep->value, root->value);
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

//this function checks processes and execute input/output redirction 
// based on nodes
bool processNode(graph_node* root) {

    if(checkExecution(root)) {
        dep_node* tmp = root->commands;
        while (tmp != NULL) {
            pid_t pid = fork();
            if (pid == -1) { 
                fprintf(stderr, "Failed to fork(). Terminating at once.\n");
                exit(1);
            }

            else if (pid > 0) { 
                int status;
                waitpid(pid, &status, 0);
                if (status != 0) {
                    fprintf(stderr, 
                    "<537make: recipe for target '%s' failed>\n", root->value);
                    fprintf(stderr, 
                    "<537make: *** [%s] Error %d>\n", root->value, status);
                    exit(1);
                }
                tmp = tmp->next;
            }

            else if (pid == 0) { 
                bool Iredirection, Oredirection = false;
                char* fIdir;
                char* fOredir;
                char copy[MAX];
                strncpy(copy, tmp->value, MAX);
                char *argv[MAX];
                for(unsigned int i = 0; i < MAX; i++) {
                    argv[i] = NULL;
                }
                int i = 0;
                char *res = strtok(copy, " ");
                while (res) {
                    if (strcmp(res, "<") == 0) {
                        Iredirection = true;
                        fIdir = strtok(NULL, " ");
                        res = strtok(NULL, " ");
                        i++;
                    } else if (strcmp(res, ">") == 0) {
                        Oredirection = true;
                        fOredir = strtok(NULL, " ");
                        res = strtok(NULL, " ");
                        i++;
                    } else {
                        argv[i] = res;
                        i++;
                        res = strtok(NULL, " ");
                    }
                }
                argv[i] = NULL;

                if (argv[0] == NULL) {
                    fprintf(stderr, 
                    "<NULL passed as a command for execution>\n");
                    exit(1);
                }
                if (Iredirection) {
                    int in = open(fIdir, O_RDONLY);
                    dup2(in, STDIN_FILENO);
                    close(in);
                }
                if (Oredirection) {
                    int out = open(fOredir, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
                    dup2(out, STDOUT_FILENO);
                    close(out);
                }
                execvp(argv[0], argv);
                exit(1);
            }
            else {
                fprintf(stderr, "<PID can not be negative. Exit.>");
                _exit(0);
            }
        }
        return true;
    }
    return false;
}
