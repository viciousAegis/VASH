#include "headers.h"

input allArgs;
input backArgs;
input foreArgs;

input* backArgsArr;
input* foreArgsArr;

int backArgsCount;
int foreArgsCount;

isBackgroundProcess = 0;

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
    isBackgroundProcess = 0;
    backArgsCount = 0;
    foreArgsCount = 0;
    // concatenate all arguments with command word
    parseSystemInput();

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
        // parent process
        // wait for child to finish
        waitpid(pid, NULL, 0);
        time_t endTime = time(NULL);
        timeElapsedSinceChildStart = endTime - startTime;
    }
}

void execBackgroundProcess()
{
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
        LL_add(backgroundPIDs, 0, pid);
        printf("[%d]\n", pid);
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