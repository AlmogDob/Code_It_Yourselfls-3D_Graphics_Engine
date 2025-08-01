/* This code is heavily inspired by the 3D engine of 'OneLoneCoder' in C++.
you can find the source code in:
https://github.com/OneLoneCoder/Javidx9/tree/master/ConsoleGameEngine/BiggerProjects/Engine3D .
featured in this video of his:
https://youtu.be/ih20l3pJoeU?si=CzQ8rjk5ZEOlqEHN. */ 

#define SETUP
#define UPDATE
#define RENDER
#include "./include/display.c"
#include "./include/Matrix2D.h"
#define ALMOG_RENDER_SHAPES_IMPLEMENTATION
#include "./include/Almog_Render_Shapes.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./include/Almog_Engine.h"

float theta;
Mesh temp_mesh;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    // game_state->const_fps = 60;
    theta = 0;  

    ada_init_array(Tri, temp_mesh);
    ada_init_array(Tri, game_state->scene.proj_temp_mesh);

    char file_name[MAX_LEN_LINE];
    // strncpy(file_name, "./obj_files/f16/f16.obj", MAX_LEN_LINE);
    // strncpy(file_name, "./obj_files/cruiser/cruiser.obj", MAX_LEN_LINE);
    // strncpy(file_name, "./obj_files/bunny.obj", MAX_LEN_LINE);
    strncpy(file_name, "./obj_files/teapot.obj", MAX_LEN_LINE);


    game_state->scene.mesh = ae_get_mesh_from_obj_file(file_name);
    ae_normalize_mesh(game_state->scene.mesh);

    game_state->scene.temp_mesh = ae_create_cube(1);

}

void update(game_state_t *game_state)
{
    ae_set_projection_mat(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);

    // MAT2D_PRINT(game_state->scene.camera.position);

    theta += 50 * game_state->delta_time;

    ae_create_copy_of_mesh(&temp_mesh, game_state->scene.mesh.elements, game_state->scene.mesh.length);

    ae_rotate_mesh_Euler_xyz(temp_mesh, 0, theta, theta * 0.3);
    ae_translate_mesh(temp_mesh, 0, 0, 2);

    ae_project_mesh_world2screen(game_state->scene.proj_mat, &(game_state->scene.proj_temp_mesh), temp_mesh, game_state->window_w, game_state->window_h, game_state->scene.light_direction, &(game_state->scene));

    ae_qsort_tri(game_state->scene.proj_temp_mesh.elements, 0, game_state->scene.proj_temp_mesh.length-1);

    // AE_PRINT_MESH(game_state->scene.proj_temp_mesh);
    // exit(1);

    temp_mesh.length = 0;
    // free(temp_mesh.elements);
}

void render(game_state_t *game_state)
{
    ars_fill_mesh_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_temp_mesh, 0xFFFFFF);
    // ars_fill_mesh_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_temp_mesh, -1);

    // ars_draw_mesh(game_state->window_pixels_mat, game_state->scene.proj_temp_mesh, 0x0000FF);
    
    game_state->scene.proj_temp_mesh.length = 0;
    // free(game_state->scene.proj_temp_mesh.elements);
}

