#include "headers.h"

/*
TODO
do time of process
background process
*/

void testSystemCommand()
{

    // concatenate all arguments with command word
    input* allArgs = (input*) calloc(1024, sizeof(input));
    allArgs[0] = (input) calloc(1024, sizeof(char));
    strcpy(allArgs[0], commandWord);
    for(int i = 0; i < argCount; i++)
    {
        allArgs[i+1] = (input) calloc(1024, sizeof(char));
        strcpy(allArgs[i+1], arguments[i]);
    }
    allArgs[argCount+1] = NULL;

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
        flag = execvp(allArgs[0], allArgs);
        if(flag < 0)
        {
            write(2,"command not found\n",20);
            // kill the child process if unsuccessful execution
            kill(getpid(), SIGKILL);
        }
    }

    if(pid > 0)
    {
        // parent process
        // wait for child to finish
        waitpid(pid, NULL, 0);
    }
}