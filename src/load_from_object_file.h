#ifndef LOAD_FROM_OBJECT_FILE
#define LOAD_FROM_OBJECT_FILE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINE 200
#define MAXWORD 100

#ifndef MESH_TYPE
#define MESH_TYPE

#ifndef VEC3_IMPLEMENTATION
// #define VEC3_IMPLEMENTATION
#endif //VEC3_IMPLEMENTATION
#include "Vec3.h"
#include <SDL2/SDL.h>

#define MAX_NUM_OF_TRIANGLES 15000
#define MAX_NUM_OF_VERTS 3*MAX_NUM_OF_TRIANGLES
#define dprintS(expr) printf(#expr " = %s\n", expr)
#define dprintI(expr) printf(#expr " = %d\n", expr)
#define dprintC(expr) printf(#expr " = %c\n", expr)
#define dprintF(expr) printf(#expr " = %g\n", expr)


typedef struct {
    Vec3 p[3];
    float mid_z;
    SDL_Color my_color;
} triangle;

typedef struct {
    triangle tris[MAX_NUM_OF_TRIANGLES];
} mesh;
#endif //MESH_TYPE

int my_getline(char *line, int max, FILE *file);
int load_from_object_file(mesh *this_mesh, char *file_name);


#endif //LOAD_FROM_OBJECT_FILE