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
#include "./include/Almog_Render_Shapes2D.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./include/Almog_Engine.h"

void ae_appand_copy_of_mesh(Mesh_array *mesh_array, Mesh mesh) {
    Mesh_array temp_mesh_array = *mesh_array;
    Mesh temp_mesh;
    ada_init_array(Tri, temp_mesh);
    for (size_t i = 0; i < mesh.length; i++) {
        ada_appand(Tri, temp_mesh, mesh.elements[i]);
    }
    ada_appand(Mesh, temp_mesh_array, temp_mesh);

    *mesh_array = temp_mesh_array;
}

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    // game_state->const_fps = 5;

    ada_init_array(Mesh, game_state->scene.original_meshes);
    ada_init_array(Mesh, game_state->scene.in_world_meshes);
    ada_init_array(Mesh, game_state->scene.projected_meshes);

    char file_path[MAX_LEN_LINE];
    // strncpy(file_path, "./obj_files/f16/f16.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/cruiser/cruiser.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/bunny.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/axis.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/video_ship.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/indoor_plant_02_obj/indoor_plant_02.obj", MAX_LEN_LINE);

    // strncpy(file_path, "./stl_files/plug.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/pin.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/teapot.stl", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Stanford dragon highres.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Stanford dragon lowres.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Voronoi_Stanford_Bunny.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Lucy_120mm_simplified.STL", MAX_LEN_LINE);

    // strncpy(file_path, "./stl_files/teapot.stl", MAX_LEN_LINE);
    strncpy(file_path, "./obj_files/f16/f16.obj", MAX_LEN_LINE);
    ada_appand(Mesh, game_state->scene.original_meshes, ae_get_mesh_from_file(file_path));

    // ada_appand(Mesh, game_state->scene.original_meshes, ae_create_cube(1, 0xFFFFFFFF));


    printf("[INFO] number of meshes: %zu\n", game_state->scene.original_meshes.length);
    size_t sum = 0;
    for (size_t i = 0; i < game_state->scene.original_meshes.length; i++) {
        printf("[INFO] mesh number %zu: %zu\n", i, game_state->scene.original_meshes.elements[i].length);
        sum += game_state->scene.original_meshes.elements[i].length;
    }
    printf("[INFO] total number of triangles: %zu\n", sum);


    for (size_t i = 0; i < game_state->scene.original_meshes.length; i++) {
        ae_normalize_mesh(game_state->scene.original_meshes.elements[i]);
    }
    for (size_t i = 0; i < game_state->scene.original_meshes.length; i++) {
        ae_appand_copy_of_mesh(&(game_state->scene.in_world_meshes), game_state->scene.original_meshes.elements[i]);
        ae_appand_copy_of_mesh(&(game_state->scene.projected_meshes), game_state->scene.original_meshes.elements[i]);
        game_state->scene.projected_meshes.elements[i].length = 0;
    }

    // ae_rotate_mesh_Euler_xyz(game_state->scene.in_world_meshes.elements[0], -90, 0, 180);

    // ae_translate_mesh(game_state->scene.in_world_meshes.elements[0], 0, 0, 2);
}

void update(game_state_t *game_state)
{
    ae_set_projection_mat(game_state->scene.proj_mat, game_state->scene.camera.aspect_ratio, game_state->scene.camera.fov_deg, game_state->scene.camera.z_near, game_state->scene.camera.z_far);
    ae_set_view_mat(game_state->scene.view_mat, game_state->scene.camera, game_state->scene.up_direction);

    float theta = 10 * game_state->delta_time;
    // float theta = 0;

    ae_rotate_mesh_Euler_xyz(game_state->scene.in_world_meshes.elements[0], theta, 0, theta/2);

    for (size_t i = 0; i < game_state->scene.in_world_meshes.length; i++) {
        ae_project_mesh_world2screen(game_state->scene.proj_mat, game_state->scene.view_mat, &(game_state->scene.projected_meshes.elements[i]), game_state->scene.in_world_meshes.elements[i], game_state->window_w, game_state->window_h, game_state->scene.light_direction, &(game_state->scene));
    }

}

void render(game_state_t *game_state)
{
    for (size_t i = 0; i < game_state->scene.projected_meshes.length; i++) {
        ars2D_fill_mesh_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->inv_z_buffer_mat, game_state->scene.projected_meshes.elements[i]);
        // ars2D_fill_mesh_scanline_rasterizer(game_state->window_pixels_mat, game_state->scene.projected_meshes.elements[i]);
        // ars2D_draw_mesh(game_state->window_pixels_mat, game_state->scene.projected_meshes.elements[i], 0x00000000);
    }

    for (size_t i = 0; i < game_state->scene.in_world_meshes.length; i++) {
        game_state->scene.projected_meshes.elements[i].length = 0;
    }

    // ae_copy_z_buffer_to_screen(game_state->window_pixels_mat, game_state->inv_z_buffer_mat);
}
