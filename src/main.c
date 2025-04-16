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
#define ALMOG_RENDER_SHAPES_IMPLEMENTATION
#include "./../include/Almog_Render_Shapes.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./../include/Almog_Engine.h"

#include <math.h>

#define PI M_PI

Mesh cube;
Mesh proj_cube;

Tri ae_project_tri_world2screen(Mat2D proj_mat, Tri tri)
{
    Tri des_tri;
    for (int i = 0; i < 3; i++) {
        des_tri.points[i] = ae_project_point_world2screen(proj_mat, tri.points[i]);
    }

    return des_tri;
}

Mesh ae_project_mesh_world2screen(Mat2D proj_mat, Mesh src)
{
    Mesh des;
    for (int i = 0; i < src.length; i++) {
        Tri temp_tri;
        temp_tri = ae_project_tri_world2screen(proj_mat, src.elements[i]);
        ada_appand(Tri, des, temp_tri);
    }

    return des;
}

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;

    ada_init_array(Tri, proj_cube);

    cube = ae_create_cube(100);

    Mat2D proj_mat = mat2D_alloc(4, 4);
    float z_near  = 0.1;
    float z_far   = 1000;
    float fov_deg = 90;
    float ar      = game_state->window_h / game_state->window_w;

    ae_set_projection_mat(proj_mat, ar, fov_deg, z_near, z_far);


}

void update(game_state_t *game_state)
{
    (void)game_state;
}

void render(game_state_t *game_state)
{
    ars_draw_mesh(game_state->window_pixels_mat, cube, 0xFFFFFF);
}

