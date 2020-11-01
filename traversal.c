//
// Created by Arpit Jain on 11/1/18.
// Co-author Anshu Verma
//

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <printf.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include "traversal.h"

bool traverseAndExecute(graph_node* root) {

    if (root->children[0] == NULL) {
        return executeNodeCommands(root);
    }

    for (unsigned int i = 0; i < MAX_SIZE; ++i) {
        if (root->children[i] == NULL) {
            break;
        }
        traverseAndExecute(root->children[i]);
    }

    return executeNodeCommands(root);
}