#ifndef __VARIABLES_H
#define __VARIABLES_H

//for file paths
typedef char* path;

//for input buffer strings
typedef char* input;

// command word
typedef char* cmdWord;

char* vash;

cmdWord commandWord;

path rootPath;

input bufferInput;

input* arguments;

int argCount;

path currDirectory;

path prevDirectory;

#endif