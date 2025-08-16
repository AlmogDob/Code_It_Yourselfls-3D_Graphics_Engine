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

void ae_line_itersect_plane(Mat2D intersection_p, Mat2D plane_p, Mat2D plane_n, Mat2D line_start, Mat2D line_end)
{
    mat2D_normalize(plane_n);
    float plane_d = - mat2D_dot_product(plane_n, plane_p);
    float ad = mat2D_dot_product(line_start, plane_n);
    float bd = mat2D_dot_product(line_end, plane_n);
    float t  = (- plane_d - ad) / (bd - ad);
    mat2D_sub(line_end, line_start);
    Mat2D line_start_to_end = line_end;
    mat2D_mult(line_start_to_end, t);
    Mat2D line_to_intersection = line_start_to_end;
    
    mat2D_fill(intersection_p, 0);
    mat2D_add(intersection_p, line_start);
    mat2D_add(intersection_p, line_to_intersection);
}

/* signed distance from point to plane */
float signed_dist_point_and_plane(Point p, Mat2D plane_p, Mat2D plane_n)
{
    mat2D_normalize(plane_n);
    Mat2D p_mat2D = mat2D_alloc(3, 1);
    ae_point_to_mat2D(p, p_mat2D);

    float res = mat2D_dot_product(plane_n, p_mat2D) - mat2D_dot_product(plane_n, plane_p); 

    mat2D_free(p_mat2D);

    return res;
}

/* returns number of inside triangles */
int ae_tri_clip_with_plane(Tri tri_in, Mat2D plane_p, Mat2D plane_n, Tri *tri_out1, Tri *tri_out2)
{
    mat2D_normalize(plane_n);

    /* if the signed distance is positive, the point lies on the "inside" of the plane */
    Point inside_points[3];
    Point outside_points[3];
    int inside_points_count = 0;
    int outside_points_count = 0;
    
    /* calc signed distance of each point of tri_in */
    float d0 = signed_dist_point_and_plane(tri_in.points[0], plane_p, plane_n);
    float d1 = signed_dist_point_and_plane(tri_in.points[1], plane_p, plane_n);
    float d2 = signed_dist_point_and_plane(tri_in.points[2], plane_p, plane_n);

    if (d0 >= 0) {
        inside_points[inside_points_count++] = tri_in.points[0];
    } else {
        outside_points[outside_points_count++] = tri_in.points[0];
    }
    if (d1 >= 0) {
        inside_points[inside_points_count++] = tri_in.points[1];
    } else {
        outside_points[outside_points_count++] = tri_in.points[1];
    }
    if (d2 >= 0) {
        inside_points[inside_points_count++] = tri_in.points[2];
    } else {
        outside_points[outside_points_count++] = tri_in.points[2];
    }

    /* classifying the triangle points */
    if (inside_points_count == 0) {
        return 0;
    } else if (inside_points_count == 3) {
        *tri_out1 = tri_in;
        return 1;
    } else if (inside_points_count == 1 && outside_points_count == 2) {
        (*tri_out1).points[0] = inside_points[0];
        (*tri_out1).points[0] = inside_points[0];
        (*tri_out1).points[0] = inside_points[0];
    }
}

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    // game_state->const_fps = 30;
    theta = 0;  

    ada_init_array(Tri, temp_mesh);
    ada_init_array(Tri, game_state->scene.proj_temp_mesh);

    char file_path[MAX_LEN_LINE];
    // strncpy(file_path, "./obj_files/f16/f16.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/cruiser/cruiser.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/bunny.obj", MAX_LEN_LINE);
    strncpy(file_path, "./obj_files/axis.obj", MAX_LEN_LINE);
    // strncpy(file_path, "./obj_files/video_ship.obj", MAX_LEN_LINE);

    // strncpy(file_path, "./stl_files/plug.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/pin.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/teapot.stl", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Stanford dragon highres.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Stanford dragon lowres.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Voronoi_Stanford_Bunny.STL", MAX_LEN_LINE);
    // strncpy(file_path, "./stl_files/Lucy_120mm_simplified.STL", MAX_LEN_LINE);

    game_state->scene.mesh = ae_get_mesh_from_file(file_path);

    dprintSIZE_T(game_state->scene.mesh.length);
    // ae_rotate_mesh_Euler_xyz(game_state->scene.mesh, 0, 0, 180);
    // ae_rotate_mesh_Euler_xyz(game_state->scene.mesh, -90, 0, 180);
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
    ars_fill_mesh_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_temp_mesh, 0xCCCCCC);
    // ars_fill_mesh_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_temp_mesh, -1);

    ars_draw_mesh(game_state->window_pixels_mat, game_state->scene.proj_temp_mesh, 0xAA0000);
    
    game_state->scene.proj_temp_mesh.length = 0;
}

