#include "headers.h"

int min(int a, int b)
{
    return a < b ? a : b;
}

void addToHistory()
{
    historyCmdCount = min(historyCmdCount, MAX_COUNT-1);


    char* cmd = (char*) calloc(256, sizeof(char));
    cmd = constructCommand();

    int flag = checkIfExists(cmd);
    if(flag) return;

    for(int i = historyCmdCount-1; i > -1; i--)
    {
        strcpy(cmdHistoryArray[i+1], cmdHistoryArray[i]);
    }
    strcpy(cmdHistoryArray[0], cmd);
    printf("%d\n", historyCmdCount);

    if(historyCmdCount < MAX_COUNT)
        historyCmdCount++;

    writeToHistoryFile();
}

int checkIfExists(cmdWord cmd)
{
    if(!strcmp(cmdHistoryArray[0], cmd))
        return 1;
    return 0;
}

void printHistory()
{
    int displayCount = min(historyCmdCount, MAX_DISPLAY_COUNT);
    for(int i = displayCount - 1; i >= 0; i--)
    {
        printf("%d: %s\n", displayCount - i, cmdHistoryArray[i]);
    }
}

void loadHistory()
{
    historyCmdCount = 0;

    FILE* fp = fopen("cmd_history.txt", "r");
    if(!fp)
    {
        printf("No history file found\n");
        return;
    }

    char* line = (char*) calloc(MAX_LENGTH, sizeof(char));
    while(fgets(line, MAX_LENGTH, fp) != NULL)
    {
        line = removeTrailingEscape(line);
        strcpy(cmdHistoryArray[historyCmdCount++], line);
    }

    fclose(fp);
}

void writeToHistoryFile()
{
    FILE* fp = fopen("cmd_history.txt", "w");
    if(!fp)
    {
        printf("Error opening history file\n");
        return;
    }

    for(int i = 0; i < historyCmdCount; i++)
    {
        fprintf(fp, "%s\n", cmdHistoryArray[i]);
    }

    fclose(fp);
}

char* constructCommand()
{
    char* cmd = (char*) calloc(256, sizeof(char));
    strcpy(cmd, commandWord);

    for(int i = 0; i < argCount; i++)
    {
        strcat(cmd, " ");
        strcat(cmd, arguments[i]);
    }

    cmd = removeTrailingEscape(cmd);
    return cmd;
}