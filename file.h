struct File
{
    char *name;
    int size;
    struct File **children;
};

void cmd_find(char *pattern, struct File *root);
