


void checkTarget(char *line, unsigned int size, int lineNo);

char *deleteSpace(char *str);

void checkCmd(char *line, unsigned int size, int lineNo);

typedef struct depNode {
    char* element;
    struct depNode* next;
} dep_node;

dep_node *create(char *element);

void add(dep_node *node, char *element);
