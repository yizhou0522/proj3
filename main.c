#include <stdio.h>
#include "reader.h"
// #include "option_parser.h"

int main(int argc, char *argv[]) {

    struct_input unprocessedInput = parse_and_get_unprocessed_input(argc, argv);
    reader(unprocessedInput);

}