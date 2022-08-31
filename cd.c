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
    if(argCount > 1)
    {
        printf("cd: too many arguments\n");
        return;
    }
    
    absoluteFilePath = (path) calloc(1024, sizeof(char));
    relFilePath = arguments[0];

    if(strlen(relFilePath) > 1 && relFilePath[0] == '~')
    {
        currDirectory = rootPath;
        relFilePath = removeLeadingTilde(relFilePath);
    }
    else
    {
        getcwd(currDirectory, 1024);
    }


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
    else if(!strcmp(relFilePath, "..") || !strcmp(relFilePath, "/.."))
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

    // print the new directory
    performPWD();
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

path removeLeadingTilde(path relFilePath)
{
    if(relFilePath[0] == '~')
    {
        relFilePath++;
    }
    return relFilePath;
}
