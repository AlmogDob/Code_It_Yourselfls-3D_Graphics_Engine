#include <stdio.h>
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./../include/Almog_String_Manipulation.h"
#include "./../include/Almog_Engine.h"
#include "./../include/Almog_Dynamic_Array.h"

typedef struct {
    size_t length;
    size_t capacity;
    Point* elements;
} Points;

void print_points(Points p)
{
    for (size_t i = 0; i < p.length; i++) {
        printf("point %3zu: (%5f, %5f, %5f)\n", i, p.elements[i].x, p.elements[i].y, p.elements[i].z);
    }
}

int main()
{
    char current_line[MAX_LEN_LINE];

    Points points;
    ada_init_array(Point, points);
    Point p = {.x = 1, .y = 2, .z = 3};
    ada_appand(Point, points, p);

    int line_len = asm_get_line(stdin, current_line);

    printf("current line: %s\n", current_line);

    print_points(points);
    return 0;
}
