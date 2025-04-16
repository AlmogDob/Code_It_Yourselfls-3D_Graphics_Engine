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

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;

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
    ars_draw_tri(game_state->window_pixels_mat, cube.elements[0], 0xFFFFFF);

}

