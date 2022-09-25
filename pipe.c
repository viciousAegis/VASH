#include "headers.h"

char* token;
char* pipeToken;

int checkPipe(input command)
{
    if(strchr(command, '|'))
    {
        pipeCount = 0;
        return 1;
    }
    return 0;
}

void performPipe(input command)
{
    //duplicate the command
    input cmd = (input) calloc(strlen(command), sizeof(char));
    strcpy(cmd, command);

    token = strtok(command, " ");
    if(!strcmp(token, "|"))
    {
        printErrorMsg("Error: Pipe cannot read from NULL command\n");
        return;
    }

    while(token != NULL)
    {
        if(!strcmp(token, "|"))
        {
            pipeCount++;
            token = strtok(NULL, " ");
            if(token == NULL)
            {
                printErrorMsg("Error: Pipe cannot write to NULL command\n");
                return;
            }
        }

        token = strtok(NULL, " ");
    }

    //restore command
    strcpy(command, cmd);

    pipeToken = strtok(command, "|");
    int inputFile = STDIN_FILENO;

    pid_t pid;

    for(int i = 0; i <= pipeCount; i++)
    {
        int fd[2];
        pipe(fd);
        inPipe = 0;

        strcpy(bufferInput, pipeToken);
        pipeToken = strtok(NULL, "|");

        pid = fork();
        if(pid < 0)
        {
            printErrorMsg("Error: Fork failed\n");
            return;
        }

        if(pid == 0)
        {
            dup2(inputFile, STDIN_FILENO);

            if(i != pipeCount)
            {
                inPipe = 1;
                dup2(fd[1], STDOUT_FILENO);
            }
            handleInput();
            exit(0);
        }
        else
        {
            waitpid(pid, NULL, 0);
            close(fd[1]);
            inputFile = fd[0];
        }

    }
}