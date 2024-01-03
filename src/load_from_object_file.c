

#include "load_from_object_file.h"

#if 0

#define dprintS(expr) printf(#expr " = %s\n", expr)
#define dprintI(expr) printf(#expr " = %d\n", expr)
#define dprintC(expr) printf(#expr " = %c\n", expr)
#define dprintF(expr) printf(#expr " = %g\n", expr)

int main()
{
    mesh this_mesh;
    int number_of_triangle = load_from_object_file(&this_mesh, "./simple_shape/simple_shape.obj");
    
    VEC3_PRINT(this_mesh.tris[0].p[0]);
    VEC3_PRINT(this_mesh.tris[0].p[1]);
    VEC3_PRINT(this_mesh.tris[0].p[2]);

    dprintI(number_of_triangle);

    VEC3_PRINT(this_mesh.tris[number_of_triangle-1].p[0]);
    VEC3_PRINT(this_mesh.tris[number_of_triangle-1].p[1]);
    VEC3_PRINT(this_mesh.tris[number_of_triangle-1].p[2]);
    // char s[20];
    // my_getline(s, 20, stdin);
    // printf("%s",s);
    return 0;
}

#endif 



/* getline: read a line, return length */
int my_getline(char *line, int max, FILE *file)
{
    if (fgets(line,max,file) == NULL) {
        return 0;
    }
    else {
        return strlen(line);
    }
}

int load_from_object_file(mesh *this_mesh, char *file_name)
{
    /*test*/
    // printf("load\n");
    /*test*/
    FILE *fp;
    Vec3 verts[MAX_NUM_OF_VERTS];
    int verts_counter = 0, triangles_counter = 0, i, j, length, test_counter = 0, number_int;
    char line[MAXLINE], c, word[MAXWORD];
    float number;

    if ((fp = fopen(file_name, "r")) == NULL) {
        fprintf(stderr, "Error: can't open %s\n", file_name);
        return 0;
    }
    /*test*/
    // printf("before first while\n");
    /*test*/
    while((length = my_getline(line, MAXLINE, fp)) != 0) {
        if (line[0] == 'v' && line[1] == ' ') {
            for (i = 2, j = 0; (c = line[i]) != '\n'; i++) {
                /*test*/
                // dprintC(c);
                /*test*/
                while (!isspace(c)) {
                    word[j++] = line[i];
                    /*test*/
                    // dprintI(i);
                    // dprintI(j);
                    /*test*/

                    c = line[++i];
                }
                word[j] = '\0';
                // dprintS(word);
                number = (float)atof(word);
                verts[verts_counter].x = number;
                // dprintF(number);

                j = 0;
                if (c == '\n') {
                    printf("c is newline 1\n");
                    break;
                }
                c = line[++i];

                while (!isspace(c)) {
                    word[j++] = line[i];
                    /*test*/
                    // dprintI(i);
                    // dprintI(j);
                    /*test*/
                    i++;
                    c = line[i];
                }
                word[j] = '\0';
                number = (float)atof(word);
                verts[verts_counter].y = number;
                // dprintF(number);

                j = 0;
                if (c == '\n') {
                    printf("c is newline 2\n");
                    break;
                }
                c = line[++i];
                
                while (!isspace(c)) {
                    word[j++] = line[i];
                    /*test*/
                    // dprintI(i);
                    // dprintI(j);
                    /*test*/
                    i++;
                    c = line[i];
                }
                word[j] = '\0';
                number = (float)atof(word);
                verts[verts_counter].z = number;
                // dprintF(verts[verts_counter].z);
                // dprintF(number);

                j = 0;

                // VEC3_PRINT(verts[verts_counter]);
                // dprintI(verts_counter);
                verts_counter++;

                /*test*/               
                // dprintI(i);
                break;
                // test_counter++;
                /*test*/
            }
        }
        
        if (line[0] == 'f' && line[1] == ' ') {
            for (i = 2, j = 0; (c = line[i]) != '\n'; i++) {
                /*test*/
                // dprintC(c);
                /*test*/
                while (!isspace(c)) {
                    word[j++] = line[i];
                    /*test*/
                    // dprintI(i);
                    // dprintI(j);
                    /*test*/

                    c = line[++i];
                }
                word[j] = '\0';
                // dprintS(word);
                number_int = atoi(word);
                this_mesh->tris[triangles_counter].p[0] = verts[number_int - 1];
                // dprintF(number_int);

                j = 0;
                if (c == '\n') {
                    printf("c is newline 1\n");
                    break;
                }
                c = line[++i];

                while (!isspace(c)) {
                    word[j++] = line[i];
                    /*test*/
                    // dprintI(i);
                    // dprintI(j);
                    /*test*/
                    i++;
                    c = line[i];
                }
                word[j] = '\0';
                number_int = atoi(word);
                this_mesh->tris[triangles_counter].p[1] = verts[number_int - 1];
                // dprintF(number_int);

                j = 0;
                if (c == '\n') {
                    printf("c is newline 2\n");
                    break;
                }
                c = line[++i];
                
                while (!isspace(c)) {
                    word[j++] = line[i];
                    /*test*/
                    // dprintI(i);
                    // dprintI(j);
                    /*test*/
                    i++;
                    c = line[i];
                }
                word[j] = '\0';
                number_int = atoi(word);
                this_mesh->tris[triangles_counter].p[2] = verts[number_int - 1];
                // dprintF(number_int);

                j = 0;

                // dprintI(triangles_counter);
                triangles_counter++;

                /*test*/               
                // dprintI(i);
                break;
                // test_counter++;
                /*test*/
            }
            ;
        }
        /*test*/
        if (test_counter > 1.5e4) {
            // dprintI(test_counter);
            // break;
        }
        test_counter++;
        /*test*/
    }
    fclose(fp);
    return triangles_counter;
}
