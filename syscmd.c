#include "headers.h"

input allArgs;
input backArgs;
input foreArgs;

input* backArgsArr;
input* foreArgsArr;

int backArgsCount;
int foreArgsCount;

int isBackgroundProcess = 0;

void parseSystemInput()
{
    allArgs = (input) calloc(1024, sizeof(char));
    sprintf(allArgs,"%s", commandWord);

    for(int i = 0; i < argCount; i++)
    {
        sprintf(allArgs, "%s %s", allArgs, arguments[i]);
    }

    checkIfBackgroundProcess();

    if(isBackgroundProcess)
    {
        backArgs = strtok(allArgs, "&");
        foreArgs = strtok(NULL, "&");
    }
    else
    {
        foreArgs = allArgs;
    }

    if(isBackgroundProcess)
    {
        backArgsArr = (input*) calloc(1024, sizeof(input));
        for(char* word = strtok(backArgs, " \t"); word; word = strtok(NULL, " \t"))
        {
            backArgsArr[backArgsCount] = (input) calloc(strlen(word), sizeof(char));
            strcpy(backArgsArr[backArgsCount++], word);
        }
        backArgs[backArgsCount] = NULL;

        if(foreArgs != NULL)
        {
            foreArgsArr = (input*) calloc(1024, sizeof(input));
            for(char* word = strtok(foreArgs, " \t"); word; word = strtok(NULL, " \t"))
            {
                foreArgsArr[foreArgsCount] = (input) calloc(strlen(word), sizeof(char));
                strcpy(foreArgsArr[foreArgsCount++], word);
            }
            foreArgs[foreArgsCount] = NULL;
        }
    }
    else
    {
        foreArgsArr = (input*) calloc(1024, sizeof(input));
        for(char* word = strtok(foreArgs, " \t"); word; word = strtok(NULL, " \t"))
        {
            foreArgsArr[foreArgsCount] = (input) calloc(strlen(word), sizeof(char));
            strcpy(foreArgsArr[foreArgsCount++], word);
        }
        foreArgs[foreArgsCount] = NULL;
    }
}

void checkIfBackgroundProcess()
{
    if(strchr(allArgs, '&'))
    {
        isBackgroundProcess = 1;
    }
}

void testSystemCommand()
{
    foregroundPID = 0;
    isBackgroundProcess = 0;
    backArgsCount = 0;
    foreArgsCount = 0;
    // concatenate all arguments with command word
    parseSystemInput();

    cmdInputName = (input) calloc(1024, sizeof(char));

    if(isBackgroundProcess)
    {
        execBackgroundProcess();
        if(foreArgsCount > 0)
        {
            execForegroundProcess();
        }
    }
    else
    {
        execForegroundProcess();
    }

}

void execForegroundProcess()
{
    for(int i = 0; i < foreArgsCount; i++)
    {
        sprintf(cmdInputName, "%s %s", cmdInputName, foreArgsArr[i]);
    }

    startTime = time(NULL);
    //fork a child process
    pid_t pid = fork();;

    int flag;

    if(pid < 0)
    {
        // fork failed
        perror("");
        return;
    }
    
    if(pid == 0)
    {
        // child process
        //execute command and check for errors
        flag = execvp(foreArgsArr[0], foreArgsArr);
        if(flag < 0)
        {
            printErrorMsg("command not found\n");
            // kill the child process if unsuccessful execution
            exit(1);
        }
    }

    if(pid > 0)
    {
        foregroundPID = pid;
        // parent process
        // wait for child to finish
        waitpid(pid, NULL, WUNTRACED);
        time_t endTime = time(NULL);
        timeElapsedSinceChildStart = endTime - startTime;
    }
}

void execBackgroundProcess()
{
    for(int i = 0; i < backArgsCount; i++)
    {
        sprintf(cmdInputName, "%s %s", cmdInputName, backArgsArr[i]);
    }

    //fork a child process
    time_t startTime = time(NULL);
    pid_t pid = fork();;

    int flag;

    if(pid < 0)
    {
        // fork failed
        perror("");
        return;
    }
    
    if(pid == 0)
    {
        // child process
        //execute command and check for errors
        setpgid(0, 0);
        flag = execvp(backArgsArr[0], backArgsArr);
        if(flag < 0)
        {
            printErrorMsg("\ncommand not found\n");
            // kill the child process if unsuccessful execution
            exit(1);
        }
    }

    if(pid > 0)
    {
        DataType* process = initDataType(pid, cmdInputName, ++processCount);
        LL_add(backgroundPIDs, 0, process);
        printf("[%d] %d\n", processCount, pid);
        // parent process
        // do not wait for child to finish
    }
}

void waitForBackgroundChild()
{
    int wstat;
    pid_t pid;

    while((pid = waitpid(-1,&wstat,WNOHANG)) > 0)
    {
        if(LL_empty(backgroundPIDs))
        {
            return;
        }
        if(LL_search(backgroundPIDs, pid) == 1)
        {
            LL_delete(backgroundPIDs, pid);
        
            char* outMsg = (char*) calloc(1024, sizeof(char));
            if(WIFEXITED(wstat) == 1)
            {
                sprintf(outMsg,"\n%s with pid: %d exited normally\n",backArgsArr[0], pid);
                write(1, outMsg, strlen(outMsg));
            }
            else
            {
                sprintf(outMsg,"\n%s with pid: %d exited abnormally\n",backArgsArr[0], pid);
                printErrorMsg(outMsg);
                perror(errno);
            }
            prompt(currDirectory);
            fflush(stdout);
        }
    }
}

void printJobs(LL* l)
{
    int runFlag = 0;
    int stopFlag = 0;

    for(int i = 0; i < argCount; i++)
    {
        if(strchr(arguments[i], 'r'))
        {
            runFlag = 1;
        }
        if(strchr(arguments[i], 's'))
        {
            stopFlag = 1;
        }
    }

    DataType* jobsArray = (DataType*) calloc(l->size ,sizeof(DataType));

    LL_Node* curr = l->head;
    int i = 0;
    while(curr != NULL)
    {
        jobsArray[i++] = *curr->data;
        curr = curr->next;
    }

    qsort(jobsArray, l->size, sizeof(DataType), compareJobs);

    if(runFlag + stopFlag == 0 || runFlag + stopFlag == 2)
    {
        for(int i = 0; i < l->size; i++)
        {
            char* status = getStatus(getProcessStatus(jobsArray[i].pid));
            if(status == NULL) continue;
            printf("[%d] %s %s - %d\n", jobsArray[i].processNumber, status, jobsArray[i].name, jobsArray[i].pid);
        }
    }

    if(runFlag)
    {
        for(int i = 0; i < l->size; i++)
        {
            char* status = getStatus(getProcessStatus(jobsArray[i].pid));
            if(status == NULL) continue;
            if(strcmp(status, "Running") == 0)
            {
                printf("[%d] %s %s - %d\n", jobsArray[i].processNumber, status, jobsArray[i].name, jobsArray[i].pid);
            }
        }
    }
    
    if(stopFlag)
    {
        for(int i = 0; i < l->size; i++)
        {
            char* status = getStatus(getProcessStatus(jobsArray[i].pid));
            if(status == NULL) continue;
            if(strcmp(status, "Stopped") == 0)
            {
                printf("[%d] %s %s - %d\n", jobsArray[i].processNumber, status, jobsArray[i].name, jobsArray[i].pid);
            }
        }
    }
    
}

char* getStatus(char* statusSymbol)
{
    if(statusSymbol == NULL)
    {
        return NULL;
    }

    char symbol = statusSymbol[0];

    char* status = (char*) calloc(1024, sizeof(char));

    switch(symbol)
    {
        case 'R':
            sprintf(status, "Running");
            break;
        default:
            sprintf(status, "Stopped");
            break;
    }

    return status;
}

int compareJobs(const void* a, const void* b)
{
    DataType* jobA = (DataType*) a;
    DataType* jobB = (DataType*) b;

    return strcmp(jobA->name, jobB->name);
}

void bg()
{
    if(argCount != 1)
    {
        printErrorMsg("bg: invalid number of arguments\n");
        return;
    }

    int jobNumber = atoi(arguments[0]);

    if(jobNumber == 0)
    {
        printErrorMsg("bg: invalid job number\n");
        return;
    }

    int jobPid = LL_search_processNo(backgroundPIDs, jobNumber);

    if(jobPid == 0)
    { 
        printErrorMsg("bg: invalid job number\n");
        return;
    }

    kill(jobPid, SIGCONT);
}

void fg()
{
    if(argCount != 1)
    {
        printErrorMsg("fg: invalid number of arguments\n");
        return;
    }

    int jobNumber = atoi(arguments[0]);

    if(jobNumber == 0)
    {
        printErrorMsg("fg: invalid job number\n");
        return;
    }

    int jobPid = LL_search_processNo(backgroundPIDs, jobNumber);

    if(jobPid == 0)
    { 
        printErrorMsg("fg: invalid job number\n");
        return;
    }

    kill(jobPid, SIGCONT);

    signal(SIGTTOU, SIG_IGN), signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, jobPid);

    int wstat;
    waitpid(jobPid, &wstat, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpid());
    signal(SIGTTOU, SIG_DFL), signal(SIGTTOU, SIG_DFL);

    if(!WIFSTOPPED(wstat))
    {
        LL_delete(backgroundPIDs, jobPid);
    }
    else
    {
        printErrorMsg("fg: process stopped\n");
    }

    if(WTERMSIG(wstat) || WEXITSTATUS(wstat))
    {
        printErrorMsg("fg: process exited abnormally\n");
    }
}