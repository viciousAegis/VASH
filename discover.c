#include "headers.h"


/*
TODO:
implement absolute directory
*/

path targetDir;
fileName targetFile;
path foundPath;

int displayAllFiles;

char* discFlags;

void initFilesToSearch()
{
    targetDir = (path) calloc(1024, sizeof(char));
    targetFile = (fileName) calloc(1024, sizeof(char));

    foundPath = (path) calloc(1024, sizeof(char));

    discFlags = (char*) calloc(2, sizeof(char));

    displayAllFiles = 0;
}

void parseFilesAndFlagsInInput()
{
    for(int i = 0; i < argCount; i++)
    {
        if(arguments[i][0] == '"')
        {   
            fileName file = (fileName) calloc(strlen(arguments[i]), sizeof(char));
            file = cleanFileNameString(arguments[i]);
            strcpy(targetFile, file);
        }
        else if(arguments[i][0] == '-')
        {
            arguments[i]++;
            strcat(discFlags, arguments[i]);
        }
        else
        {
            path dir = (path) calloc(strlen(arguments[i]), sizeof(char));
            strcpy(dir, arguments[i]);
            strcpy(targetDir, dir);
        }
    }

    if(strlen(targetFile) == 0)
    {
        displayAllFiles = 1;
    }

    if(strlen(targetDir) == 0)
    {
        strcpy(targetDir, ".");
    }
    else if(targetDir[0] == '~')
    {
        strcpy(targetDir, ".");
    }
}

void performDiscover()
{
    initFilesToSearch();
    parseFilesAndFlagsInInput();

    if(displayAllFiles)
    {
        if(!strlen(discFlags) || strlen(discFlags) == 2)
            displayAllFilesInDir(targetDir);
        else if(discFlags[0] == 'f')
            displayFilesInDir(targetDir);
        else if(discFlags[0] == 'd')
            displayDirsInDir(targetDir);
        return;
    }

    int flag = findFileInDir(targetDir);
    if(!flag) printf("Not found\n");
}

int findFileInDir(path directory)
{
    int searchDirs, searchFiles;

    if(!strlen(discFlags) || strlen(discFlags) == 2)
    {
        searchDirs = 1;
        searchFiles = 1;
    }
    else if(discFlags[0] == 'f')
    {
        searchDirs = 0;
        searchFiles = 1;
    }
    else if(discFlags[0] == 'd')
    {
        searchDirs = 1;
        searchFiles = 0;
    }

    int flag = 0;

    DIR* dir = opendir(directory);
    if(dir == NULL)
    {
        write(2, "Error: directory not found\n", 27);
        return 0;
    }

    struct dirent* file;
    while((file = readdir(dir)) != NULL)
    {
        if(file->d_name[0] == '.')
        {
            continue;
        }

        if(file->d_type == DT_DIR)
        {
            if(searchDirs && !strcmp(targetFile, file->d_name))
            {
                sprintf(foundPath, "%s/%s", directory, file->d_name);
                printf("%s\n", foundPath);
                flag = 1;
            }
            path newDir = (path) calloc(1024, sizeof(char));
            sprintf(newDir, "%s/%s", directory, file->d_name);
            if(flag == 1)
            {
                findFileInDir(newDir);
            }
            else
            {
                flag = findFileInDir(newDir);
            }
        }
        else if(searchFiles && !strcmp(targetFile, file->d_name))
        {
            sprintf(foundPath, "%s/%s", directory, file->d_name);
            printf("%s\n", foundPath);
            flag = 1;
        }
    }
    return flag;
}

void displayAllFilesInDir(path directory)
{
    if(!strcmp(directory, targetDir) && !strcmp(targetDir,"."))
    {
        printf("%s\n", targetDir);
    }
    DIR* dir = opendir(directory);
    if(dir == NULL)
    {
        write(2, "Error: directory not found\n", 27);
        return;
    }

    struct dirent* file;
    while((file = readdir(dir)) != NULL)
    {
        if(file->d_name[0] == '.')
        {
            continue;
        }

        printf("%s/%s\n", directory, file->d_name);

        if(file->d_type == DT_DIR)
        {
            path newDir = (path) calloc(1024, sizeof(char));
            sprintf(newDir, "%s/%s", directory, file->d_name);

            displayAllFilesInDir(newDir);
        }
    }
}

void displayFilesInDir(path directory)
{
    DIR* dir = opendir(directory);
    if(dir == NULL)
    {
        write(2, "Error: directory not found\n", 27);
        return;
    }

    struct dirent* file;
    while((file = readdir(dir)) != NULL)
    {
        if(file->d_name[0] == '.')
        {
            continue;
        }

        if(file->d_type == DT_DIR)
        {
            path newDir = (path) calloc(1024, sizeof(char));
            sprintf(newDir, "%s/%s", directory, file->d_name);

            displayFilesInDir(newDir);
        }

        if(file->d_type == DT_REG)
        {
            printf("%s/%s\n", directory, file->d_name);
        }
    }
}

void displayDirsInDir(path directory)
{
    DIR* dir = opendir(directory);
    if(dir == NULL)
    {
        write(2, "Error: directory not found\n", 27);
        return;
    }

    struct dirent* file;
    while((file = readdir(dir)) != NULL)
    {
        if(file->d_name[0] == '.')
        {
            continue;
        }

        if(file->d_type == DT_DIR)
        {
            printf("%s/%s\n", directory, file->d_name);

            path newDir = (path) calloc(1024, sizeof(char));
            sprintf(newDir, "%s/%s", directory, file->d_name);

            displayDirsInDir(newDir);
        }
    }
}

char* cleanFileNameString(fileName file)
{
    char* fileName = (char*) calloc(strlen(file), sizeof(char));
    strcpy(fileName, file);

    fileName = removeTrailingEscape(fileName);

    fileName = fileName + 1;
    fileName[strlen(fileName)-1] = '\0';

    return fileName;
}