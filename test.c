#define VEC3_IMPLEMENTATION
#include "./src/Vec3.h"

#define MATRIX_IMPLEMENTATION
#include "./src/Matrix.h"

#define NUM_OF_TRIANGLES 100
#define PRINT_TRIANGLE(v, padding) print_triangle(v, padding, #v)

#define dprintS(expr) printf(#expr " = %s\n", expr)
#define dprintI(expr) printf(#expr " = %d\n", expr)
#define dprintC(expr) printf(#expr " = %c\n", expr)
#define dprintF(expr) printf(#expr " = %g\n", expr)

typedef struct {
    Vec3 p[3];
} triangle;

typedef struct {
    triangle tris[NUM_OF_TRIANGLES];
} mesh;

void print_triangle(triangle t, int padding, char *name);

int main(void)
{
    int a, b, c, *d;

    a = 1;
    c = a;
    a = 2;

    dprintI(a);
    dprintI(c);
    d = &c;
    dprintI(*d);

    a = 3;
    d = &a;
    
    dprintI(a);
    dprintI(*d);

    *d = 4;
    dprintI(a);
    dprintI(c);
    dprintI(*d);
    return 0;
}


void print_triangle(triangle t, int padding, char *name)
{
    printf("%s\n", name);
    printf("%*s", padding,""); VEC3_PRINT(t.p[0]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[1]);
    printf("%*s", padding,"");VEC3_PRINT(t.p[2]);
}