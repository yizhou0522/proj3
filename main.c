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

    // struct_input unprocessedInput = parse_and_get_unprocessed_input(argc, argv);
    reader(input);

}