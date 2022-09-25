#ifndef __SYSCMD_H
#define __SYSCMD_H

int processCount;

int isBackgroundProcess;

int childCaught;

time_t startTime;

input cmdInputName;

void parseSystemInput();

void checkIfBackgroundProcess();

void testSystemCommand();

void execBackgroundProcess();

void execForegroundProcess();

void waitForBackgroundChild();

char* getStatus(char*);

void fg();

void bg();

#endif