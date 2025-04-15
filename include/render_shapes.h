#ifndef RENDER_SHAPES_H_
#define RENDER_SHAPES_H_

#include <math.h>
#include <stdint.h>
#include "Matrix2D.h"

void rs_draw_circle(Mat2D screen_mat, Mat2D center, float r, uint32_t color);
void rs_fill_circle(Mat2D screen_mat, Mat2D center, float r, uint32_t color);

#ifdef RENDER_SHAPES_IMPLEMENTATION
#undef RENDER_SHAPES_IMPLEMENTATION

void rs_draw_circle(Mat2D screen_mat, Mat2D center, float r, uint32_t color)
{
    MATRIX2D_ASSERT(2 == center.rows);
    MATRIX2D_ASSERT(1 == center.cols);

    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0 && diff > -r*2) {
                float x = MAT2D_AT(center, 0, 0) + dx;
                float y = MAT2D_AT(center, 1, 0) + dy;
                if (x >= screen_mat.cols || y >= screen_mat.rows || x < 0 || y < 0) {
                    continue;
                }
                MAT2D_AT(screen_mat, y, x) = color;
            }
        }
    }
}

void rs_fill_circle(Mat2D screen_mat, Mat2D center, float r, uint32_t color)
{
    MATRIX2D_ASSERT(2 == center.rows);
    MATRIX2D_ASSERT(1 == center.cols);

    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0) {
                float x = MAT2D_AT(center, 0, 0) + dx;
                float y = MAT2D_AT(center, 1, 0) + dy;
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