////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (main.c)
// Other Files:      (prog_exec.c, prog_exec.h, spec_graph.c, spec_graph.h,
//                    spec_repr.c, spec_repr.h, text_parser.c, text_parser.h,
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

/*
 * This main class takes valid command line arguments and checks if those
 * contain the -f option to specific the name of the makefile.
 */
int main(int argc, char *argv[]) {

    bool fileFind = false;
    int option;

    input ip = init_input();

    while ((option = getopt(argc, argv, "f:")) != -1) {

        switch (option) {

            case 'f':
                if(fileFind){
                    ip.name = optarg;
                }
                break;

            default: // Error Input Option
                fprintf(stderr,"<Error: Format shoud be %s [-f <file_name>]>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    //Deal with the case that users input unnecessary arguments.
    if (argc > optind) {
        int i;
        if (!fileFind) {
            i = 1;
        } else {
            i = 3;
        }

        int j =0;
        for (; i < argc; ++i) {
            ip.spec_targets[j] = argv[i];
            j++;
        }
        ip.spec_targets[j] = NULL;
    }

    parser(ip);

}
