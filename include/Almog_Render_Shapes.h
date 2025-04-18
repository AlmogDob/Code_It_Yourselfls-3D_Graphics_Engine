#ifndef ALMOG_RENDER_SHAPES_H_
#define ALMOG_RENDER_SHAPES_H_

#include <math.h>
#include <stdint.h>
#include "Matrix2D.h"
#include "Almog_Engine.h"

void ars_draw_point(Mat2D screen_mat, int x, int y, uint32_t color);
void ars_draw_line(Mat2D screen_mat, int x1, int y1, int x2, int y2, uint32_t color);
void ars_draw_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color);
void ars_fill_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color);
void ars_draw_tri(Mat2D screen_mat, Tri tri, uint32_t color);
void ars_draw_mesh(Mat2D screen_mat, Mesh mesh, uint32_t color);

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

void ars_draw_mesh(Mat2D screen_mat, Mesh mesh, uint32_t color)
{
    for (size_t i = 0; i < mesh.length; i++) {
        ars_draw_tri(screen_mat, mesh.elements[i], color);
    }
}

#endif /*ALMOG_RENDER_SHAPES_IMPLEMENTATION*/
