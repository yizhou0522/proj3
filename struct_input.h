//
// Created by Arpit Jain on 11/3/18.
// Co-author Anshu Verma
//

#ifndef INC_537MAKE_STRUCT_INPUT_H
#define INC_537MAKE_STRUCT_INPUT_H

#endif //INC_537MAKE_STRUCT_INPUT_H

#ifndef INC_537MAKE_CONSTANTS_H
#define INC_537MAKE_CONSTANTS_H
#include "constants.h"
#endif

typedef struct struct_input {

    char* make_file_name;
    char** targets_to_build;

} struct_input;

struct_input get_default_input_arg();