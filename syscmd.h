#ifndef __SYSCMD_H
#define __SYSCMD_H

int isBackgroundProcess;

int childCaught;

void parseSystemInput();

void checkIfBackgroundProcess();

void testSystemCommand();

void execBackgroundProcess();

void execForegroundProcess();

void waitForBackgroundChild();

#endif