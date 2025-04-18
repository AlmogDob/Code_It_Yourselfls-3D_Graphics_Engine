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

float theta;
Scene scene;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    theta = 0;

    scene.camera.z_near       = 0.1;
    scene.camera.z_far        = 1000;
    scene.camera.fov_deg      = 90;
    scene.camera.aspect_ratio = (float)game_state->window_h / (float)game_state->window_w;

    scene.camera.position = mat2D_alloc(3, 1);
    mat2D_fill(scene.camera.position, 0);
    scene.camera.direction = mat2D_alloc(3, 1);
    mat2D_fill(scene.camera.direction, 0);
    MAT2D_AT(scene.camera.direction, 2, 0) = 1;

    scene.light_direction = mat2D_alloc(3, 1);
    mat2D_fill(scene.light_direction, 0);
    MAT2D_AT(scene.light_direction, 2, 0) = -1;

    scene.proj_mat = mat2D_alloc(4, 4);
    ae_set_projection_mat(scene.proj_mat, scene.camera.aspect_ratio, scene.camera.fov_deg, scene.camera.z_near, scene.camera.z_far);

    scene.cube = ae_create_cube(1);
}

Mesh temp_cube;
void update(game_state_t *game_state)
{
    theta += 50 * game_state->delta_time;

    temp_cube = ae_create_copy_of_mesh(scene.cube);

    ae_rotate_mesh_Euler_xyz(temp_cube, 0.5 * theta, theta * 0.3, theta);
    ae_translate_mesh(temp_cube, 0, 0, 2.5);

    scene.proj_cube = ae_project_mesh_world2screen(scene.proj_mat, temp_cube, game_state, &scene);

    free(temp_cube.elements);
}

void render(game_state_t *game_state)
{
    ars_draw_mesh(game_state->window_pixels_mat, scene.proj_cube, 0xFFFFFF);
    // AE_PRINT_MESH(proj_cube);

    free(scene.proj_cube.elements);
}

