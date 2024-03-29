#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__

int getCommands();

// parse input to get command word and arguments
int parseInput();

void handleInput();

void cmdNotFound();

int checkExit();

int checkEcho();

int checkPWD();

int checkCD();

int checkLS();

int checkClear();

int checkHistory();

int checkDiscover();

char* removeTrailingEscape(path);

char* removeLeadingTilde(path);

void resetRedirection();

int handleRedirection();

void takeInputFromFile();

void redirectOutputToFile();

void clearArguments();

char* inputFile;
char* outputFile;

input* commands;
int cmdCount;

FILE* inputFD;
FILE* outputFD;

input* redirections;

int isInputRedirected;
int isOutputRedirected;
int isOutputAppended;

int originalSTDOUT;
int originalSTDIN;

#endif