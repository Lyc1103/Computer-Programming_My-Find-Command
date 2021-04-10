# My "Find" Command

> Author : Ya Chen<br>
> Date : 2021 / 4 / 10<br>
> List :
>
> > <a href = "#description">Description</a><br><a href = "#input">Input</a><br><a href = "#output">Output</a><br><a href = "#examples">Examples</a>

---

<div id = "description">

## Description

`find` is a useful command to find files you want.<br>
In this problem, you need to implement its `-iname` test, which accepts a pattern to match the filename (case insensitive).<br>
For example, given a directory like this.

```
.
├── cat
├── rabbit
└── fox
    └── cat
```

If the command is `find . -iname cat`, it will match `./cat` and `./fox/cat` (dont’t forget the `./` prefix).<br>
Patterns may contain these wildcards

- `?` matches any single character

- `*`matches any sequence (any number of any characters, just everything).

- `[...]` matches any single character between `[` and `]`, e.g., `[abc]` will match a, b or c.

Its difinition is a little different from HRG in last homework, be careful!

Given a struct File in below definition.

```c
struct File
{
  char *name;
  int size;
  struct File **children;
}
```

- `name` is the file’s base name. It will be `.` if this file is root.
- `size` means how many files under this directory. If this is a file, `size` will be 0.
- `children` is an array of directory’s children’s pointers with length of `size`.

You should implement a function `void cmd_find(char *pattern, struct File *root)`, which prints all matched files’ full name.

Also, this is the `main.c` content, if you need. I write some function that can help you parse input like the `tree --fromfile .` behavior.

```c
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
```

</div>
<br>
<br>
<div id = "input">

## Input

Sample inputs follow this format. This is a little different from the input format for judging, but I think it is more readable.

```
pattern
empty line
root
```

The first line is `pattern` we want to match, and `root` is the directory structure in `tree`'s output format.

</div>
<br>
<br>
<div id = "output">

## Output

Print every matched files’ full name. Each one occupies one line, in given order.

</div>
<br>
<br>
<div id = "examples">

## Examples

### Input 1

```
apple
./apple
./banana
./grape
```

### Output 1

```
./apple
```

<br>

### Input 2

```
apple
./apple
./AppLe
./APplE
```

### Output 2

```
./apple
./AppLe
./APplE
```

<br>

### Input 3

```
NT?U
./NTU
./NTNU
./NTUST
```

### Output 3

```
./NTNU
```

<br>

### Input 4

```
*.py
./m1/foo.c
./m1/bar.py
./m1/foo.py
./m2/bar.py
```

### Output 4

```
./m1.py
./m1/bar.py
./m1/foo.py
./m2/bar.py
```

</div>
