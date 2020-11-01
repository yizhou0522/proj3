//
// Created by Arpit Jain on 11/3/18.
// Co-author Anshu Verma
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "option_parser.h"

struct_input parse_and_get_unprocessed_input(int argc, char **argv) {

    int optionAsInt;
    bool is_file_passed = false;

    struct_input unprocessedInputArg = get_default_input_arg();

    while ((optionAsInt = getopt(argc, argv, "f:")) != -1) {

        switch (optionAsInt) {

            case 'f':
                is_file_passed = true;
                unprocessedInputArg.make_file_name = optarg;
                break;

            default: // Error Input Option
                fprintf(stderr, "Usage: %s [-f <file_name>]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // INTERESTING ONE HERE :)
    // In case the user passes some redundant arguments, say 537ps -p 123 -v ABC
    // now, for this case, the count of arguments passed > the count of (option + option argument) read.
    // So, putting the same as a if clause to detect this anomaly and report error.
    if (argc > optind) {
        int i;
        if (is_file_passed) {
            i = 3;
        } else {
            i = 1;
        }

        int j =0;
        for (; i < argc; ++i) {
            unprocessedInputArg.targets_to_build[j] = argv[i];
            j++;
        }
        unprocessedInputArg.targets_to_build[j] = NULL;
    }

    return unprocessedInputArg;
}