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
Mat2D proj_mat;
float theta;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    theta = 0;

    cube = ae_create_cube(1);
    // AE_PRINT_MESH(cube);

    proj_mat = mat2D_alloc(4, 4);

    float z_near  = 0.1;
    float z_far   = 1000;
    float fov_deg = 90;
    float ar      = (float)game_state->window_h / (float)game_state->window_w;

    ae_set_projection_mat(proj_mat, ar, fov_deg, z_near, z_far);


    // AE_PRINT_MESH(proj_cube);

}

Mesh temp_cube;
void update(game_state_t *game_state)
{
    theta += 50 * game_state->delta_time;

    temp_cube = ae_create_copy_of_mesh(cube);

    ae_rotate_mesh_Euler_xyz(temp_cube, 0.5 * theta, theta * 0.3, theta);
    ae_translate_mesh(temp_cube, 0, 0, 2.5);

    proj_cube = ae_project_mesh_world2screen(proj_mat, temp_cube, game_state);

    free(temp_cube.elements);
}

void render(game_state_t *game_state)
{
    ars_draw_mesh(game_state->window_pixels_mat, proj_cube, 0xFFFFFF);
    // AE_PRINT_MESH(proj_cube);

    free(proj_cube.elements);
}

