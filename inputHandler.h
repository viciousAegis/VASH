#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__

int getCommands();

// parse input to get command word and arguments
void parseInput();

void handleInput();

void cmdNotFound();

int checkExit();

int checkEcho();

int checkPWD();

int checkCD();

int checkClear();

char* removeTrailingEscape(path);

char* removeLeadingTilde(path);

#endif