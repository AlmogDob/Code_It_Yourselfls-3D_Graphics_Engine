#include <stdio.h>
#include "./include/Almog_Dynamic_Array.h"
#include <string.h>

typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array;

void print_int_array(ada_int_array a)
{
    printf("[");
    for (size_t i = 0; i < a.length-1; i++) {
        printf("%d, ", a.elements[i]);
    }
    printf("%d]\n", a.elements[a.length-1]);
}

int main()
{
    ada_int_array x;
    ada_init_array(int, x);

    for (int i = 0; i < 10; i++) {
        ada_appand(int, x, i);
    }

    print_int_array(x);

    ada_insert(int, x, 15, 1);

    print_int_array(x);

    ada_remove(int, x, 1);

    print_int_array(x);

    ada_remove_unordered(int, x, 1);

    print_int_array(x);

    return 0;
}
