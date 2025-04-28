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


float theta;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    // game_state->const_fps = 60;
    theta = 0;  

    {
    #include "./../build/f16.c"
    game_state->scene.mesh = ae_create_copy_of_mesh(f16.elements, f16.length);
    }
    {
    // #include "./../build/bunny.c"
    // game_state->scene.mesh = ae_create_copy_of_mesh(bunny.elements, bunny.length);
    }

    game_state->scene.cube = ae_create_cube(1);

}

Mesh temp_cube;
void update(game_state_t *game_state)
{
    theta += 50 * game_state->delta_time;

    temp_cube = ae_create_copy_of_mesh(game_state->scene.mesh.elements, game_state->scene.mesh.length);

    // temp_cube = ae_create_copy_of_mesh(game_state->scene.cube.elements, game_state->scene.cube.length);

    ae_rotate_mesh_Euler_xyz(temp_cube, 0, theta, theta * 0.3);
    // ae_translate_mesh(temp_cube, 0, 0, 2);
    ae_translate_mesh(temp_cube, 0, 0, 5);
    // ae_translate_mesh(temp_cube, 0, 0, 16);
    // ae_translate_mesh(temp_cube, 0, 0, 30);
    // ae_translate_mesh(temp_cube, 0, 0, 170);

    game_state->scene.proj_cube = ae_project_mesh_world2screen(game_state->scene.proj_mat, temp_cube, game_state->window_w, game_state->window_h, game_state->scene.light_direction, &(game_state->scene));

    ae_qsort_tri(game_state->scene.proj_cube.elements, 0, game_state->scene.proj_cube.length-1);

    // AE_PRINT_MESH(game_state->scene.proj_cube);
    // exit(1);

    free(temp_cube.elements);
}

void render(game_state_t *game_state)
{
    ars_fill_mesh_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_cube, 0xFFFFFF);
    // ars_fill_mesh_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_cube, -1);

    // ars_draw_mesh(game_state->window_pixels_mat, game_state->scene.proj_cube, 0x0000FF);
    
    free(game_state->scene.proj_cube.elements);
}

