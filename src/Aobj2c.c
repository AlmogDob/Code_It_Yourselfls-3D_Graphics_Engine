#include <stdio.h>
#include <string.h>
#include <errno.h>
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./../include/Almog_Engine.h"
#include "./../include/Almog_String_Manipulation.h"
#include "./../include/Almog_Dynamic_Array.h"

#define AE_PRINT_TRI(tri) ae_print_tri(tri, #tri, 0)
#define AE_PRINT_MESH(mesh) ae_print_mesh(mesh, #mesh, 0)

int main(int argc, char const *argv[])
{
    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE], current_num_str[MAX_LEN_LINE];
    char file_name[MAX_LEN_LINE], file_extention[MAX_LEN_LINE], mesh_name[MAX_LEN_LINE], output_file_name[MAX_LEN_LINE];
    int texture_warning_was_printed = 0;

    if (--argc != 1) {
        fprintf(stderr, "%s:%d: [Error] not the right usage: ./Aobj2c 'file name'\n", __FILE__, __LINE__);
        exit(1);
    }
    strncpy(file_name, *(++argv), MAX_LEN_LINE);
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

    strncpy(output_file_name, "./build/", MAX_LEN_LINE);
    strncat(output_file_name, mesh_name, MAX_LEN_LINE/2);
    strncat(output_file_name, ".c", MAX_LEN_LINE/2);
    FILE *fp_output = fopen(output_file_name, "wt");
    if (fp_input == NULL) {
        fprintf(stderr, "%s:%d: [Error] failed to open output file: '%s'. %s\n", __FILE__, __LINE__, output_file_name, strerror(errno));
        exit(1);
    }

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

    // print_points(points);
    // AE_PRINT_MESH(mesh);

    // if (asm_length(file_name) == 0) {
    //     fprintf(stderr, "%s:%d: [Error] could not find file name in the file\n", __FILE__, __LINE__);
    //     exit(1);
    // }

    /* outputing data to stdout */
    fprintf(fp_output, "Mesh_static %s;\n%s.length = %zu;\nTri temp_tri_vec[] = {", mesh_name, mesh_name, mesh.length);
        for (size_t i = 0; i < mesh.length; i++) {
            fprintf(fp_output, "{{");
            fprintf(fp_output, "{%f", mesh.elements[i].points[0].x);
            fprintf(fp_output, ", %f", mesh.elements[i].points[0].y);
            fprintf(fp_output, ", %f}", mesh.elements[i].points[0].z);
            for (size_t j = 1; j < 3; j++) {
                fprintf(fp_output, ", {%f", mesh.elements[i].points[j].x);
                fprintf(fp_output, ", %f", mesh.elements[i].points[j].y);
                fprintf(fp_output, ", %f}", mesh.elements[i].points[j].z);
            }
            fprintf(fp_output, "}, ");
            fprintf(fp_output, "{%f", mesh.elements[i].center.x);
            fprintf(fp_output, ", %f", mesh.elements[i].center.y);
            fprintf(fp_output, ", %f}", mesh.elements[i].center.z);
            fprintf(fp_output, ", %f, %f, 1, 1},\n", mesh.elements[i].z_min, mesh.elements[i].z_max);
        }
    fprintf(fp_output, "};\n");
    fprintf(fp_output, "%s.elements = temp_tri_vec;\n", mesh_name);


    free(points.elements);
    free(mesh.elements);
    fclose(fp_input);
    fclose(fp_output);

    return 0;
}
