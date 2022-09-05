#include "headers.h"

input* commands;
int cmdCount;

int getCommands()
{
    // handle pressing enter
    if(!strcmp(bufferInput, "\n"))
        return 0;

    commands = (input*) calloc(100, sizeof(input));
    cmdCount = 0;
    for(char* cmd = strtok(bufferInput, ";"); cmd; cmd = strtok(NULL, ";"))
    {
        commands[cmdCount] = (input) calloc(strlen(cmd), sizeof(char));
        strcpy(commands[cmdCount++], cmd);
    }

    return 1;
}

void parseInput(input command)
{
    command = removeTrailingEscape(command);
    
    commandWord = strtok(command, " \t");

    arguments = (input*) calloc(1024, sizeof(input));
    argCount = 0;
    
    for(char* word = strtok(NULL, " \t"); word; word = strtok(NULL, " \t"))
    {
        arguments[argCount] = (input) calloc(strlen(word), sizeof(char));
        strcpy(arguments[argCount++], word);
    }
    if(argCount == 0)
    {
        arguments[argCount] = (input) calloc(1, sizeof(char));
        strcpy(arguments[argCount], "");
    }
}

void handleInput()
{
    int flag = getCommands();

    if(!flag)
        return;

    for(int i = 0; i < cmdCount; i++)
    {
        parseInput(commands[i]);

        addToHistory();

        if(checkExit())
        {
            performExit();
        }

        if(checkEcho())
        {
            performEcho();
        }
        else if(checkPWD())
        {
            performPWD();
        }
        else if(checkCD())
        {
            performCD();
        }
        else if(checkClear())
        {
            clearDisplay();
        }
        else if(checkLS())
        {
            performLS();
        }
        else if(checkHistory())
        {
            printHistory();
        }
        else if(checkDiscover())
        {
            performDiscover();
        }
        else
        {
            testSystemCommand();
        }
    }
}

int checkExit()
{
    if(!strcmp(commandWord,"exit") || !strcmp(commandWord, "quit") || !strcmp(commandWord, "q"))
    {
        return 1;
    }
    return 0;
}

int checkEcho()
{
    if(!strcmp(commandWord,"echo"))
    {
        return 1;
    }
    return 0;
}

int checkPWD()
{
    if(!strcmp(commandWord,"pwd"))
    {
        return 1;
    }
    return 0;
}

int checkCD()
{
    if(!strcmp(commandWord,"cd"))
    {
        return 1;
    }
    return 0;
}

int checkClear()
{
    if(!strcmp(commandWord,"clear") || !strcmp(commandWord,"c"))
    {
        return 1;
    }
    return 0;
}

int checkLS()
{
    if(!strcmp(commandWord,"ls"))
    {
        return 1;
    }
    return 0;
}

int checkHistory()
{
    if(!strcmp(commandWord,"history") || !strcmp(commandWord,"h"))
    {
        return 1;
    }
    return 0;
}

int checkDiscover()
{
    if(!strcmp(commandWord,"discover") || !strcmp(commandWord,"d"))
    {
        return 1;
    }
    return 0;
}

char* removeTrailingEscape(char* path)
{
    int len = strlen(path);
    if(path[len - 1] == '\n')
    {
        path[len - 1] = '\0';
    }
    return path;
}

void cmdNotFound()
{
    printf("%s: Command not found: %s\n", vash, commandWord);
}

