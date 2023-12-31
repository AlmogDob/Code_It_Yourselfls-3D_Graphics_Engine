#define VEC3_IMPLEMENTATION
#include "Vec3.h"

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
    triangle t;

    t.p[0] = Vec3_new(0,0,0);
    t.p[1] = Vec3_new(0,1,0);
    t.p[2] = Vec3_new(1,1,0);

    PRINT_TRIANGLE(t, 4);

    return 0;
}


void print_triangle(triangle t, int padding, char *name)
{
    printf("%s\n", name);
    printf("%*s", padding,""); VEC3_PRINT(t.p[0]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[1]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[2]);
}