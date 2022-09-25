#include "headers.h"

int pid;
char* status;
unsigned long memory;
char* exePath;
int pgid;
int pgrid;
int isBackground;

void performPinfo()
{
    if(argCount == 0)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(arguments[0]);
    }

    path procFilePath = (path) calloc(1024, sizeof(char));
    sprintf(procFilePath, "/proc/%d/stat", pid);

    FILE* file = fopen(procFilePath, "r");
    if(file == NULL)
    {
        printErrorMsg("Error: No such file\n");
        return;
    }
    
    char* statContents = (char*) calloc(1024, sizeof(char));
    char* actualStatContents = (char*) calloc(1024, sizeof(char));

    fgets(statContents,1024,file);

    strtok(statContents,")");
    actualStatContents = strtok(NULL, ")");

    int statCount = 0;

    status = (char*) calloc(32, sizeof(char));

    for(char* out = strtok(actualStatContents, " "); out; out = strtok(NULL, " "))
    {
        if(statCount == 0)
            status = out;
        else if(statCount == 2)
            pgid = atoi(out);
        else if(statCount == 5)
            pgrid = atoi(out);
        else if(statCount == 20)
            memory = atol(out);
        statCount++;
    }

    isBackground = pgrid - pgid;

    if(isBackground == 0)
    {
        strcat(status, "+");
    } //not background

    fclose(file);

    sprintf(procFilePath, "/proc/%d/exe", pid);
    FILE* file2 = fopen(procFilePath, "r");
    if(file2 == NULL)
    {
        printErrorMsg("Error: No such file\n");
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
    printf("Memory : %lu\n", memory);
    printf("Executable Path : %s\n", exePath);
}

char* getProcessStatus(int givenPid)
{
    path procFilePath = (path) calloc(1024, sizeof(char));
    sprintf(procFilePath, "/proc/%d/stat", givenPid);

    FILE* file = fopen(procFilePath, "r");
    if(file == NULL)
    {
        printErrorMsg("Error: No such file\n");
        return NULL;
    }
    
    char* statContents = (char*) calloc(1024, sizeof(char));
    char* actualStatContents = (char*) calloc(1024, sizeof(char));

    fgets(statContents,1024,file);

    strtok(statContents,")");
    actualStatContents = strtok(NULL, ")");

    int statCount = 0;

    status = (char*) calloc(32, sizeof(char));

    for(char* out = strtok(actualStatContents, " "); out; out = strtok(NULL, " "))
    {
        if(statCount == 0)
            status = out;
        break;
    }

    fclose(file);

    return status;
}