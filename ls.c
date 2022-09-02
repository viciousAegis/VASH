#include "headers.h"

/*
    flags:
    ls .
    ls ..
    ls -a
    ls -l
    ls -la
    ls -l -a
    ls <dir>
    ls <dir> -a
    ls dir dir dir
*/

fileName* fileNames;
int fileCount = 0;

char* flags;

path* directories;
int dirCount = 0;

void parseFlagsAndDirsInInput()
{
    dirCount = 0;
    flags = (char*) calloc(2, sizeof(char));
    directories = (path*) calloc(1024, sizeof(path));
    for(int i = 0; i < argCount; i++)
    {
        if(arguments[i][0] == '-')
        {
            arguments[i]++;
            strcat(flags, arguments[i]);
        }
        else
        {
            directories[dirCount++] = arguments[i];
        }
    }

    if(dirCount == 0)
    {
        directories[dirCount] = ".";
        return;
    }
    qsort(directories, dirCount, sizeof(path), compare);
}

void performLS()
{
    parseFlagsAndDirsInInput();
    initFileNames();

    if(!dirCount)
    {
        // ls -flags
        initMyDir(".");
        if(strlen(flags))
        {
            if(strchr(flags, 'a'))
            {
                printAllDir();
            }
        }
        else // ls
        {
            printDir();
        }
    }
    else
    {
        // ls <dir> <dir> <dir>
        for(int i = 0; i < dirCount; i++)
        {
            initMyDir(directories[i]);
            printf("%s:\n", directories[i]);
            if(strlen(flags))
            {
                if(strchr(flags, 'a'))
                {
                    printAllDir();
                }
            }
            else // ls
            {
                printDir();
            }
            printf("\n");
        }
    }
}

int compare(const void* a, const void* b)
{
    return strcmp(*(char**)a, *(char**)b);
}

void initFileNames()
{
    fileNames = (fileName*) calloc(100, sizeof(fileName));
    for(int i = 0; i < 100; i++)
    {
        fileNames[i] = (fileName) calloc(1024, sizeof(char));
    }
}

void initMyDir(path dirPath)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(dirPath)) != NULL)
    {
        fileCount = 0;
        while ((ent = readdir(dir)) != NULL)
        {
            strcpy(fileNames[fileCount++], ent->d_name);
        }
        closedir(dir);
    }
    else
    {
        perror("");
        exit(1);
    }

    qsort(fileNames, fileCount, sizeof(fileName), compare);
}

void printDir()
{
    for(int i = 0; i < fileCount; i++)
    {
        if(fileNames[i][0] != '.')
            printf("%s\n", fileNames[i]);
    }
}

void printAllDir()
{
    for(int i = 0; i < fileCount; i++)
    {
        printf("%s\n", fileNames[i]);
    }
}