#include "headers.h"

void catchSignals()
{
    signal(SIGINT, sigIntHandler);
    signal(SIGTSTP, sigTstpHandler);
    signal(SIGCHLD, waitForBackgroundChild);
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
        LL_add(backgroundPIDs, 0, foregroundPID);
        kill(foregroundPID, SIGTSTP);
        foregroundPID = 0;
        return;
    }
    write(1, "\n", 1);
    prompt(currDirectory);
    fflush(stdout);
}