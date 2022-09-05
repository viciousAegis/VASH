#ifndef __HISTORY_H
#define __HISTORY_H

#define MAX_LENGTH 1024
#define MAX_COUNT 20
#define MAX_DISPLAY_COUNT 10

int historyCmdCount;

char cmdHistoryArray[MAX_COUNT][MAX_LENGTH];

void addToHistory();

void printHistory();

void loadHistory();

void writeToHistoryFile();

char* constructCommand();

int checkIfExists(char*);

#endif