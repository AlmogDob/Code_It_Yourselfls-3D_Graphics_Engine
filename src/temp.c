#include <stdio.h>
#include <string.h>
#include "./include/Matrix2D.h"
#define ALMOG_RENDER_SHAPES_IMPLEMENTATION
#include "./include/Almog_Render_Shapes.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./include/Almog_Engine.h"


int main()
{
    Mat2D plane_p = mat2D_alloc(3, 1);
    Mat2D plane_n = mat2D_alloc(3, 1);

    mat2D_fill(plane_p, 0);
    mat2D_fill(plane_n, 0);
    MAT2D_AT(plane_n, 0, 0) = +1;

    Point p0 = {.x = -1, .y = 0 , .z = 0};
    Point p1 = {.x = 1, .y = 1 , .z = 0};
    Point p2 = {.x = 1, .y = -1, .z = 0};
    Tri tri_in  = {.points[0] = p0, .points[1] = p1, .points[2] = p2};
    Tri tri_out1 = {0};
    Tri tri_out2 = {0};

    ae_tri_clip_with_plane(tri_in, plane_p, plane_n, &tri_out1, &tri_out2);

    AE_PRINT_TRI(tri_in);
    AE_PRINT_TRI(tri_out1);
    AE_PRINT_TRI(tri_out2);

    return 0;
}
