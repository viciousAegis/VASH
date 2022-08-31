#include "headers.h"

//Vicious A3gis Shell
char* vash = "vash";

// the root path of the shell
path rootPath;

//the last used path
path prevDirectory;

// input from the user
input bufferInput;

// arguments for the command
input* arguments;

path currDirectory;