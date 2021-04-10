#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

#define PATTERN_SIZE 80
#define FULL_FILENAME_SIZE 100005

// create a new file
struct File *new_file()
{
    struct File *new = calloc(1, sizeof(struct File));
    if (!new)
    {
        fputs("Allocation failed!", stderr);
        exit(EXIT_FAILURE);
    }
    return new;
}

void set_filename(struct File *file, const char *name, int name_len)
{
    file->name = malloc(sizeof(char) * (name_len + 1));
    if (!file->name)
    {
        fputs("Allocation for filename failed!", stderr);
        exit(EXIT_FAILURE);
    }
    strncpy(file->name, name, name_len);
    file->name[name_len] = 0;
}

// count length of basename
int get_basename_len(const char *filename)
{
    int r = 0;
    while (*filename && *filename != '/')
        filename++, r++;
    return r;
}

// add a file to root
void add_file(struct File *root, const char *filename)
{
    int basename_len = get_basename_len(filename);
    int next = -1;
    for (int i = 0; i < root->size; i++)
    {
        if (strncmp(root->children[i]->name, filename, basename_len) == 0)
        {
            next = i;
            break;
        }
    }
    if (next == -1)
    {
        root->children = realloc(root->children, sizeof(struct File *) * (root->size + 1));
        if (!root->children)
        {
            fputs("Reallocation failed!", stderr);
            exit(EXIT_FAILURE);
        }
        struct File *temp_file = new_file();
        set_filename(temp_file, filename, basename_len);
        root->children[root->size] = temp_file;
        root->size++;
        next = root->size - 1;
    }
    if (!filename[basename_len])
        return;
    add_file(root->children[next], filename + basename_len + 1);
}

// recursively print file
void print_file(const struct File *root, int level)
{
    const char *INDENTATION = "  ";
    for (int i = 0; i < level; i++)
        printf("%s", INDENTATION);
    puts(root->name);
    for (int i = 0; i < root->size; i++)
        print_file(root->children[i], level + 1);
}

int main()
{
    // setup file root
    struct File *root = new_file();
    char pattern[PATTERN_SIZE];
    char filename[FULL_FILENAME_SIZE];
    fgets(pattern, PATTERN_SIZE, stdin);
    pattern[strlen(pattern) - 1] = 0;
    while (!feof(stdin))
    {
        fgets(filename, FULL_FILENAME_SIZE, stdin);
        filename[strlen(filename) - 1] = 0;
        add_file(root, filename);
    }
    // root->children[0] is "."
    cmd_find(pattern, root->children[0]);
    return 0;
}