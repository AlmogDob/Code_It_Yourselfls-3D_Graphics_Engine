#ifndef RENDER_SHAPES_H_
#define RENDER_SHAPES_H_

#include <math.h>
#include <stdint.h>
#include "Matrix2D.h"

void rs_draw_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color);
void rs_fill_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color);

#ifdef RENDER_SHAPES_IMPLEMENTATION
#undef RENDER_SHAPES_IMPLEMENTATION

void rs_draw_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0 && diff > -r*2) {
                float x = center_x + dx;
                float y = center_y + dy;
                if (x >= screen_mat.cols || y >= screen_mat.rows || x < 0 || y < 0) {
                    continue;
                }
                MAT2D_AT(screen_mat, y, x) = color;
            }
        }
    }
}

void rs_fill_circle(Mat2D screen_mat, float center_x, float center_y, float r, uint32_t color)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0) {
                float x = center_x + dx;
                float y = center_y + dy;
                if (x >= screen_mat.cols || y >= screen_mat.rows || x < 0 || y < 0) {
                    continue;
                }
                MAT2D_AT(screen_mat, y, x) = color;
            }
        }
    }
}

#endif /*RENDER_SHAPES_IMPLEMENTATION*/

#endif /*RENDER_SHAPES_H_*/