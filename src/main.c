/* This code is heavily inspired by the 3D engine of 'OneLoneCoder' in C++.
you can find the source code in:
https://github.com/OneLoneCoder/Javidx9/tree/master/ConsoleGameEngine/BiggerProjects/Engine3D .
featured in this video of his:
https://youtu.be/ih20l3pJoeU?si=CzQ8rjk5ZEOlqEHN. */ 

#define SETUP
#define UPDATE
#define RENDER
#include "./../include/display.c"
#define MATRIX2D_IMPLEMENTATION
#include "./../include/Matrix2D.h"
#define RENDER_SHAPES_IMPLEMENTATION
#include "./../include/render_shapes.h"
#include "./../include/Almog_Dynamic_Array.h"

typedef struct {
    float x;
    float y;
    float z;
} Point ;

typedef struct {
    Point points[3];
} Tri;

typedef struct {
    size_t length;
    size_t capacity;
    Tri *elements;
} Mesh; /* Tri ada array */

Tri create_tri(Point p1, Point p2, Point p3)
{
    Tri tri;

    tri.points[0] = p1;
    tri.points[1] = p2;
    tri.points[2] = p3;

    return tri;
}

#define PRINT_TRI(tri) print_tri(tri, #tri, 0)
void print_tri(Tri tri, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding, "", tri.points[0].x, tri.points[0].y, tri.points[0].z, (int) padding, "", tri.points[1].x, tri.points[1].y, tri.points[1].z, (int) padding, "", tri.points[2].x, tri.points[2].y, tri.points[2].z);
}

#define PRINT_MESH(mesh) print_mesh(mesh, #mesh, 0)
void print_mesh(Mesh mesh, char *name, size_t padding)
{
    char tri_name[256];
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < mesh.length; i++) {
        snprintf(tri_name, 256, "tri %zu", i);
        print_tri(mesh.elements[i], tri_name, 4);
    }
}

Mesh create_cube(const size_t len)
{
    Mesh cube;

    ada_init_array(Tri, cube);

    Tri tri1 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri1);
    Tri tri2 = {
        .points[0].x = len,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = 0,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri2);
    Tri tri3 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
    };
    ada_appand(Tri, cube, tri3);
    Tri tri4 = {
        .points[0].x = len,
        .points[0].y = len,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = len,
    };
    ada_appand(Tri, cube, tri4);
    Tri tri5 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
    };
    ada_appand(Tri, cube, tri5);
    Tri tri6 = {
        .points[0].x = len,
        .points[0].y = len,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri6);
    Tri tri7 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = 0,
        .points[2].y = len,
        .points[2].z = len,
    };
    ada_appand(Tri, cube, tri7);
    Tri tri8 = {
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri8);
    Tri tri9 = {
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri9);
    Tri tri10 = {
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri10);
    Tri tri11 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri11);
    Tri tri12 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri12);
    
    return cube;
}

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;

    Mesh cube = create_cube(1);

}

void update(game_state_t *game_state)
{
}

void render(game_state_t *game_state)
{
    float r = 100;
    Mat2D center = mat2D_alloc(2, 1);
    srand(time(0));
    mat2D_rand(center, 0, fmin(game_state->window_h, game_state->window_w));

    rs_fill_circle(game_state->window_pixels_mat, MAT2D_AT(center, 0, 0), MAT2D_AT(center, 1, 0), r, 0xFFFFFFFF);

}

