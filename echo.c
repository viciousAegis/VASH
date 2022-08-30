#include "headers.h"

void performEcho()
{
    if(!strcmp(arguments, ""))
    {
        printf("incorrect format. Please enter arguments after echo\n");
        return;
    }
    printf("%s", arguments);
}