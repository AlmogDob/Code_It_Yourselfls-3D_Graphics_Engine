#include <stdio.h>
#include <string.h>
#include "./include/Matrix2D.h"
#define ALMOG_RENDER_SHAPES_IMPLEMENTATION
#include "./include/Almog_Render_Shapes.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./include/Almog_Engine.h"

Point ae_line_itersect_plane(Mat2D plane_p, Mat2D plane_n, Mat2D line_start, Mat2D line_end)
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
    
    Mat2D intersection_p = mat2D_alloc(3, 1);
    mat2D_fill(intersection_p, 0);
    mat2D_add(intersection_p, line_start);
    mat2D_add(intersection_p, line_to_intersection);

    Point ans_p = ae_mat2D_to_point(intersection_p);

    mat2D_free(intersection_p);

    return ans_p;
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

    printf("(d0, d1, d2) -> (%f, %f, %f)\n", d0, d1, d2);
    printf("(in, out) -> (%d, %d)\n", inside_points_count, outside_points_count);
    /* classifying the triangle points */
    if (inside_points_count == 0) {
        return 0;
    } else if (inside_points_count == 3) {
        *tri_out1 = tri_in;
        return 1;
    } else if (inside_points_count == 1 && outside_points_count == 2) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        (*tri_out1).points[0] = inside_points[0];

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out1).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end);

        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[1], line_end);
        (*tri_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end);

        mat2D_free(line_start);
        mat2D_free(line_end);

        return 1;
    } else if (inside_points_count == 2 && outside_points_count == 1) {
        Mat2D line_start = mat2D_alloc(3, 1);
        Mat2D line_end   = mat2D_alloc(3, 1);
        
        (*tri_out1).points[0] = inside_points[0];
        (*tri_out1).points[1] = inside_points[1];
        ae_point_to_mat2D(inside_points[0], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out1).points[2] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end);

        (*tri_out2).points[0] = inside_points[1];
        ae_point_to_mat2D(inside_points[1], line_start);
        ae_point_to_mat2D(outside_points[0], line_end);
        (*tri_out2).points[1] = ae_line_itersect_plane(plane_p, plane_n, line_start, line_end);
        (*tri_out2).points[2] = (*tri_out1).points[2];

        mat2D_free(line_start);
        mat2D_free(line_end);

        return 2;
    }

}

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
