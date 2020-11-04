////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (spec_repr.h)
// Other Files:      (main.c, prog_exec.c, prog_exec.h, spec_graph.c,
//                    spec_graph.h, spec_repr.c, text_parser.c, text_parser.h,
//                    makefile)
//
// Semester:         CS 537 Fall 2020
// Instructor:       Barton Miller
// Author:           (Yizhou Liu, Yixing TU)
// Email:            (liu773@wisc.edu, ytu26@wisc.edu)
// CS Login:         (yizhou, yixingt)
////////////////////////////////////////////////////////////////////////////////
void checkTarget(char *line, unsigned int size, int lineNo);

char *deleteSpace(char *str);

void checkCmd(char *line, unsigned int size, int lineNo);

typedef struct depNode {
    char* value;
    struct depNode* next;
} dep_node;

dep_node *create(char *value);

void add(dep_node *node, char *value);
