#ifndef LOAD_FROM_OBJECT_FILE
#define LOAD_FROM_OBJECT_FILE

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINE 200
#define MAXWORD 100

int my_getline(char *line, int max, FILE *file);
bool load_from_object_file(char *file_name);


#endif