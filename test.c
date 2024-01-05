#define VEC3_IMPLEMENTATION
#include "./src/Vec3.h"

#define MATRIX_IMPLEMENTATION
#include "./src/Matrix.h"

#define NUM_OF_TRIANGLES 100
#define PRINT_TRIANGLE(v, padding) print_triangle(v, padding, #v)

typedef struct {
    Vec3 p[3];
} triangle;

typedef struct {
    triangle tris[NUM_OF_TRIANGLES];
} mesh;

void print_triangle(triangle t, int padding, char *name);

int main(void)
{
    Mat m;

    m = mat_alloc(5,5);
    mat_identity_mat(m);
    MAT_PRINT(m);
    return 0;
}


void print_triangle(triangle t, int padding, char *name)
{
    printf("%s\n", name);
    printf("%*s", padding,""); VEC3_PRINT(t.p[0]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[1]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[2]);
}