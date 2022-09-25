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

/*
0- file
1- directory
-1 - executable
*/

lsFileCount = 0;

char* flags;

path* directories;
int dirCount = 0;

void parseFlagsAndDirsInInput()
{
    dirCount = 0;
    flags = (char*) calloc(32, sizeof(char));
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

        if(strlen(flags) > 0)
        {
            for(int i = 0; i < strlen(flags); i++)
            {
                if(flags[i] != 'a' && flags[i] != 'l')
                {
                    printErrorMsg("ls: invalid flags\n");
                    return;
                }
            }

            if(!strcmp(flags, "a"))
            {
                printAllDir();
            }
            else if(!strcmp(flags, "l"))
            {
                printDirWithInfo(".", 0);
            }
            else if(strchr(flags, 'a') && strchr(flags, 'l'))
            {
                printDirWithInfo(".", 1);
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
            for(int i = 0; i < strlen(flags); i++)
            {
                if(flags[i] != 'a' && flags[i] != 'l')
                {
                    printErrorMsg("ls: invalid flags\n");
                    return;
                }
            }
            
            int flag = initMyDir(directories[i]);
            if(flag == -1)
            {
                // that means its not a directory
                handleFiles(directories[i]);
                continue;
            }

            if(dirCount != 1)
                printf("%s:\n", directories[i]);

            if(strlen(flags)>0)
            {
                if(!strcmp(flags, "a"))
                {
                    printAllDir();
                }
                else if(!strcmp(flags, "l"))
                {
                    printDirWithInfo(directories[i], 0);
                }
                else if(strchr(flags, 'a') && strchr(flags, 'l'))
                {
                    printDirWithInfo(directories[i], 1);
                }
            }
            else // ls
            {
                printDir();
            }

            if(i != dirCount - 1)
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
    fileNames = (fileName*) calloc(1024, sizeof(fileName));
    for(int i = 0; i < 100; i++)
    {
        fileNames[i] = (fileName) calloc(1024, sizeof(char));
    }

    areDirs = (int*) calloc(100, sizeof(int));
}

int initMyDir(path dirPath)
{
    DIR *dir;
    struct dirent *ent;
    struct stat fileStat;

    if ((dir = opendir(dirPath)) != NULL)
    {
        lsFileCount = 0;
        while ((ent = readdir(dir)) != NULL)
        {
            strcpy(fileNames[lsFileCount++], ent->d_name);
        }
        closedir(dir);
    }
    else
    {
        return -1;
    }

    qsort(fileNames, lsFileCount, sizeof(fileName), compare);

    for(int i = 0; i < lsFileCount; i++)
    {
        path filePath = (path) calloc(1024, sizeof(char));
        sprintf(filePath, "%s/%s", dirPath, fileNames[i]);

        stat(filePath, &fileStat);
        if(S_ISDIR(fileStat.st_mode))
        {
            areDirs[i] = 1;
        }
        else if(fileStat.st_mode & S_IXUSR || fileStat.st_mode & S_IXGRP || fileStat.st_mode & S_IXOTH)
        {
            areDirs[i] = -1;
        }
        else
        {
            areDirs[i] = 0;
        }
    }

    return 0;
}

void printDir()
{
    for(int i = 0; i < lsFileCount; i++)
    {
        if(fileNames[i][0] != '.')
        {
            if(areDirs[i] == 1)
            {
                printf("\033[1;34m");
                printf("%s\n", fileNames[i]);
                printf("\033[0m");
            }
            else if(areDirs[i] == -1)
            {
                printf("\033[1;31m");
                printf("%s\n", fileNames[i]);
                printf("\033[0m");
            }
            else
            {
                printf("%s\n", fileNames[i]);
            }
        }
    }
}

void printAllDir()
{
    for(int i = 0; i < lsFileCount; i++)
    {
        if(areDirs[i] == 1)
        {
            // directories colored in blue
            printf("\033[1;34m");
            printf("%s\n", fileNames[i]);
            printf("\033[0m");
        }
        else if(areDirs[i] == -1)
        {
            // executable files colored in red
            printf("\033[1;31m");
            printf("%s\n", fileNames[i]);
            printf("\033[0m");
        }
        else
        {
            // files colored in white
            printf("%s\n", fileNames[i]);
        }
    }
}

void printDirWithInfo(path dirPath, int isAll)
{
    int blockSize = 0;
    for(int i = 0; i < lsFileCount; i++)
    {
        if(!isAll)
        {
            if(fileNames[i][0] == '.')
                continue;
        }
        struct stat fileStat;
        path filePath = (path) calloc(1024, sizeof(char));
        sprintf(filePath, "%s/%s", dirPath, fileNames[i]);
        stat(filePath, &fileStat);
        blockSize += fileStat.st_blocks;
    }

    printf("total %d\n", blockSize);

    for(int i = 0; i < lsFileCount; i++)
    {
        if(!isAll)
        {
            if(fileNames[i][0] == '.')
                continue;
        }
        path absPath = (path) calloc(1024, sizeof(char));
        sprintf(absPath, "%s/%s", dirPath, fileNames[i]);

        char* permissions = (char*) calloc(10, sizeof(char));
        int hardLinks;
        char* owner = (char*) calloc(30, sizeof(char));
        char* group = (char*) calloc(30, sizeof(char));
        char* size = (char*) calloc(10, sizeof(char));
        char* date = (char*) calloc(30, sizeof(char));
        
        permissions = getFilePermissions(absPath);
        
        struct stat fileStat;
        stat(absPath, &fileStat);
        
        hardLinks = fileStat.st_nlink;

        struct passwd *pw = getpwuid(fileStat.st_uid);
        struct group *gr = getgrgid(fileStat.st_gid);

        time_t rawtime = fileStat.st_mtime;
        struct tm * timeinfo = localtime(&rawtime);
        strftime(date, 30, "%b %d %H:%M", timeinfo);

        sprintf(owner, "%s", pw->pw_name);
        sprintf(group, "%s", gr->gr_name);
        sprintf(size, "%lld", fileStat.st_size);

        char* outputString =  (char*) calloc(1024, sizeof(char));
        sprintf(outputString, "%s %3d %-10s %-5s %8s %-10s ", permissions, hardLinks, owner, group, size, date);

        printf("%s", outputString);

        if(areDirs[i] == 1)
        {
            printf("\033[1;34m");
            printf("%-10s\n", fileNames[i]);
            printf("\033[0m");
        }
        else if(areDirs[i] == -1)
        {
            printf("\033[1;31m");
            printf("%-10s\n", fileNames[i]);
            printf("\033[0m");
        }
        else
        {
            printf("%-10s\n", fileNames[i]);
        }
    }
}

char* getFilePermissions(path filePath)
{
    char* permissions = (char*) calloc(10, sizeof(char));
    struct stat fileStat;

    stat(filePath, &fileStat);

    if(S_ISDIR(fileStat.st_mode))
    {
        permissions[0] = 'd';
    }
    else if(S_ISBLK(fileStat.st_mode))
    {
        permissions[0] = 'b';
    }
    else if(S_ISCHR(fileStat.st_mode))
    {
        permissions[0] = 'c';
    }
    else if(S_ISFIFO(fileStat.st_mode))
    {
        permissions[0] = 'p';
    }
    else if(S_ISLNK(fileStat.st_mode))
    {
        permissions[0] = 'l';
    }
    else if(S_ISSOCK(fileStat.st_mode))
    {
        permissions[0] = 's';
    }
    else
    {
        permissions[0] = '-';
    }
    permissions[1] = fileStat.st_mode & S_IRUSR ? 'r' : '-';
    permissions[2] = fileStat.st_mode & S_IWUSR ? 'w' : '-';
    permissions[3] = fileStat.st_mode & S_IXUSR ? 'x' : '-';
    permissions[4] = fileStat.st_mode & S_IRGRP ? 'r' : '-';
    permissions[5] = fileStat.st_mode & S_IWGRP ? 'w' : '-';
    permissions[6] = fileStat.st_mode & S_IXGRP ? 'x' : '-';
    permissions[7] = fileStat.st_mode & S_IROTH ? 'r' : '-';
    permissions[8] = fileStat.st_mode & S_IWOTH ? 'w' : '-';
    permissions[9] = fileStat.st_mode & S_IXOTH ? 'x' : '-';

    return permissions;
}

void handleFiles(char* file)
{
    int isFile = 0;

    initMyDir(".");

    for(int i = 0; i < lsFileCount; i++)
    {
        if(!strcmp(file, fileNames[i]))
        {
            isFile = 1;
            break;
        }
    }
    
    if(isFile)
    {
        struct stat fileStat;
        stat(file, &fileStat);

        if((fileStat.st_mode & S_IXUSR) || (fileStat.st_mode & S_IXGRP) || (fileStat.st_mode & S_IXOTH))
        {
            printf("\033[1;31m");
            printf("%s\n", file);
            printf("\033[0m");
            return;
        }
        printf("%s\n", file);
        return;
    }

    char errorString[100];
    sprintf(errorString, "ls: %s", file);
    perror(errorString);
}