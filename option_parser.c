#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "option_parser.h"
#define MAX_SIZE 2048

struct_input parse_and_get_unprocessed_input(int argc, char **argv) {

    bool fileFind = false;
    int option;

    struct_input input = get_default_input_arg();

    while ((option = getopt(argc, argv, "f:")) != -1) {

        switch (option) {

            case 'f':
                
                if(fileFind){
                    input.make_file_name = optarg;
                }
                break;

            default: // Error Input Option
                fprintf(stderr, "Error: Format shoud be %s [-f <file_name>]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // INTERESTING ONE HERE :)
    // In case the user passes some redundant arguments, say 537ps -p 123 -v ABC
    // now, for this case, the count of arguments passed > the count of (option + option argument) read.
    // So, putting the same as a if clause to detect this anomaly and report error.
    if (argc > optind) {
        int i;
        if (!fileFind) {
            i = 1;
        } else {
            i = 3;
        }

        int j =0;
        for (; i < argc; ++i) {
            input.targets_to_build[j] = argv[i];
            j++;
        }
        input.targets_to_build[j] = NULL;
    }

    return input;
}