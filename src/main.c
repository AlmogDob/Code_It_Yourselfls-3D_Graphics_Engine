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
    // game_state->to_limit_fps = 0;
    game_state->const_fps = 30;
    theta = 0;  

    ada_init_array(Tri, temp_mesh);
    ada_init_array(Tri, game_state->scene.proj_temp_mesh);

    char file_path[MAX_LEN_LINE];
    // strncpy(file_path, "./obj_files/f16/f16.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/cruiser/cruiser.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/bunny.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/teapot.obj", MAX_LEN_LINE);
    strncpy(file_path, "./obj_files/axis.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/video_ship.obj", MAX_LEN_LINE);

    // strncpy(file_path, "./stl_files/plug.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/pin.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/teapot.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Stanford dragon highres.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Stanford dragon lowres.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Voronoi_Stanford_Bunny.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Lucy_120mm_simplified.STL", MAX_LEN_LINE);

    game_state->scene.mesh = ae_get_mesh_from_file(file_path);

    dprintSIZE_T(game_state->scene.mesh.length);
    // ae_rotate_mesh_Euler_xyz(game_state->scene.mesh, 0, 0, 180);
    // ae_rotate_mesh_Euler_xyz(game_state->scene.mesh, -90, 0, 0);
    // ae_rotate_mesh_Euler_xyz(game_state->scene.mesh, 0, 180, 0);
    ae_normalize_mesh(game_state->scene.mesh);

}

void update(game_state_t *game_state)
{
    ae_set_projection_mat(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_set_view_mat(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);

    // MAT2D_PRINT(game_state->scene.camera.position);

    theta += 50 * game_state->delta_time;

    ae_create_copy_of_mesh(&temp_mesh, game_state->scene.mesh.elements, game_state->scene.mesh.length);

    // ae_rotate_mesh_Euler_xyz(temp_mesh, 0, theta, theta/10);
    ae_translate_mesh(temp_mesh, 0, 0, 2);

    ae_project_mesh_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &(game_state->scene.proj_temp_mesh), temp_mesh, game_state->window_w, game_state->window_h, game_state->scene.light_direction, &(game_state->scene));

    temp_mesh.length = 0;
}

void render(game_state_t *game_state)
{
    ars_fill_mesh_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_temp_mesh, 0xFFFFFF);
    // ars_fill_mesh_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_temp_mesh, -1);

    // ars_draw_mesh(game_state->window_pixels_mat, game_state->scene.proj_temp_mesh, 0x0000FF);
    
    game_state->scene.proj_temp_mesh.length = 0;
}

