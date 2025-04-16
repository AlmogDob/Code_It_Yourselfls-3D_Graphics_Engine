#ifndef ALMOG_ENGINE_H_
#define ALMOG_ENGINE_H_

#include "./../include/Almog_Dynamic_Array.h"
#include "./../include/Matrix2D.h"

#ifndef AE_ASSERT
#include <assert.h>
#define AE_ASSERT assert
#endif

typedef struct {
    float x;
    float y;
    float z;
} Point ;

typedef struct {
    Point points[3];
} Tri;

typedef struct {
    size_t length;
    size_t capacity;
    Tri *elements;
} Mesh; /* Tri ada array */

#define AE_PRINT_TRI(tri) ae_print_tri(tri, #tri, 0)
#define AE_PRINT_MESH(mesh) ae_print_mesh(mesh, #mesh, 0)

Tri ae_create_tri(Point p1, Point p2, Point p3);
void ae_print_tri(Tri tri, char *name, size_t padding);
void ae_print_mesh(Mesh mesh, char *name, size_t padding);
Mesh ae_create_cube(const size_t len);
void ae_set_projection_mat(Mat2D proj_mat,float aspect_ratio, float FOV_deg, float z_near, float z_far);
Point ae_project_point_world2screen(Mat2D proj_mat, Point src);

#endif /* ALMOG_ENGINE_H_ */

#ifdef ALMOG_ENGINE_IMPLEMENTATION
#undef ALMOG_ENGINE_IMPLEMENTATION

Tri ae_create_tri(Point p1, Point p2, Point p3)
{
    Tri tri;

    tri.points[0] = p1;
    tri.points[1] = p2;
    tri.points[2] = p3;

    return tri;
}

void ae_print_tri(Tri tri, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding, "", tri.points[0].x, tri.points[0].y, tri.points[0].z, (int) padding, "", tri.points[1].x, tri.points[1].y, tri.points[1].z, (int) padding, "", tri.points[2].x, tri.points[2].y, tri.points[2].z);
}

void ae_print_mesh(Mesh mesh, char *name, size_t padding)
{
    char tri_name[256];
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < mesh.length; i++) {
        snprintf(tri_name, 256, "tri %zu", i);
        ae_print_tri(mesh.elements[i], tri_name, 4);
    }
}

Mesh ae_create_cube(const size_t len)
{
    Mesh cube;

    ada_init_array(Tri, cube);

    Tri tri1 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri1);
    Tri tri2 = {
        .points[0].x = len,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = 0,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri2);
    Tri tri3 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
    };
    ada_appand(Tri, cube, tri3);
    Tri tri4 = {
        .points[0].x = len,
        .points[0].y = len,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = len,
    };
    ada_appand(Tri, cube, tri4);
    Tri tri5 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
    };
    ada_appand(Tri, cube, tri5);
    Tri tri6 = {
        .points[0].x = len,
        .points[0].y = len,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri6);
    Tri tri7 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = 0,
        .points[2].y = len,
        .points[2].z = len,
    };
    ada_appand(Tri, cube, tri7);
    Tri tri8 = {
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri8);
    Tri tri9 = {
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri9);
    Tri tri10 = {
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri10);
    Tri tri11 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri11);
    Tri tri12 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = 0,
    };
    ada_appand(Tri, cube, tri12);
    
    return cube;
}

void ae_set_projection_mat(Mat2D proj_mat,float aspect_ratio, float FOV_deg, float z_near, float z_far)
{
    AE_ASSERT(4 == proj_mat.cols); 
    AE_ASSERT(4 == proj_mat.rows); 

    float field_of_view = 1.0f / tanf(0.5f * FOV_deg * PI / 180);
    float z_normalization = z_far / (z_far - z_near);

    MAT2D_AT(proj_mat, 0, 0) = aspect_ratio * field_of_view;
    MAT2D_AT(proj_mat, 1, 1) = field_of_view;
    MAT2D_AT(proj_mat, 2, 2) = z_normalization;
    MAT2D_AT(proj_mat, 2, 3) = 1;
    MAT2D_AT(proj_mat, 3, 2) = - z_normalization * z_near;
}

Point ae_project_point_world2screen(Mat2D proj_mat, Point src)
{
    Mat2D src_point_mat = mat2D_alloc(1,4);
    Mat2D des_point_mat = mat2D_alloc(1,4);
    Point des;

    MAT2D_AT(src_point_mat, 0, 0) = src.x;
    MAT2D_AT(src_point_mat, 0, 1) = src.y;
    MAT2D_AT(src_point_mat, 0, 2) = src.z;
    MAT2D_AT(src_point_mat, 0, 3) = 1;

    mat2D_dot(des_point_mat, src_point_mat, proj_mat);

    if (!MAT2D_AT(src_point_mat, 0, 3)) {
        des.x = MAT2D_AT(src_point_mat, 0, 0) / MAT2D_AT(src_point_mat, 0, 3);
        des.y = MAT2D_AT(src_point_mat, 0, 1) / MAT2D_AT(src_point_mat, 0, 3);
        des.z = MAT2D_AT(src_point_mat, 0, 2) / MAT2D_AT(src_point_mat, 0, 3);
    }

    mat2D_free(src_point_mat);
    mat2D_free(des_point_mat);

    return des;
}


#endif /* ALMOG_ENGINE_IMPLEMENTATION */