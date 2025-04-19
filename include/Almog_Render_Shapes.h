#ifndef ALMOG_RENDER_SHAPES_H_
#define ALMOG_RENDER_SHAPES_H_

#include <math.h>
#include <stdint.h>
#include "Matrix2D.h"
#include "Almog_Engine.h"

void ars_draw_point(Mat2D screen_mat, int x, int y, uint32_t color);
void ars_draw_line(Mat2D screen_mat, int x1, int y1, int x2, int y2, uint32_t color);
void ars_draw_arrow(Mat2D screen_mat, int xs, int ys, int xe, int ye, float head_size, float angle_deg, uint32_t color);

void ars_draw_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color);
void ars_fill_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color);

void ars_draw_tri(Mat2D screen_mat, Tri tri, uint32_t color);
void ars_fill_tri_scanline_rasterizer(Mat2D screen_mat, Tri tri, uint32_t color);

void ars_draw_mesh(Mat2D screen_mat, Mesh mesh, uint32_t color);
void ars_fill_mesh_scanline_rasterizer(Mat2D screen_mat, Mesh mesh, uint32_t color);

#endif /*ALMOG_RENDER_SHAPES_H_*/

#ifdef ALMOG_RENDER_SHAPES_IMPLEMENTATION
#undef ALMOG_RENDER_SHAPES_IMPLEMENTATION

void ars_draw_point(Mat2D screen_mat, int x, int y, uint32_t color)
{
    if ((x < (int)screen_mat.cols && y < (int)screen_mat.rows) && (x >= 0 && y >= 0)) { /* point is in screen */
        MAT2D_AT(screen_mat, y, x) = color;
    }
}

void ars_draw_line(Mat2D screen_mat, int x1, int y1, int x2, int y2, uint32_t color)
{
    /* This function is inspired by the Olive.c function developed by 'Tsoding' on his YouTube channel. You can fined the video in this link: https://youtu.be/LmQKZmQh1ZQ?list=PLpM-Dvs8t0Va-Gb0Dp4d9t8yvNFHaKH6N&t=4683. */

    int x = x1;
    int y = y1;
    int dx, dy;

    ars_draw_point(screen_mat, x, y, color);

    dx = x2 - x1;
    dy = y2 - y1;

    if (0 == dx && 0 == dy) return;
    if (0 == dx) {
        while (x != x2 || y != y2) {
            if (dy > 0) {
                y++;
            }
            if (dy < 0) {
                y--;
            }
            ars_draw_point(screen_mat, x, y, color);
        }
        return;
    }
    if (0 == dy) {
        while (x != x2 || y != y2) {
            if (dx > 0) {
                x++;
            }
            if (dx < 0) {
                x--;
            }
            ars_draw_point(screen_mat, x, y, color);
        }
        return;
    }

    /* float m = (float)dy / dx */
    int b = y1 - dy * x1 / dx;

    if (x1 > x2) {
        int temp_x = x1;
        x1 = x2;
        x2 = temp_x;
    }
    for (x = x1; x < x2; x++) {
        int sy1 = dy * x / dx + b;
        int sy2 = dy * (x + 1) / dx + b;
        if (sy1 > sy2) {
            int temp_y = sy1;
            sy1 = sy2;
            sy2 = temp_y;
        }
        for (y = sy1; y <= sy2; y++) {
            ars_draw_point(screen_mat, x, y, color);
        }
    }

}

void ars_draw_arrow(Mat2D screen_mat, int xs, int ys, int xe, int ye, float head_size, float angle_deg, uint32_t color)
{
    /* This function is a bit complicated but this is what I could come up with */
    Mat2D pe = mat2D_alloc(3, 1);
    mat2D_fill(pe, 0);
    MAT2D_AT(pe, 0, 0) = xe;
    MAT2D_AT(pe, 1, 0) = ye;
    Mat2D v1 = mat2D_alloc(3, 1);
    mat2D_fill(v1, 0);
    Mat2D v2 = mat2D_alloc(3, 1);
    mat2D_fill(v2, 0);
    Mat2D temp_v = mat2D_alloc(3, 1);
    mat2D_fill(temp_v, 0);
    Mat2D DCM_p = mat2D_alloc(3, 3);
    mat2D_fill(DCM_p, 0);
    mat2D_set_rot_mat_z(DCM_p, angle_deg);
    Mat2D DCM_m = mat2D_alloc(3, 3);
    mat2D_fill(DCM_m, 0);
    mat2D_set_rot_mat_z(DCM_m, -angle_deg);

    int x_center = xs*head_size + xe*(1-head_size);
    int y_center = ys*head_size + ye*(1-head_size);

    MAT2D_AT(v1, 0, 0) = x_center;
    MAT2D_AT(v1, 1, 0) = y_center;
    mat2D_copy(v2, v1);

    /* v1 */
    mat2D_copy(temp_v, v1);
    mat2D_sub(temp_v, pe);
    mat2D_fill(v1, 0);
    mat2D_dot(v1, DCM_p, temp_v);
    mat2D_add(v1, pe);

    /* v2 */
    mat2D_copy(temp_v, v2);
    mat2D_sub(temp_v, pe);
    mat2D_fill(v2, 0);
    mat2D_dot(v2, DCM_m, temp_v);
    mat2D_add(v2, pe);

    ars_draw_line(screen_mat, MAT2D_AT(v1, 0, 0), MAT2D_AT(v1, 1, 0), xe, ye, color);
    ars_draw_line(screen_mat, MAT2D_AT(v2, 0, 0), MAT2D_AT(v2, 1, 0), xe, ye, color);
    ars_draw_line(screen_mat, xs, ys, xe, ye, color);

    mat2D_free(pe);
    mat2D_free(v1);
    mat2D_free(v2);
    mat2D_free(temp_v);
    mat2D_free(DCM_p);
    mat2D_free(DCM_m);
}

void ars_draw_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0 && diff > -r*2) {
                ars_draw_point(screen_mat, center_x + dx, center_y + dy, color);
            }
        }
    }
}

void ars_fill_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0) {
                ars_draw_point(screen_mat, center_x + dx, center_y + dy, color);
            }
        }
    }
}

void ars_draw_tri(Mat2D screen_mat, Tri tri, uint32_t color)
{
    ars_draw_line(screen_mat, tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, color);
    ars_draw_line(screen_mat, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, color);
    ars_draw_line(screen_mat, tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, color);
}

    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */

/* This works but there are some artifacts */
void ars_fill_tri_scanline_rasterizer(Mat2D screen_mat, Tri tri, uint32_t color)
{
    /* arranging the points according to y value */
    Point p0 = tri.points[0];
    Point p1 = tri.points[1];
    Point p2 = tri.points[2];
    if (p1.y > p0.y) {
        Point temp = p1;
        p1 = p0;
        p0 = temp;
    }
    if (p2.y > p1.y) {
        Point temp = p2;
        p2 = p1;
        p1 = temp;
        if (p1.y > p0.y) {
            Point temp = p1;
            p1 = p0;
            p0 = temp;
        }
    }
    if (p2.y > p0.y) {
        Point temp = p2;
        p2 = p0;
        p0 = temp;
    }

    /* finding max and min x */
    int x_max = fmax(p0.x, fmax(p1.x, p2.x));
    int x_min = fmin(p0.x, fmin(p1.x, p2.x));

    if (p0.x == p1.x && p1.x == p2.x) {
        ars_draw_tri(screen_mat, tri, color);
        return;
    }

    /* The rasterization */
    float m01 = (p0.y - p1.y) / (p0.x - p1.x);
    float b01 = p0.y - m01 * p0.x;
    float m02 = (p0.y - p2.y) / (p0.x - p2.x);
    float b02 = p0.y - m02 * p0.x;
    float m12 = (p1.y - p2.y) / (p1.x - p2.x);
    float b12 = p1.y - m12 * p1.x;

    float epsilon = 1e-3;
    int gap = 15;
    // printf("m01: %f, m02: %f, m12: %f\n", m01, m02, m12);
    if (fabs(m02) < epsilon || fabs(m12) < epsilon || fabs(m01) < epsilon) return;
    for (int y = (int)p2.y; y < (int)p1.y; y++)
    {
        float x02 = (y - b02) / m02;
        float x12 = (y - b12) / m12;
        if (x02 <= x_min-gap || x02 >= x_max+gap) continue;
        if (x12 <= x_min-gap || x12 >= x_max+gap) continue;
        if (fabs(p0.x - p2.x) - fabs(p0.x - x02) < 0) continue;
        if (fabs(p1.x - p2.x) - fabs(p1.x - x12) < 0) continue;
        ars_draw_line(screen_mat, x02, y, x12, y, color);
        // printf("x02: %d, x12: %d, y: %d\n", (int)x02, (int)x12, (int)y);
    }
    for (int y = (int)p1.y; y <= (int)p0.y; y++) {
        float x01 = (y - b01) / m01;
        float x02 = (y - b02) / m02;
        if (x01 <= x_min-gap || x01 >= x_max+gap) continue;
        if (x02 <= x_min-gap || x02 >= x_max+gap) continue;
        if (fabs(p1.x - p0.x) - fabs(p1.x - x01) < 0) continue;
        if (fabs(p0.x - p2.x) - fabs(p0.x - x02) < 0) continue;
        ars_draw_line(screen_mat, x02, y, x01, y, color);
    }
}

void ars_draw_mesh(Mat2D screen_mat, Mesh mesh, uint32_t color)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        if (tri.to_draw) {
            ars_draw_tri(screen_mat, tri, color);
        }
    }
}

void ars_fill_mesh_scanline_rasterizer(Mat2D screen_mat, Mesh mesh, uint32_t color)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        if (tri.to_draw) {
            ars_fill_tri_scanline_rasterizer(screen_mat, tri, color);
        }
    }
}

#endif /*ALMOG_RENDER_SHAPES_IMPLEMENTATION*/
