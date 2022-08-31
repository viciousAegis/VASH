#include "headers.h"

void performEcho()
{
    char* toPrint = (char*) calloc(1024, sizeof(char));
    sprintf(toPrint, "%s", arguments[0]);
    if(toPrint == NULL)
    {
        printf("\n");
    }
    for(int i = 1; i < argCount; i++)
    {
        sprintf(toPrint, "%s %s", toPrint, arguments[i]);
    }
    printf("%s\n", toPrint);
}