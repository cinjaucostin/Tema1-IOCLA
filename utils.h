#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir
{
	char *name;
	struct Dir *parent;
	struct File *head_children_files;
	struct Dir *head_children_dirs;
	struct Dir *next;
} Dir;

typedef struct File
{
	char *name;
	struct Dir *parent;
	struct File *next;
} File;

File *create_new_file(Dir *parent, char *name);
void touch(Dir *parent, char *name);
Dir *create_new_dir(Dir *parent, char *name);
void mkdir(Dir *parent, char *name);
void ls(Dir *parent);
void rm(Dir *parent, char *name);
void elibereaza_fisiere(Dir *target);
void stop(Dir *target);
void rmdir(Dir *parent, char *name);
void cd(Dir **target, char *name);
char *pwd(Dir *target);
void print_n_spaces(int level, char *str);
void tree(Dir *target, int level);
int cauta_director_sau_fisier(Dir *parent, char *name);
void mv(Dir *parent, char *oldname, char *newname);

#endif