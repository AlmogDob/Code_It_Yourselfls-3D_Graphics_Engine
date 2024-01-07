#include "load_from_object_file.h"

#if 0

#define dprintS(expr) printf(#expr " = %s\n", expr)
#define dprintI(expr) printf(#expr " = %d\n", expr)
#define dprintC(expr) printf(#expr " = %c\n", expr)
#define dprintF(expr) printf(#expr " = %g\n", expr)
#define PRINT_TRIANGLE(t, padding) print_triangle(t, padding, #t)
void print_triangle(triangle t, int padding, char *name);

int main()
{
    mesh this_mesh;
    int number_of_triangle = load_from_object_file(&this_mesh, "./lego_man/test_file.obj");
    

    // VEC3_PRINT(this_mesh.tris[0].p[0]);
    // VEC3_PRINT(this_mesh.tris[0].p[1]);
    // VEC3_PRINT(this_mesh.tris[0].p[2]);
    
    // VEC3_PRINT(this_mesh.tris[1].p[0]);
    // VEC3_PRINT(this_mesh.tris[1].p[1]);
    // VEC3_PRINT(this_mesh.tris[1].p[2]);

    // VEC3_PRINT(this_mesh.tris[2].p[0]);
    // VEC3_PRINT(this_mesh.tris[2].p[1]);
    // VEC3_PRINT(this_mesh.tris[2].p[2]);

    // VEC3_PRINT(this_mesh.tris[3].p[0]);
    // VEC3_PRINT(this_mesh.tris[3].p[1]);
    // VEC3_PRINT(this_mesh.tris[3].p[2]);

    dprintI(number_of_triangle);
    PRINT_TRIANGLE(this_mesh.tris[number_of_triangle-3], 4);
    PRINT_TRIANGLE(this_mesh.tris[number_of_triangle-2], 4);
    PRINT_TRIANGLE(this_mesh.tris[number_of_triangle-1], 4);

    // char s[20];
    // my_getline(s, 20, stdin);
    // printf("%s",s);
    return 0;
}

void print_triangle(triangle t, int padding, char *name)
{
    printf("%s\n", name);
    printf("%*s", padding,""); VEC3_PRINT(t.p[0]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[1]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[2]);
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
    int verts_counter = 0, triangles_counter = 0,
        i, j, length, test_counter = 0, number_int_1,
        number_int_2, number_int_3, number_int_4, is_quad;
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
        Vec3_init_vec(&verts[verts_counter]);
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
                    // printf("c is newline 1\n");
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
                // verts[verts_counter].w = 1;
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
            is_quad = 0;
            for (i = 2, j = 0; (c = line[i]) != '\n'; i++) {
                /*test*/
                // dprintC(c);
                /*test*/
                while (!isspace(c) && c != '/') {
                    word[j++] = line[i];
                    /*test*/
                    // dprintI(i);
                    // dprintI(j);
                    /*test*/
                
                    c = line[++i];
                    /*test*/
                    // dprintC(c);
                    /*test*/
                }
                // dprintC(c);
                word[j] = '\0';
                // dprintS(word);
                number_int_1 = atoi(word);
                this_mesh->tris[triangles_counter].p[0] = verts[number_int_1 - 1];
                // dprintI(number_int_1);

                if (c == '/') {
                    // printf("hi\n");
                    c = line[++i];
                    /*test*/
                    // dprintC(c);
                    /*test*/
                    while (!isspace(c) && c != '/') {
                    
                        /*test*/
                        // dprintI(i);
                        // dprintI(j);
                        /*test*/
                        
                        c = line[++i];
                        /*test*/
                        // dprintC(c);
                        /*test*/
                    }
                }

                j = 0;
                
                
                if (c == ' ') {
                    c = line[++i];
                    /*test*/
                    // printf("c was ' ' but now\n");
                    // dprintC(c);
                    
                    /*test*/
                }

                while (!isspace(c) || c =='/') {
                    word[j++] = line[i];
                    /*test*/
                    // dprintI(i);
                    // dprintI(j);
                    /*test*/
                    i++;
                    c = line[i];
                    /*test*/
                    // dprintC(c);
                    /*test*/
                }
                word[j] = '\0';
                number_int_2 = atoi(word);
                this_mesh->tris[triangles_counter].p[1] = verts[number_int_2 - 1];
                // dprintI(number_int_2);

                if (c == '/') {
                    // printf("hi\n");
                    c = line[++i];
                    /*test*/
                    // dprintC(c);
                    /*test*/
                    while (!isspace(c) && c != '/') {
                    
                        /*test*/
                        // dprintI(i);
                        // dprintI(j);
                        /*test*/

                        c = line[++i];
                        /*test*/
                        // dprintC(c);
                        /*test*/
                    }
                }

                j = 0;
                if (c == '\n') {
                    // printf("c is newline 2\n");
                    break;
                }
                c = line[++i];
                /*test*/
                // dprintC(c);
                /*test*/
                if (c == ' ') {
                    c = line[++i];
                    /*test*/
                    // dprintC(c);
                    /*test*/
                }
                
                while (!isspace(c) && c != '/') {
                    word[j++] = line[i];
                    /*test*/
                    // dprintI(i);
                    // dprintI(j);
                    /*test*/
                    i++;
                    c = line[i];
                    /*test*/
                    // dprintC(c);
                    /*test*/
                }
                word[j] = '\0';
                number_int_3 = atoi(word);
                this_mesh->tris[triangles_counter].p[2] = verts[number_int_3 - 1];
                // dprintI(number_int_3);


                if (c == '/') {
                    // printf("hi\n");
                    c = line[++i];
                    /*test*/
                    // dprintC(c);
                    /*test*/
                    while (!isspace(c) && c != '/') {
                    
                        /*test*/
                        // dprintI(i);
                        // dprintI(j);
                        /*test*/

                        c = line[++i];
                        /*test*/
                        // dprintC(c);
                        /*test*/
                    }
                }
                j = 0;   

                triangles_counter++;       

                if (c == ' ') {
                    is_quad = 1;
                } 
                else {
                    break;
                }

                if (is_quad) {
                    /*test*/
                    // printf("quad is 1\n");
                    /*test*/
                    c = line[++i];    
                    /*test*/
                    // dprintC(c);
                    /*test*/
                    if (c == ' ') {
                        c = line[++i];
                        /*test*/
                        // dprintC(c);
                        /*test*/
                    }   
                    
                    while (!isspace(c) && c != '/') {
                        word[j++] = line[i];
                        /*test*/
                        // dprintI(i);
                        // dprintI(j);
                        /*test*/
                        i++;
                        c = line[i];
                        /*test*/
                        // dprintC(c);
                        /*test*/
                    }
                    word[j] = '\0';
                    number_int_4 = atoi(word);
                    // dprintI(number_int_4);
                    
                    // if (c == '/') {
                    //     // printf("hi\n");
                    //     c = line[++i];
                    //     /*test*/
                        // dprintC(c);
                    //     /*test*/
                    //     while (!isspace(c) && c != '/') {
                        
                    //         /*test*/
                    //         // dprintI(i);
                    //         // dprintI(j);
                    //         /*test*/

                    //         c = line[++i];
                    //         /*test*/
                            // dprintC(c);
                    //         /*test*/
                    //         if (c == ' ') {
                    //             c = line[++i];
                    //             /*test*/
                                // dprintC(c);
                    //             /*test*/
                    //         }
                    //     }
                    // }
                }

                
                

                if (is_quad) {
                    this_mesh->tris[triangles_counter].p[0] = verts[number_int_1 - 1];
                    this_mesh->tris[triangles_counter].p[1] = verts[number_int_3 - 1];
                    this_mesh->tris[triangles_counter].p[2] = verts[number_int_4 - 1];
                    triangles_counter++;
                }
                // dprintI(triangles_counter);
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
