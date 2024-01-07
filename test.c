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
    Mat m, n, dst;

    m = mat_alloc(3,3);
    n = mat_alloc(3,3);
    dst = mat_alloc(3,3);
    mat_identity_mat(n);
    mat_identity_mat(m);
    MAT_AT(m,2,2) = 0;
    MAT_AT(m,1,2) = 1;
    MAT_PRINT(m);
    MAT_AT(n,0,0) = 2;
    MAT_AT(n,2,1) = 1;
    MAT_PRINT(n);
    mat_dot(dst, n, m);
    MAT_PRINT(dst);
    return 0;
}


void print_triangle(triangle t, int padding, char *name)
{
    printf("%s\n", name);
    printf("%*s", padding,""); VEC3_PRINT(t.p[0]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[1]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[2]);
}