#include "headers.h"

int pid;
char* status;
int memory;
char* exePath;
int isBackground;

void performPinfo()
{
    if(argCount == 0)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(arguments[1]);
    }
    
    path procFilePath = (path) calloc(1024, sizeof(char));
    sprintf(procFilePath, "/proc/%d/stat", pid);
    
    FILE* file = fopen(procFilePath, "r");
    if(file == NULL)
    {
        printErrorMsg("Error: No such process\n");
        return;
    }
    
    char** statContents = (char**) calloc(1024, sizeof(char*));
    statContents[0] = (char*) calloc(1024, sizeof(char));
    while(fscanf(file, "%s", statContents[0]) != EOF)
    {
        for(int i = 1; i < 1024; i++)
        {
            statContents[i] = (char*) calloc(1024, sizeof(char));
            fscanf(file, "%s", statContents[i]);
        }
    }

    char* status = (char*) calloc(1024, sizeof(char));
    status = statContents[2];
    memory = atoi(statContents[22]);

    isBackground = atoi(statContents[4]) - atoi(statContents[7]);

    if(isBackground == 0)
    {
        strcat(status, "+");
    } //not background

    fclose(file);

    sprintf(procFilePath, "/proc/%d/exe", pid);
    FILE* file2 = fopen(procFilePath, "r");
    if(file2 == NULL)
    {
        printErrorMsg("Error: No such process\n");
        return;
    }

    exePath = (char*) calloc(1024, sizeof(char));
    
    readlink(procFilePath, exePath, 1024);

    fclose(file2);

    printPinfo();
}

void printPinfo()
{
    printf("pid : %d\n", pid);
    printf("Process Status : %s\n", status);
    printf("Memory : %d\n", memory);
    printf("Executable Path : %s\n", exePath);
}