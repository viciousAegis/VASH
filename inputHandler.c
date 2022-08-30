#include "headers.h"

void parseInput()
{
    commandWord = strtok(bufferInput, " \t");

    arguments = (input) calloc(1024, sizeof(char));
    for(char* word = strtok(NULL, ";"); word; word = strtok(NULL, ";"))
    {
        arguments = strcat(arguments, word);
    }
}

void handleInput()
{
    parseInput();

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

}

int checkExit()
{
    if(!strcmp(commandWord,"exit") || !strcmp(commandWord,"exit\n"))
    {
        return 1;
    }
    return 0;
}

int checkEcho()
{
    if(!strcmp(commandWord,"echo") || !strcmp(commandWord,"echo\n"))
    {
        return 1;
    }
    return 0;
}

int checkPWD()
{
    if(!strcmp(commandWord,"pwd") || !strcmp(commandWord,"pwd\n"))
    {
        return 1;
    }
    return 0;
}

int checkCD()
{
    if(!strcmp(commandWord,"cd") || !strcmp(commandWord,"cd\n"))
    {
        return 1;
    }
    return 0;
}