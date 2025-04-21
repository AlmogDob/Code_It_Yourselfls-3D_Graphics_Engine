#ifndef ALMOG_ENGINE_H_
#define ALMOG_ENGINE_H_

#include "./../include/Almog_Dynamic_Array.h"
#include "./../include/Matrix2D.h"

#ifndef AE_ASSERT
#include <assert.h>
#define AE_ASSERT assert
#endif

#include <math.h>
#include <stdbool.h>

#ifndef PI
#define PI M_PI
#endif

typedef struct {
    float x;
    float y;
    float z;
} Point ;

typedef struct {
    Point points[3];
    bool to_draw;
} Tri;

typedef struct {
    size_t length;
    size_t capacity;
    Tri *elements;
} Mesh; /* Tri ada array */

typedef struct {
    size_t length;
    Tri *elements;
} Mesh_static;

typedef struct {
    Mat2D position;
    Mat2D direction;
    float z_near;
    float z_far;
    float fov_deg;
    float aspect_ratio;
} Camera;

typedef struct {
    Mesh cube;
    Mesh proj_cube;
    Mesh_static mesh;
    Camera camera;
    Mat2D light_direction;
    Mat2D proj_mat;
} Scene;

Tri ae_create_tri(Point p1, Point p2, Point p3);
Mesh ae_create_copy_of_mesh(Tri *mesh, size_t len);
Mesh ae_create_cube(const size_t len);
void ae_point_to_mat2D(Point p, Mat2D m);

void ae_print_tri(Tri tri, char *name, size_t padding);
void ae_print_mesh(Mesh mesh, char *name, size_t padding);
void ae_print_mesh_static(Mesh_static mesh, char *name, size_t padding);

void ae_calc_normal_to_tri(Mat2D normal, Tri tri);
void ae_translate_mesh(Mesh mesh, float x, float y, float z);
void ae_rotate_mesh_Euler_xyz(Mesh mesh, float phi, float theta, float psi);

void ae_set_projection_mat(Mat2D proj_mat,float aspect_ratio, float FOV_deg, float z_near, float z_far);
Point ae_project_point_world2screen(Mat2D proj_mat, Point src);
Tri ae_project_tri_world2screen(Mat2D proj_mat, Tri tri, int window_w, int window_h, Scene *scene);
Mesh ae_project_mesh_world2screen(Mat2D proj_mat, Mesh src, int window_w, int window_h, Scene *scene);

#endif /* ALMOG_ENGINE_H_ */

#ifdef ALMOG_ENGINE_IMPLEMENTATION
#undef ALMOG_ENGINE_IMPLEMENTATION

#define AE_PRINT_TRI(tri) ae_print_tri(tri, #tri, 0)
#define AE_PRINT_MESH(mesh) ae_print_mesh(mesh, #mesh, 0)
#define AE_PRINT_MESH_STATIC(mesh) ae_print_mesh_static(mesh, #mesh, 0)

Tri ae_create_tri(Point p1, Point p2, Point p3)
{
    Tri tri;

    tri.points[0] = p1;
    tri.points[1] = p2;
    tri.points[2] = p3;

    return tri;
}

Mesh ae_create_copy_of_mesh(Tri *mesh, size_t len)
{
    Mesh des;
    ada_init_array(Tri, des);
    for (size_t i = 0; i < len; i++) {
        ada_appand(Tri, des, mesh[i]);
    }

    return des;
}

Mesh ae_create_cube(const size_t len)
{
    Mesh cube;

    ada_init_array(Tri, cube);

    Tri tri1 = { /* south */
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
        .to_draw = true
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
        .to_draw = true
    };
    ada_appand(Tri, cube, tri2);
    Tri tri3 = { /* north */
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = len,
        .points[2].z = len,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri3);
    Tri tri4 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = len,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri4);
    Tri tri5 = { /* east */
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri5);
    Tri tri6 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = len,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri6);
    Tri tri7 = { /* west */
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = len,
        .points[2].z = 0,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri7);
    Tri tri8 = {
        .points[0].x = 0,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = 0,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri8);
    Tri tri9 = { /* top */
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = 0,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = len,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri9);
    Tri tri10 = {
        .points[0].x = 0,
        .points[0].y = len,
        .points[0].z = 0,

        .points[1].x = len,
        .points[1].y = len,
        .points[1].z = len,

        .points[2].x = len,
        .points[2].y = len,
        .points[2].z = 0,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri10);
    Tri tri11 = { /* bottom */
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = len,

        .points[2].x = 0,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri11);
    Tri tri12 = {
        .points[0].x = len,
        .points[0].y = 0,
        .points[0].z = len,

        .points[1].x = 0,
        .points[1].y = 0,
        .points[1].z = 0,

        .points[2].x = len,
        .points[2].y = 0,
        .points[2].z = 0,
        .to_draw = true
    };
    ada_appand(Tri, cube, tri12);
    
    return cube;
}

void ae_point_to_mat2D(Point p, Mat2D m)
{
    MATRIX2D_ASSERT((3 == m.rows && 1 == m.cols) || (1 == m.rows && 3 == m.cols));
    
    if (3 == m.rows) {
        MAT2D_AT(m, 0, 0) = p.x;
        MAT2D_AT(m, 1, 0) = p.y;
        MAT2D_AT(m, 2, 0) = p.z;
    }
    if (3 == m.cols) {
        MAT2D_AT(m, 0, 0) = p.x;
        MAT2D_AT(m, 0, 1) = p.y;
        MAT2D_AT(m, 0, 2) = p.z;
    }

}

void ae_print_tri(Tri tri, char *name, size_t padding)
{
    printf("%*s%s:\n", (int) padding, "", name);
    printf("%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n%*s    (%f, %f, %f)\n", (int) padding, "", tri.points[0].x, tri.points[0].y, tri.points[0].z, (int) padding, "", tri.points[1].x, tri.points[1].y, tri.points[1].z, (int) padding, "", tri.points[2].x, tri.points[2].y, tri.points[2].z);
    printf("%*s    draw? %d\n", (int)padding, "", tri.to_draw);
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

void ae_print_mesh_static(Mesh_static mesh, char *name, size_t padding)
{
    char tri_name[256];
    printf("%*s%s:\n", (int) padding, "", name);
    for (size_t i = 0; i < mesh.length; i++) {
        snprintf(tri_name, 256, "tri %zu", i);
        ae_print_tri(mesh.elements[i], tri_name, 4);
    }
}

void ae_calc_normal_to_tri(Mat2D normal, Tri tri)
{
    MATRIX2D_ASSERT(3 == normal.rows && 1 == normal.cols);

    Mat2D a = mat2D_alloc(3, 1);
    Mat2D b = mat2D_alloc(3, 1);
    Mat2D c = mat2D_alloc(3, 1);

    ae_point_to_mat2D(tri.points[0], a);
    ae_point_to_mat2D(tri.points[1], b);
    ae_point_to_mat2D(tri.points[2], c);

    mat2D_sub(b, a);
    mat2D_sub(c, a);

    mat2D_cross(normal, b, c);

    mat2D_mult(normal, 1/mat2D_calc_norma(normal));

    mat2D_free(a);
    mat2D_free(b);
    mat2D_free(c);
}

void ae_translate_mesh(Mesh mesh, float x, float y, float z)
{
    for (size_t i = 0; i < mesh.length; i++) {
        for (int j = 0; j < 3; j++) {
            mesh.elements[i].points[j].x += x;
            mesh.elements[i].points[j].y += y;
            mesh.elements[i].points[j].z += z;
        }
    }
}

/* phi around x, theta around y, psi around z.
DCM = Cz*Cy*Cx */
void ae_rotate_mesh_Euler_xyz(Mesh mesh, float phi, float theta, float psi)
{
    Mat2D RotZ = mat2D_alloc(3,3);
    mat2D_set_rot_mat_z(RotZ, psi);
    Mat2D RotY = mat2D_alloc(3,3);
    mat2D_set_rot_mat_y(RotY, theta);
    Mat2D RotX = mat2D_alloc(3,3);
    mat2D_set_rot_mat_x(RotX, phi);
    Mat2D DCM = mat2D_alloc(3,3);
    // mat2D_fill(DCM,0);
    Mat2D temp = mat2D_alloc(3,3);
    // mat2D_fill(temp,0);
    mat2D_dot(temp, RotY, RotZ);
    mat2D_dot(DCM, RotX, temp); /* I have a DCM */

    Mat2D src_point_mat = mat2D_alloc(3,1);
    Mat2D des_point_mat = mat2D_alloc(3,1);

    for (size_t i = 0; i < mesh.length; i++) {
        for (int j = 0; j < 3; j++) {
            // mat2D_fill(src_point_mat, 0);
            // mat2D_fill(des_point_mat, 0);
            Point des;
            Point src = mesh.elements[i].points[j];

            MAT2D_AT(src_point_mat, 0, 0) = src.x;
            MAT2D_AT(src_point_mat, 1, 0) = src.y;
            MAT2D_AT(src_point_mat, 2, 0) = src.z;

            mat2D_dot(des_point_mat, DCM, src_point_mat);

            des.x = MAT2D_AT(des_point_mat, 0, 0);
            des.y = MAT2D_AT(des_point_mat, 1, 0);
            des.z = MAT2D_AT(des_point_mat, 2, 0);

            mesh.elements[i].points[j] = des;
        }
    }

    mat2D_free(RotZ);
    mat2D_free(RotY);
    mat2D_free(RotX);
    mat2D_free(DCM);
    mat2D_free(src_point_mat);
    mat2D_free(des_point_mat);
}

void ae_set_projection_mat(Mat2D proj_mat,float aspect_ratio, float FOV_deg, float z_near, float z_far)
{
    AE_ASSERT(4 == proj_mat.cols); 
    AE_ASSERT(4 == proj_mat.rows); 

    mat2D_fill(proj_mat, 0);

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
    // mat2D_fill(src_point_mat, 0);
    Mat2D des_point_mat = mat2D_alloc(1,4);
    // mat2D_fill(des_point_mat, 0);
    Point des;

    MAT2D_AT(src_point_mat, 0, 0) = src.x;
    MAT2D_AT(src_point_mat, 0, 1) = src.y;
    MAT2D_AT(src_point_mat, 0, 2) = src.z;
    MAT2D_AT(src_point_mat, 0, 3) = 1;

    mat2D_dot(des_point_mat, src_point_mat, proj_mat);

    if (MAT2D_AT(des_point_mat, 0, 3)) {
        des.x = MAT2D_AT(des_point_mat, 0, 0) / MAT2D_AT(des_point_mat, 0, 3);
        des.y = MAT2D_AT(des_point_mat, 0, 1) / MAT2D_AT(des_point_mat, 0, 3);
        des.z = MAT2D_AT(des_point_mat, 0, 2) / MAT2D_AT(des_point_mat, 0, 3);
    } else {
        des.x = MAT2D_AT(des_point_mat, 0, 0);
        des.y = MAT2D_AT(des_point_mat, 0, 1);
        des.z = MAT2D_AT(des_point_mat, 0, 2);
    }

    mat2D_free(src_point_mat);
    mat2D_free(des_point_mat);

    return des;
}

Tri ae_project_tri_world2screen(Mat2D proj_mat, Tri tri, int window_w, int window_h, Scene *scene)
{
    Mat2D tri_normal = mat2D_alloc(3, 1);
    Mat2D temp_camera2tri = mat2D_alloc(3, 1);
    Mat2D camera2tri = mat2D_alloc(1, 3);
    Mat2D dot_product = mat2D_alloc(1, 1);
    // mat2D_fill(dot_product, 0);
    Tri des_tri;

    ae_calc_normal_to_tri(tri_normal, tri);
    ae_point_to_mat2D(tri.points[0], temp_camera2tri);
    mat2D_sub(temp_camera2tri, scene->camera.position);
    mat2D_transpose(camera2tri, temp_camera2tri);

    mat2D_dot(dot_product, camera2tri, tri_normal);

    if (MAT2D_AT(dot_product, 0, 0) < 0) {
        des_tri.to_draw = true;
    } else {
        des_tri.to_draw = false;
    }

    for (int i = 0; i < 3; i++) {
        des_tri.points[i] = ae_project_point_world2screen(proj_mat, tri.points[i]);

        /* scale into view */
        des_tri.points[i].x += 1;
        des_tri.points[i].y += 1;

        des_tri.points[i].x *= 0.5f * window_w;
        des_tri.points[i].y *= 0.5f * window_h;
    }

    mat2D_free(tri_normal);

    return des_tri;
}

Mesh ae_project_mesh_world2screen(Mat2D proj_mat, Mesh src, int window_w, int window_h, Scene *scene)
{
    Mesh des;

    ada_init_array(Tri, des);

    for (size_t i = 0; i < src.length; i++) {
        Tri temp_tri;
        temp_tri = ae_project_tri_world2screen(proj_mat, src.elements[i], window_w, window_h, scene);
        // AE_PRINT_TRI(temp_tri);
        
        ada_appand(Tri, des, temp_tri);
    }

    return des;
}



#endif /* ALMOG_ENGINE_IMPLEMENTATION */