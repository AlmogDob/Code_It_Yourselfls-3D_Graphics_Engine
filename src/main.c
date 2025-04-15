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

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;

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

    rs_fill_circle(game_state->window_pixels_mat, center, r, 0xFFFFFFFF);

}

