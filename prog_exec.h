////////////////////////////////////////////////////////////////////////////////
// Main File:        (main.c)
// This File:        (prog_exec.h)
// Other Files:      (main.c, prog_exec.c, spec_graph.c, spec_graph.h,
//                    spec_repr.c, spec_repr.h, text_parser.c, text_parser.h,
//                    makefile)
//
// Semester:         CS 537 Fall 2020
// Instructor:       Barton Miller
// Author:           (Yizhou Liu, Yixing TU)
// Email:            (liu773@wisc.edu, ytu26@wisc.edu)
// CS Login:         (yizhou, yixingt)
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include "spec_graph.h"

bool processGraph(graph_node* root);
bool processNode(graph_node* root);
