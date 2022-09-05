#ifndef __DISCOVER_H
#define __DISCOVER_H

#define MAX_FILES 100

void performDiscover();

void parseFilesAndFlagsInInput();

int findFileInDir(char*);

void displayAllFilesInDir(char*);

void displayFilesInDir(char*);

void displayDirsInDir(char*);

char* cleanFileNameString(char*);

#endif