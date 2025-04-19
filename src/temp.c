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
Tri tri;

void setup(game_state_t *game_state)
{
    game_state->to_limit_fps = 0;
    // game_state->const_fps = 60;
    theta = 0;

    game_state->scene.cube = ae_create_cube(1);
}

Mesh temp_cube;
void update(game_state_t *game_state)
{
    theta += 50 * game_state->delta_time;

    temp_cube = ae_create_copy_of_mesh(game_state->scene.cube);

    ae_rotate_mesh_Euler_xyz(temp_cube, 0.5 * theta, theta * 0.3, theta);
    ae_translate_mesh(temp_cube, 0, 0, 2.5);

    game_state->scene.proj_cube = ae_project_mesh_world2screen(game_state->scene.proj_mat, temp_cube, game_state->window_w, game_state->window_h, &(game_state->scene));

    free(temp_cube.elements);
}

void ars_fill_tri_Pinedas_rasterizer(Mat2D screen_mat, Tri tri, uint32_t color)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */

    Mat2D p0       = mat2D_alloc(3, 1);
    Mat2D p1       = mat2D_alloc(3, 1);
    Mat2D p2       = mat2D_alloc(3, 1);
    Mat2D v01      = mat2D_alloc(3, 1);
    Mat2D v12      = mat2D_alloc(3, 1);
    Mat2D v20      = mat2D_alloc(3, 1);
    Mat2D p        = mat2D_alloc(3, 1);
    Mat2D cross01p = mat2D_alloc(3, 1);
    Mat2D cross12p = mat2D_alloc(3, 1);
    Mat2D cross20p = mat2D_alloc(3, 1);

    ae_point_to_mat2D(tri.points[0], p0);
    ae_point_to_mat2D(tri.points[1], p1);
    ae_point_to_mat2D(tri.points[2], p2);

    ars_draw_arrow(screen_mat, tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, 0.2, 20, color);
    ars_draw_arrow(screen_mat, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, 0.2, 20, color);
    ars_draw_arrow(screen_mat, tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, 0.2, 20, color);

    mat2D_copy(v01, p1);
    mat2D_sub(v01, p0);
    mat2D_copy(v12, p2);
    mat2D_sub(v12, p1);
    mat2D_copy(v20, p0);
    mat2D_sub(v20, p2);

    /* finding bounding box */
    int x_min = fmin(MAT2D_AT(p0, 0, 0), fmin(MAT2D_AT(p1, 0, 0), MAT2D_AT(p2, 0, 0)));
    int x_max = fmax(MAT2D_AT(p0, 0, 0), fmax(MAT2D_AT(p1, 0, 0), MAT2D_AT(p2, 0, 0)));
    int y_min = fmin(MAT2D_AT(p0, 1, 0), fmin(MAT2D_AT(p1, 1, 0), MAT2D_AT(p2, 1, 0)));
    int y_max = fmax(MAT2D_AT(p0, 1, 0), fmax(MAT2D_AT(p1, 1, 0), MAT2D_AT(p2, 1, 0)));
    // printf("xmin: %d, xmax: %d || ymin: %d, ymax: %d\n", x_min, x_max, y_min, y_max);

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            // ars_draw_point(screen_mat, x, y, color);
            MAT2D_AT(p, 0, 0) = x;
            MAT2D_AT(p, 1, 0) = y;
            MAT2D_AT(p, 2, 0) = 0;
        }
    }

    mat2D_free(p0);   
    mat2D_free(p1);   
    mat2D_free(p2);   
    mat2D_free(v01);   
    mat2D_free(v12);   
    mat2D_free(v20);   
    mat2D_free(p);   
    mat2D_free(cross01p);   
    mat2D_free(cross12p);   
    mat2D_free(cross20p);   
}

void render(game_state_t *game_state)
{
    // ars_fill_mesh_scanline_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_cube, 0x00AA00);
    // ars_draw_mesh(game_state->window_pixels_mat, game_state->scene.proj_cube, 0x0000FF);

    // ars_fill_tri_scanline_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_cube.elements[0], 0xFFFFFF);
    // ars_fill_tri_scanline_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_cube.elements[1], 0xFFFFFF);

    ars_draw_tri(game_state->window_pixels_mat, game_state->scene.proj_cube.elements[0], 0x0000FF);
    ars_fill_tri_Pinedas_rasterizer(game_state->window_pixels_mat, game_state->scene.proj_cube.elements[0], 0xFFFFFF);
    
    free(game_state->scene.proj_cube.elements);
}

