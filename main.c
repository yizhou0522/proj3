#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include "text_parser.h"

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
                fprintf(stderr, "<Error: Format shoud be %s [-f <file_name>]>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
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