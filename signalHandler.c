#include "headers.h"

void catchSignals()
{
    signal(SIGINT, sigIntHandler);
    signal(SIGTSTP, sigTstpHandler);
    signal(SIGCHLD, waitForBackgroundChild);
}

void sendSignalToProcess()
{
    int procNum = atoi(arguments[0]);
    int sigNum = atoi(arguments[1]);

    int pid = LL_search_processNo(backgroundPIDs, procNum);

    if(pid > 0)
    {
        kill(pid, sigNum);
    }
    else
    {
        printf("No such process exists\n");
    }
}

void sigIntHandler()
{
    if(foregroundPID > 0)
    {
        kill(foregroundPID, SIGINT);
        foregroundPID = 0;
        return;
    }
    write(1, "\n", 1);
    prompt(currDirectory);
    fflush(stdout);
}

void sigTstpHandler()
{
    if(foregroundPID > 0)
    {
        DataType* process = initDataType(foregroundPID, cmdInputName, ++processCount);
        LL_add(backgroundPIDs, 0, process);
        kill(foregroundPID, SIGTSTP);
        foregroundPID = 0;
        return;
    }

    write(1, "\n", 1);
    prompt(currDirectory);
    fflush(stdout);
}