#ifndef ALMOG_ENGINE_H_
#define ALMOG_ENGINE_H_

#include "./../include/Almog_Dynamic_Array.h"

#ifndef MATRIX2D_IMPLEMENTATION
#define MATRIX2D_IMPLEMENTATION
#endif
#include "./../include/Matrix2D.h"

#ifndef ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#endif
#include "./../include/Almog_String_Manipulation.h"

#ifndef AE_ASSERT
#include <assert.h>
#define AE_ASSERT assert
#endif

#include <math.h>
#include <stdbool.h>
#include <float.h>
#include <errno.h>
#include <string.h>

#ifndef PI
#define PI M_PI
#endif

#ifndef STL_HEADER_SIZE
#define STL_HEADER_SIZE 80
#endif

#ifndef STL_NUM_SIZE
#define STL_NUM_SIZE 4
#endif

#ifndef STL_SIZE_FOREACH_TRI
#define STL_SIZE_FOREACH_TRI 50
#endif

#ifndef STL_ATTRIBUTE_BITS_SIZE
#define STL_ATTRIBUTE_BITS_SIZE 2
#endif

#ifndef POINT
#define POINT
typedef struct {
    float x;
    float y;
    float z;
} Point ;
#endif

#ifndef POINTS
#define POINTS
typedef struct {
    size_t length;
    size_t capacity;
    Point* elements;
} Points;
#endif

#ifndef TRI
#define TRI
typedef struct {
    Point points[3];
    Point center;
    Point normal;
    float z_min;
    float z_max;
    bool to_draw;
    float light_intensity;
} Tri;
#endif

#ifndef MESH
#define MESH
typedef struct {
    size_t length;
    size_t capacity;
    Tri *elements;
} Mesh; /* Tri ada array */
#endif

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
    Mesh temp_mesh;
    Mesh proj_temp_mesh;
    Mesh mesh;
    Camera camera;
    Mat2D light_direction;
    Mat2D proj_mat;
} Scene;

Tri ae_create_tri(Point p1, Point p2, Point p3);
void ae_create_copy_of_mesh(Mesh *des, Tri *src_elements, size_t len);
Mesh ae_create_cube(const size_t len);
void ae_point_to_mat2D(Point p, Mat2D m);
Mesh ae_get_mesh_from_obj_file(char *file_path);
Mesh ae_get_mesh_from_stl_file(char *file_path);

void ae_print_points(Points p);
void ae_print_tri(Tri tri, char *name, size_t padding);
void ae_print_mesh(Mesh mesh, char *name, size_t padding);
void ae_print_mesh_static(Mesh_static mesh, char *name, size_t padding);

void ae_calc_normal_to_tri(Mat2D normal, Tri tri);
void ae_translate_mesh(Mesh mesh, float x, float y, float z);
void ae_rotate_mesh_Euler_xyz(Mesh mesh, float phi_deg, float theta_deg, float psi_deg);
void ae_set_mesh_bounding_box(Mesh mesh, float *x_min, float *x_max, float *y_min, float *y_max, float *z_min, float *z_max);
void ae_set_tri_center_zmin_zmax(Tri *tri);
void ae_normalize_mesh(Mesh mesh);

void ae_set_projection_mat(Mat2D proj_mat,float aspect_ratio, float FOV_deg, float z_near, float z_far);
Point ae_project_point_world2screen(Mat2D proj_mat, Point src);
Tri ae_project_tri_world2screen(Mat2D proj_mat, Tri tri, int window_w, int window_h, Mat2D light_direction, Scene *scene);
void ae_project_mesh_world2screen(Mat2D proj_mat, Mesh *des, Mesh src, int window_w, int window_h, Mat2D light_direction, Scene *scene);

void ae_swap_tri(Tri *v, int i, int j);
bool ae_compare_tri(Tri t1, Tri t2);
void ae_qsort_tri(Tri *v, int left, int right);

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

void ae_create_copy_of_mesh(Mesh *des, Tri *src_elements, size_t len)
{
    Mesh temp_des = *des;
    for (size_t i = 0; i < len; i++) {
        ada_appand(Tri, temp_des, src_elements[i]);
    }
    *des = temp_des;
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

Mesh ae_get_mesh_from_obj_file(char *file_path)
{
    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE], current_num_str[MAX_LEN_LINE];
    char file_name[MAX_LEN_LINE], file_extention[MAX_LEN_LINE], mesh_name[MAX_LEN_LINE];
    int texture_warning_was_printed = 0;

    strncpy(file_name, file_path, MAX_LEN_LINE);
    strncpy(file_extention, file_name, MAX_LEN_LINE);

    /* check if file is an obj file*/
    asm_get_word_and_cut(file_name, file_extention, '.');
    asm_get_word_and_cut(file_name, file_extention, '.');
    if (strncmp(file_extention, ".obj", MAX_LEN_LINE)) {
        fprintf(stderr, "%s:%d: [Error] unsupported file format: '%s'\n", __FILE__, __LINE__, file_name);
        exit(1);
    }

    strncpy(mesh_name, file_name, MAX_LEN_LINE);
    while(asm_length(mesh_name)) {
        asm_get_word_and_cut(current_word, mesh_name, '/');
    }

    strncpy(mesh_name, current_word, MAX_LEN_LINE);

    strncpy(current_word, ".", MAX_LEN_LINE);
    strncat(file_name, ".obj", MAX_LEN_LINE/2);
    strncat(current_word, file_name, MAX_LEN_LINE/2);

    FILE *fp_input = fopen(current_word, "rt");
    if (fp_input == NULL) {
        fprintf(stderr, "%s:%d: [Error] failed to open input file: '%s', %s\n", __FILE__, __LINE__, current_word, strerror(errno));
        exit(1);
    }

    // strncpy(output_file_name, "./build/", MAX_LEN_LINE);
    // strncat(output_file_name, mesh_name, MAX_LEN_LINE/2);
    // strncat(output_file_name, ".c", MAX_LEN_LINE/2);
    // FILE *fp_output = fopen(output_file_name, "wt");
    // if (fp_input == NULL) {
    //     fprintf(stderr, "%s:%d: [Error] failed to open output file: '%s'. %s\n", __FILE__, __LINE__, output_file_name, strerror(errno));
    //     exit(1);
    // }

    /* parsing data from file */
    Points points;
    ada_init_array(Point, points);
    Mesh mesh;
    ada_init_array(Tri, mesh);

    int line_len;

    while ((line_len = asm_get_line(fp_input, current_line)) != -1) {
        asm_get_next_word_from_line(current_word, current_line, ' ');
        if (!strncmp(current_word, "v", 1)) {
            Point p;
            asm_get_word_and_cut(current_word, current_line, ' ');
            asm_get_word_and_cut(current_word, current_line, ' ');
            p.x = atof(current_word);
            asm_get_word_and_cut(current_word, current_line, ' ');
            p.y = atof(current_word);
            asm_get_word_and_cut(current_word, current_line, ' ');
            p.z = atof(current_word);
            // printf("current word: %s\n", current_word);
            ada_appand(Point, points, p);
            // break;
        }
        if (!strncmp(current_word, "f", 1)) {
            Tri tri1, tri2;

            // printf("line: %s\nword: %s, %d\n", current_line, current_word, atoi(current_word));
            asm_get_word_and_cut(current_word, current_line, ' ');
            // printf("line: %s\nword: %s, %d\n", current_line, current_word, atoi(current_word));

            int number_of_spaces = asm_str_in_str(current_line, " ");
            // printf("%d\n", number_of_spaces);
            // exit(1);
            if (!(number_of_spaces == 3 || number_of_spaces == 4 || number_of_spaces == 5)) {
                fprintf(stderr, "%s:%d: [Error] there is unsupported number of vertices for a face: %d\n", __FILE__, __LINE__, number_of_spaces);
                exit(1);
            }
            if (number_of_spaces == 3) {
                /* there are 3 vertices for the face. */
                asm_get_word_and_cut(current_word, current_line, ' ');
                // printf("line: %s\nword: %s, %d\n", current_line, current_word, atoi(current_word));
                int number_of_backslash = asm_str_in_str(current_word, "/");
                if (number_of_backslash == 0) {
                    tri1.points[0] = points.elements[atoi(current_word)-1];
                    asm_get_word_and_cut(current_word, current_line, ' ');
                    tri1.points[1] = points.elements[atoi(current_word)-1];
                    asm_get_word_and_cut(current_word, current_line, ' ');
                    tri1.points[2] = points.elements[atoi(current_word)-1];
                }
                if (number_of_backslash == 2) {
                    if (!texture_warning_was_printed) {
                        fprintf(stderr, "%s:%d [Warning] texture and normals data ignored\n", __FILE__, __LINE__);
                        texture_warning_was_printed = 1;
                    }

                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[0] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[1] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[2] = points.elements[atoi(current_num_str)-1];
                }
                tri1.to_draw = true;
                tri1.light_intensity = 1;
                tri1.center.x = (tri1.points[0].x + tri1.points[1].x + tri1.points[2].x) / 3;
                tri1.center.y = (tri1.points[0].y + tri1.points[1].y + tri1.points[2].y) / 3;
                tri1.center.z = (tri1.points[0].z + tri1.points[1].z + tri1.points[2].z) / 3;
                tri1.z_min = fmin(tri1.points[0].z, fmin(tri1.points[1].z, tri1.points[2].z));
                tri1.z_max = fmax(tri1.points[0].z, fmax(tri1.points[1].z, tri1.points[2].z));

                ada_appand(Tri, mesh, tri1);
                // AE_PRINT_TRI(tri1);
            } else if (number_of_spaces == 5 || number_of_spaces == 4) {
                /* there are 4 vertices for the face. */
                /* sometimes there is a space in the end */
                asm_get_word_and_cut(current_word, current_line, ' ');
                // printf("line: %s\nword: %s, %d\n", current_line, current_word, atoi(current_word));
                int number_of_backslash = asm_str_in_str(current_word, "/");
                if (number_of_backslash == 0) {
                    tri1.points[0] = points.elements[atoi(current_word)-1];
                    asm_get_word_and_cut(current_word, current_line, ' ');
                    tri1.points[1] = points.elements[atoi(current_word)-1];
                    asm_get_word_and_cut(current_word, current_line, ' ');
                    tri1.points[2] = points.elements[atoi(current_word)-1];
                }
                if (number_of_backslash == 2 || number_of_backslash == 1) {
                    if (!texture_warning_was_printed) {
                        fprintf(stderr, "%s:%d [Warning] texture and normals data ignored\n", __FILE__, __LINE__);
                        texture_warning_was_printed = 1;
                    }

                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[0] = points.elements[atoi(current_num_str)-1];
                    tri2.points[2] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[1] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri1.points[2] = points.elements[atoi(current_num_str)-1];
                    tri2.points[0] = points.elements[atoi(current_num_str)-1];

                    asm_get_word_and_cut(current_word, current_line, ' ');
                    asm_get_word_and_cut(current_num_str, current_word, '/');
                    // printf("line: %s\nword: %s\nnum str: %s, %d\n", current_line, current_word, current_num_str, atoi(current_num_str));
                    tri2.points[1] = points.elements[atoi(current_num_str)-1];
                }
                tri1.to_draw = true;
                tri1.light_intensity = 1;
                tri1.center.x = (tri1.points[0].x + tri1.points[1].x + tri1.points[2].x) / 3;
                tri1.center.y = (tri1.points[0].y + tri1.points[1].y + tri1.points[2].y) / 3;
                tri1.center.z = (tri1.points[0].z + tri1.points[1].z + tri1.points[2].z) / 3;
                tri1.z_min = fmin(tri1.points[0].z, fmin(tri1.points[1].z, tri1.points[2].z));
                tri1.z_max = fmax(tri1.points[0].z, fmax(tri1.points[1].z, tri1.points[2].z));

                tri2.to_draw = true;
                tri2.light_intensity = 1;
                tri2.center.x = (tri2.points[0].x + tri2.points[1].x + tri2.points[2].x) / 3;
                tri2.center.y = (tri2.points[0].y + tri2.points[1].y + tri2.points[2].y) / 3;
                tri2.center.z = (tri2.points[0].z + tri2.points[1].z + tri2.points[2].z) / 3;
                tri2.z_min = fmin(tri2.points[0].z, fmin(tri2.points[1].z, tri2.points[2].z));
                tri2.z_max = fmax(tri2.points[0].z, fmax(tri2.points[1].z, tri2.points[2].z));

                ada_appand(Tri, mesh, tri1);
                ada_appand(Tri, mesh, tri2);
                // AE_PRINT_TRI(tri1);
                // AE_PRINT_TRI(tri2);
            }
            // exit(2);
        }
    }
    
    return mesh;
}

Mesh ae_get_mesh_from_stl_file(char *file_path)
{
    FILE *file;
    file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "%s:%d: [Error] failed to open input file: '%s', %s\n", __FILE__, __LINE__, file_path, strerror(errno));
        exit(1);
    }

    char header[STL_HEADER_SIZE];
    fread(header, STL_HEADER_SIZE, 1, file);
    // dprintSTRING(header);

    uint32_t num_of_tri;
    fread(&num_of_tri, STL_NUM_SIZE, 1, file);
    // dprintINT(num_of_tri);

    Mesh mesh;
    ada_init_array(Tri, mesh);
    for (size_t i = 0; i < num_of_tri; i++) {
        Tri temp_tri;

        fread(&(temp_tri.normal.x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.normal.y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.normal.z), STL_NUM_SIZE, 1, file);

        fread(&(temp_tri.points[0].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[0].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[0].z), STL_NUM_SIZE, 1, file);

        fread(&(temp_tri.points[1].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[1].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[1].z), STL_NUM_SIZE, 1, file);
        
        fread(&(temp_tri.points[2].x), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[2].y), STL_NUM_SIZE, 1, file);
        fread(&(temp_tri.points[2].z), STL_NUM_SIZE, 1, file);

        fseek(file, STL_ATTRIBUTE_BITS_SIZE, SEEK_CUR);

        temp_tri.to_draw = true;
        temp_tri.light_intensity = 1;
        temp_tri.center.x = (temp_tri.points[0].x + temp_tri.points[1].x + temp_tri.points[2].x) / 3;
        temp_tri.center.y = (temp_tri.points[0].y + temp_tri.points[1].y + temp_tri.points[2].y) / 3;
        temp_tri.center.z = (temp_tri.points[0].z + temp_tri.points[1].z + temp_tri.points[2].z) / 3;
        temp_tri.z_min = fmin(temp_tri.points[0].z, fmin(temp_tri.points[1].z, temp_tri.points[2].z));
        temp_tri.z_max = fmax(temp_tri.points[0].z, fmax(temp_tri.points[1].z, temp_tri.points[2].z));

        ada_appand(Tri, mesh, temp_tri);
    }

    return mesh;
}

void ae_print_points(Points p)
{
    for (size_t i = 0; i < p.length; i++) {
        printf("point %3zu: (%5f, %5f, %5f)\n", i, p.elements[i].x, p.elements[i].y, p.elements[i].z);
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
void ae_rotate_mesh_Euler_xyz(Mesh mesh, float phi_deg, float theta_deg, float psi_deg)
{
    Mat2D RotZ = mat2D_alloc(3,3);
    mat2D_set_rot_mat_z(RotZ, psi_deg);
    Mat2D RotY = mat2D_alloc(3,3);
    mat2D_set_rot_mat_y(RotY, theta_deg);
    Mat2D RotX = mat2D_alloc(3,3);
    mat2D_set_rot_mat_x(RotX, phi_deg);
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

void ae_set_mesh_bounding_box(Mesh mesh, float *x_min, float *x_max, float *y_min, float *y_max, float *z_min, float *z_max)
{
    float xmin = FLT_MAX, xmax = FLT_MIN;
    float ymin = FLT_MAX, ymax = FLT_MIN;
    float zmin = FLT_MAX, zmax = FLT_MIN;

    float x, y, z;

    for (size_t t = 0; t < mesh.length; t++) {
        for (size_t p = 0; p < 3; p++) {
            x = mesh.elements[t].points[p].x;
            y = mesh.elements[t].points[p].y;
            z = mesh.elements[t].points[p].z;
            if (x > xmax) xmax = x;
            if (x < xmin) xmin = x;
            if (y > ymax) ymax = y;
            if (y < ymin) ymin = y;
            if (z > zmax) zmax = z;
            if (z < zmin) zmin = z;
        }
    }
    *x_min = xmin;
    *x_max = xmax; 
    *y_min = ymin;
    *y_max = ymax;
    *z_min = zmin;
    *z_max = zmax;
}

void ae_set_tri_center_zmin_zmax(Tri *tri)
{
    tri->center.x = (tri->points[0].x + tri->points[1].x + tri->points[2].x) / 3;
    tri->center.y = (tri->points[0].y + tri->points[1].y + tri->points[2].y) / 3;
    tri->center.z = (tri->points[0].z + tri->points[1].z + tri->points[2].z) / 3;
    tri->z_min = fmin(tri->points[0].z, fmin(tri->points[1].z, tri->points[2].z));
    tri->z_max = fmax(tri->points[0].z, fmax(tri->points[1].z, tri->points[2].z));
}

/* normalize all the points in between -1 and 1. the origin is in the center of the body. */
void ae_normalize_mesh(Mesh mesh)
{
    float xmax, xmin, ymax, ymin, zmax, zmin;
    ae_set_mesh_bounding_box(mesh, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);

    for (size_t t = 0; t < mesh.length; t++) {
        for (size_t p = 0; p < 3; p++) {
            float x, y, z;
            x = mesh.elements[t].points[p].x;
            y = mesh.elements[t].points[p].y;
            z = mesh.elements[t].points[p].z;

            float xdiff = xmax-xmin;
            float ydiff = ymax-ymin;
            float zdiff = zmax-zmin;
            float max_diff = fmax(xdiff, fmax(ydiff, zdiff));
            float xfactor = xdiff/max_diff;
            float yfactor = ydiff/max_diff;
            float zfactor = zdiff/max_diff;

            x = (((x - xmin) / (xdiff)) * 2 - 1) * xfactor;
            y = (((y - ymin) / (ydiff)) * 2 - 1) * yfactor;
            z = (((z - zmin) / (zdiff)) * 2 - 1) * zfactor;

            ae_set_tri_center_zmin_zmax(&(mesh.elements[t]));

            mesh.elements[t].points[p].x = x;
            mesh.elements[t].points[p].y = y;
            mesh.elements[t].points[p].z = z;
        }
    }
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

Tri ae_project_tri_world2screen(Mat2D proj_mat, Tri tri, int window_w, int window_h, Mat2D light_direction, Scene *scene)
{
    Mat2D tri_normal = mat2D_alloc(3, 1);
    Mat2D temp_camera2tri = mat2D_alloc(3, 1);
    Mat2D camera2tri = mat2D_alloc(1, 3);
    Mat2D light_directio_traspose = mat2D_alloc(1, 3);
    Mat2D dot_product = mat2D_alloc(1, 1);
    // mat2D_fill(dot_product, 0);
    Tri des_tri;

    ae_calc_normal_to_tri(tri_normal, tri);
    ae_point_to_mat2D(tri.points[0], temp_camera2tri);
    mat2D_sub(temp_camera2tri, scene->camera.position);
    mat2D_transpose(camera2tri, temp_camera2tri);
    mat2D_transpose(light_directio_traspose, light_direction);

    MAT2D_AT(dot_product, 0, 0) = MAT2D_AT(light_directio_traspose, 0, 0) * MAT2D_AT(tri_normal, 0, 0) + MAT2D_AT(light_directio_traspose, 0, 1) * MAT2D_AT(tri_normal, 1, 0) + MAT2D_AT(light_directio_traspose, 0, 2) * MAT2D_AT(tri_normal, 2, 0);
    // mat2D_dot(dot_product, light_directio_traspose, tri_normal);

    des_tri.light_intensity = MAT2D_AT(dot_product, 0, 0);
    if (des_tri.light_intensity < 0) {
        des_tri.light_intensity = 0;
    }

    MAT2D_AT(dot_product, 0, 0) = MAT2D_AT(camera2tri, 0, 0) * MAT2D_AT(tri_normal, 0, 0) + MAT2D_AT(camera2tri, 0, 1) * MAT2D_AT(tri_normal, 1, 0) + MAT2D_AT(camera2tri, 0, 2) * MAT2D_AT(tri_normal, 2, 0);
    // mat2D_dot(dot_product, camera2tri, tri_normal);

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
    mat2D_free(temp_camera2tri);
    mat2D_free(camera2tri);
    mat2D_free(light_directio_traspose);
    mat2D_free(dot_product);

    ae_set_tri_center_zmin_zmax(&des_tri);

    return des_tri;
}

void ae_project_mesh_world2screen(Mat2D proj_mat, Mesh *des, Mesh src, int window_w, int window_h, Mat2D light_direction, Scene *scene)
{
    Mesh temp_des = *des;
    for (size_t i = 0; i < src.length; i++) {
        Tri temp_tri;
        temp_tri = ae_project_tri_world2screen(proj_mat, src.elements[i], window_w, window_h, light_direction, scene);
        // AE_PRINT_TRI(temp_tri);
        
        ada_appand(Tri, temp_des, temp_tri);
    }
    *des = temp_des;
}

/* swap: interchange v[i] and v[j] */
void ae_swap_tri(Tri *v, int i, int j)
{
    Tri temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

bool ae_compare_tri(Tri t1, Tri t2)
{
    // int z_min_1 = fmin(t1.points[0].z, fmin(t1.points[1].z, t1.points[2].z));
    // int z_min_2 = fmin(t2.points[0].z, fmin(t2.points[1].z, t2.points[2].z));

    // return z_min_1 > z_min_2;

    // int z_ave_1 = t1.points[0].z + t1.points[1].z + t1.points[2].z;
    // int z_ave_2 = t2.points[0].z + t2.points[1].z + t2.points[2].z;

    // return z_ave_1 > z_ave_2;

    // return t1.center.z > t2.center.z;

    // return t1.z_min > t2.z_min;

    return t1.z_max > t2.z_max;
}

/* qsort: sort v[left]...v[right] int increasing order */
void ae_qsort_tri(Tri *v, int left, int right)
{
    int i, last;

    if (left >= right)                  /* do nothing if array contains */
        return;                         /* fewer than two elements */
    ae_swap_tri(v, left, (left + right) / 2);  /* move partition elem */
    last = left;                        /* to v[0] */
    for (i = left + 1; i <= right; i++) /* partition */
        if (ae_compare_tri(v[i], v[left]))
            ae_swap_tri(v, ++last, i);
    ae_swap_tri(v, left, last); /* restore partition elem */
    ae_qsort_tri(v, left, last - 1);
    ae_qsort_tri(v, last + 1, right);
}


#endif /* ALMOG_ENGINE_IMPLEMENTATION */