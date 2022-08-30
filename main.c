#include "headers.h"

int main()
{
    while (1)
    {
        prompt();
        char a[256];
        scanf("%s", a);
        // TAKE INPUT HERE

        if(!strcmp(strtok(a, " \t"), "echo "))
        {
            echo(strtok(NULL, " \t"));
        }
    }
}
