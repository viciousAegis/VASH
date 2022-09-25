#ifndef __LS_H
#define __LS_H

fileName* fileNames;
int lsFileCount;
int* areDirs;

int compare(const void*, const void*);

void parseFlagsAndDirsInInput();

void printDir();

void printAllDir();

void printDirWithInfo();

char* getFilePermissions();

int initMyDir();

void performLS();

void initFileNames();

void handleFiles(char*);

#endif