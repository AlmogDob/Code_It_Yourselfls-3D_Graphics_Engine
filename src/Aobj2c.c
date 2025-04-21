#include <stdio.h>
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./../include/Almog_String_Manipulation.h"

int main()
{
    char current_line[MAX_LEN_LINE];

    int line_len = asm_get_line(stdin, current_line);

    printf("current line: %s\n", current_line);
    return 0;
}
