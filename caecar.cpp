#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* caesar (const char* str);

int main()
{
    /*char* str = "password";
    caesar(str);*/

    char* str = caesar("ромашка");
    printf("%s", str);
}

char* caesar (const char* str)
{
    char* code = (char*)calloc(strlen(str) + 1, sizeof(char));

    for (int i = 0; i < strlen(str); i++)
        code[i] = str[i] + 5;

    return code;
}
