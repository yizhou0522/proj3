


void checkTarget(char *line, unsigned int size, int lineNo);

char *deleteSpace(char *str);

void checkCmd(char *line, unsigned int size, int lineNo);

typedef struct depNode {
    char* value;
    struct depNode* next;
} dep_node;

dep_node *create(char *value);

void add(dep_node *node, char *value);
