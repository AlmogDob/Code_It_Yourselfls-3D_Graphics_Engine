#include <stdio.h>
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./../include/Almog_String_Manipulation.h"

int main()
{
    char src[] = " 1/1/1 2/2/2 3/3/3 4/4/4";

    printf("\n%d\n", asm_str_in_str(src, " "));

    return 0;
}
