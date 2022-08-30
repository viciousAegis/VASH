#include "headers.h"

void performPWD()
{
    char* pwd = getcwd(NULL, 0);
    printf("%s\n", pwd);
}