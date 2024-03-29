#include "headers.h"

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

int parseInput(input command)
{
    command = removeTrailingEscape(command);
    commandWord = strtok(command, " \t");
    if((int) commandWord == 0)
    {
        return -1;
    }

    arguments = (input*) calloc(1024, sizeof(input));
    argCount = 0;

    redirections = (input*) calloc(2, sizeof(input));

    for(char* word = strtok(NULL, " \t"); word; word = strtok(NULL, " \t"))
    {


        // arguments[argCount] = (input) calloc(strlen(word), sizeof(char));
        // strcpy(arguments[argCount++], word);

        if(!strcmp(word, "<"))
        {
            arguments[argCount] = (input) calloc(strlen(word), sizeof(char));
            strcpy(arguments[argCount++], word);
            
            arguments[argCount] = (input) calloc(strlen(word), sizeof(char));

            redirections[0] = word;
            isInputRedirected = 1;
            strcpy(inputFile, strtok(NULL, " \t"));
            strcpy(arguments[argCount++], inputFile);
            redirections[1] = inputFile;
        }
        else if(!strcmp(word, ">"))
        {
            // arguments[argCount] = (input) calloc(strlen(word), sizeof(char));
            
            redirections[0] = word;
            isOutputRedirected = 1;
            strcpy(outputFile, strtok(NULL, " \t"));
            // strcpy(arguments[argCount++], outputFile);
            redirections[1] = outputFile;
        }
        else if(!strcmp(word, ">>"))
        {
            // arguments[argCount] = (input) calloc(strlen(word), sizeof(char));

            redirections[0] = word;
            isOutputAppended = 1;
            strcpy(outputFile, strtok(NULL, " \t"));
            // strcpy(arguments[argCount++], outputFile);
            redirections[1] = outputFile;
        }
        else
        {
            arguments[argCount] = (input) calloc(strlen(word), sizeof(char));
            strcpy(arguments[argCount++], word);
        }
    }

    if(argCount == 0)
    {
        arguments[argCount] = (input) calloc(1, sizeof(char));
        strcpy(arguments[argCount], "");
    }

    return 0;
}

void handleInput()
{
    int flag = getCommands();

    if(!flag)
        return;

    for(int i = 0; i < cmdCount; i++)
    {
        if(checkPipe(commands[i]))
        {
            performPipe(commands[i]);
            continue;
        }

        int inputFlag = parseInput(commands[i]);
        if(inputFlag == -1)
        {
            continue;
        }
        addToHistory();

        int rdFlag = handleRedirection();
        if(rdFlag == -1)
        {
            continue;
        }

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
        else if(!strcmp(commandWord, "jobs"))
        {
            printJobs(backgroundPIDs);
        }
        else if(!strcmp(commandWord, "sig"))
        {
            sendSignalToProcess();
        }
        else if(!strcmp(commandWord,"pinfo"))
        {
            performPinfo();
        }
        else if(!strcmp(commandWord,"bg"))
        {
            bg();
        }
        else if(!strcmp(commandWord,"fg"))
        {
            fg();
        }
        else
        {
            testSystemCommand();
        }
        
        if(!inPipe)
            resetRedirection();
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

void resetRedirection()
{
    if(isOutputAppended || isOutputRedirected)
    {
        dup2(originalSTDOUT, STDOUT_FILENO);
    }

    if(isInputRedirected)
    {
        dup2(originalSTDIN, STDIN_FILENO);
    }

    isInputRedirected = 0;
    isOutputRedirected = 0;
    isOutputAppended = 0;

    inputFile = (input) calloc(1024, sizeof(char));
    outputFile = (input) calloc(1024, sizeof(char));
}

int handleRedirection()
{
    if(isInputRedirected)
    {
        inputFD = open(inputFile, O_RDONLY);

        if(inputFD == -1)
        {
            printErrorMsg("Error: File not found\n");
            return -1;
        }

        takeInputFromFile();
    }

    if(isOutputRedirected)
    { 
        outputFD = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC,  0644);
        if(outputFD == -1)
        {
            printErrorMsg("Error: File not found\n");
            return -1;
        }
        redirectOutputToFile();
    }
    else if(isOutputAppended)
    {
        outputFD = open(outputFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(outputFD == -1)
        {
            printErrorMsg("Error: File not found\n");
            return -1;
        }
        redirectOutputToFile();
    }

    return 0;
}

void takeInputFromFile()
{
    originalSTDIN = dup(STDIN_FILENO);
    if(dup2(inputFD, STDIN_FILENO) < 0)
    {
        printErrorMsg("Error: Could not redirect input\n");
    }
    close(inputFD);

    clearArguments();
}

void redirectOutputToFile()
{
    originalSTDOUT = dup(STDOUT_FILENO);

    if(dup2(outputFD, STDOUT_FILENO) < 0)
    {
        printErrorMsg("Error: File not duplicated\n");
    }
    close(outputFD);
}

void clearArguments()
{
    for(int i = 0; i < argCount; i++)
    {
        free(arguments[i]);
    }
    argCount = 0;
}