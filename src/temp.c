#include <stdio.h>
// #define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./../include/Almog_String_Manipulation.h"
#define ALMOG_ENGINE_IMPLEMENTATION
#include "./../include/Almog_Engine.h"

int main()
{
    Mesh cube = ae_create_cube(2);
    float xmax, xmin, ymax, ymin, zmax, zmin;

    ae_set_mesh_bounding_box(cube, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);

    printf("xmin: %f, xmax: %f\nymin: %f, ymax: %f\nzmin: %f, zmax: %f\n\n", xmin, xmax, ymin, ymax, zmin, zmax);

    ae_normalize_mesh(cube);

    ae_set_mesh_bounding_box(cube, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
    printf("xmin: %f, xmax: %f\nymin: %f, ymax: %f\nzmin: %f, zmax: %f\n\n", xmin, xmax, ymin, ymax, zmin, zmax);

    return 0;
}
