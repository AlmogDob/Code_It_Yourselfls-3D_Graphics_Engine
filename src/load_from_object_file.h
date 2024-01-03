#ifndef LOAD_FROM_OBJECT_FILE
#define LOAD_FROM_OBJECT_FILE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINE 200
#define MAXWORD 100

#ifndef MESH_TYPE

#include "Vec3.h"

#define MAX_NUM_OF_TRIANGLES 10000
#define MAX_NUM_OF_VERTS 3*MAX_NUM_OF_TRIANGLES


typedef struct {
    Vec3 p[3];
} triangle;

typedef struct {
    triangle tris[MAX_NUM_OF_TRIANGLES];
} mesh;
#endif //MESH_TYPE

int my_getline(char *line, int max, FILE *file);
int load_from_object_file(mesh *this_mesh, char *file_name);


#endif //LOAD_FROM_OBJECT_FILE