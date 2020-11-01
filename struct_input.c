//
// Created by Arpit Jain on 11/3/18.
// Co-author Anshu Verma
//

#include <stddef.h>
#include <stdlib.h>
#include "struct_input.h"

struct_input get_default_input_arg() {

    struct_input defaultInputArg;
    defaultInputArg.make_file_name = NULL;
    defaultInputArg.targets_to_build = (char **) malloc(MAX_SIZE * sizeof(char *));
    defaultInputArg.targets_to_build[0] = NULL;
    return defaultInputArg;
}
