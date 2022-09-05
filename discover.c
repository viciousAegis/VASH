#include "headers.h"

path targetDir;
fileName targetFile;

void performDiscover()
{
    for(int i = 0; i < argCount; i++)
    {
        printf("%s\n", arguments[i]);
    }
}   