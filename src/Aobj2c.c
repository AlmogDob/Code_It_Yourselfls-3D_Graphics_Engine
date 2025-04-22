#include <stdio.h>
#include <string.h>
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./../include/Almog_String_Manipulation.h"
#include "./../include/Almog_Engine.h"
#include "./../include/Almog_Dynamic_Array.h"

typedef struct {
    size_t length;
    size_t capacity;
    Point* elements;
} Points;

#define AE_PRINT_TRI(tri) ae_print_tri(tri, #tri, 0)
#define AE_PRINT_MESH(mesh) ae_print_mesh(mesh, #mesh, 0)

void print_points(Points p)
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

int main()
{
    /* parsing data from stdin */
    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE];
    char file_name[MAX_LEN_LINE];

    Points points;
    ada_init_array(Point, points);
    Mesh mesh;
    ada_init_array(Tri, mesh);

    int line_len;
    while ((line_len = asm_get_line(stdin, current_line)) != -1) {
        asm_get_next_word_from_line(current_word, current_line, ' ');
        if (!strncmp(current_word, "#", 1)) {
            asm_get_word_and_cut(current_word, current_line, ' ');
            asm_get_word_and_cut(current_word, current_line, ' ');
            // printf("current word: %s\n", current_word);
            if (!strcmp(current_word, "Blender")) {
                asm_get_word_and_cut(current_word, current_line, '\'');
                asm_get_word_and_cut(current_word, current_line, '\'');
                asm_get_word_and_cut(current_word, current_line, '.');
                strcpy(file_name, current_word);
            }
        }
        if (!strncmp(current_word, "'", 1)) {
            asm_get_word_and_cut(current_word, current_line, '\'');
            asm_get_word_and_cut(current_word, current_line, '.');
            // printf("current word: %s\n", current_word);
            // printf("current line: %s\n", current_line);
            // exit(1);
            strcpy(file_name, current_word);
        }
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
            Tri tri;
            asm_get_word_and_cut(current_word, current_line, ' ');
            asm_get_word_and_cut(current_word, current_line, ' ');
            tri.points[0] = points.elements[atoi(current_word)-1];
            asm_get_word_and_cut(current_word, current_line, ' ');
            tri.points[1] = points.elements[atoi(current_word)-1];
            int temp = asm_get_word_and_cut(current_word, current_line, ' ');
            tri.points[2] = points.elements[atoi(current_word)-1];

            tri.to_draw = true;

            if (temp != 1) {
                fprintf(stderr, "%s:%d: [Warning] there are more then three vertices in each face\n", __FILE__, __LINE__);
                exit(1);
            }

            ada_appand(Tri, mesh, tri);
            // printf("current line: %s\n", current_line);
            // printf("current word: %s\n", current_word);
            // printf("temp: %d\n", temp);
            // break;
        }
    }

    // print_points(points);
    // AE_PRINT_MESH(mesh);

    if (asm_length(file_name) == 0) {
        fprintf(stderr, "%s:%d: [Error] could not find file name in the file\n", __FILE__, __LINE__);
        exit(1);
    }

    /* outputing data to stdout */
    printf("Mesh_static %s;\n%s.length = %zu;\nTri temp_tri_vec[] = {", file_name, file_name, mesh.length);
        for (size_t i = 0; i < mesh.length; i++) {
            printf("{{");
            printf("{%f", mesh.elements[i].points[0].x);
            printf(", %f", mesh.elements[i].points[0].y);
            printf(", %f}", mesh.elements[i].points[0].z);
            for (size_t j = 1; j < 3; j++) {
                printf(", {%f", mesh.elements[i].points[j].x);
                printf(", %f", mesh.elements[i].points[j].y);
                printf(", %f}", mesh.elements[i].points[j].z);
            }
            printf("}, 1},\n");
        }
    printf("};\n");
    printf("%s.elements = temp_tri_vec;\n", file_name);


    free(points.elements);
    free(mesh.elements);

    return 0;
}
