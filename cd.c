#include "headers.h"

/*
    flags for cd:
    cd .
    cd ..
    cd ~
    cd -
    cd
*/

path absoluteFilePath;
path relFilePath;

void performCD()
{
    absoluteFilePath = (path) calloc(1024, sizeof(char));
    relFilePath = arguments;

    relFilePath = removeTrailingEscape(relFilePath);

    getcwd(currDirectory, 1024);

    //checking for flags
    if(!strcmp(relFilePath, "") || !strcmp(relFilePath, "~"))
    {
        goToRootDirectory();
    }
    else if(!strcmp(relFilePath, "."))
    {
        prevDirectory = currDirectory;
        //do nothing
    }
    else if(!strcmp(relFilePath, ".."))
    {
        //go up one directory
        goUpOneDirectory();
    }
    else if(!strcmp(relFilePath, "-"))
    {
        //go to previous directory
        goToPreviousDirectory();
    }
    else
    {
        goToSpecifiedDirectory();
    }
}

void goToRootDirectory()
{
    prevDirectory = currDirectory;

    absoluteFilePath = rootPath;
    changeDir();
}

void goUpOneDirectory()
{
    prevDirectory = currDirectory;

    if(!strcmp(currDirectory,rootPath)) {
        performPWD();
        return;
    }

    char testByte = currDirectory[strlen(currDirectory) - 1];
    while(testByte != '/')
    {
        currDirectory[strlen(currDirectory) - 1] = '\0';
        testByte = currDirectory[strlen(currDirectory) - 1];
    }
    currDirectory[strlen(currDirectory) - 1] = '\0';

    absoluteFilePath = currDirectory;
    changeDir();
}

void goToPreviousDirectory()
{
    absoluteFilePath = prevDirectory;
    prevDirectory = currDirectory;
    
    changeDir();
}

void goToSpecifiedDirectory()
{
        sprintf(absoluteFilePath, "%s/%s", currDirectory, relFilePath);

        int flag = changeDir();
        if(flag == 0) {
            prevDirectory = currDirectory;
        }
        return;
}

int changeDir()
{
    int flag = chdir(absoluteFilePath);

    if(flag == -1)
    {
        printf("Error: %s\n", strerror(errno));
    }
    return flag;
}

char* removeTrailingEscape(char* relFilePath)
{
    int len = strlen(relFilePath);
    if(relFilePath[len - 1] == '\n')
    {
        relFilePath[len - 1] = '\0';
    }
    return relFilePath;
}