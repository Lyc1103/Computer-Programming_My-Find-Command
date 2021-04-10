#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file.h"

#define FULL_FILENAME_SIZE 100005

bool check(const char *pattern, const char *target)
{
    if (*pattern == '\0')
    {
        return *target == '\0';
    }
    else if (*pattern == *target || *pattern == '?')
    {
        return (*target != '\0' && check(pattern + 1, target + 1));
    }
    else if (*pattern == '*')
    {
        return check(pattern + 1, target) || (*target != '\0' && check(pattern, target + 1));
    }

    return false;
}

void recursion_find(char *pattern, struct File *current_file, char *filename)
{
    if (check(pattern, current_file->name))
    {
        printf("%s\n", filename);
    }
    for (int i = 0; i < current_file->size; i++)
    {
        strcat(filename, "/");
        strcat(filename, current_file->children[i]->name);
        recursion_find(pattern, current_file->children[i], filename);

        // reset
        filename[strlen(filename) - 1 - strlen(current_file->children[i]->name)] = 0;
    }
}

void cmd_find(char *pattern, struct File *root)
{
    char filename[FULL_FILENAME_SIZE];
    strcpy(filename, ".");
    recursion_find(pattern, root, filename);
    return;
}

// ctrl + Z to exist in Windows OS
// ctrk + D to exist in Linux OS