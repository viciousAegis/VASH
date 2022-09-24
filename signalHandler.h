#ifndef __SIGNALHANDLER_H
#define __SIGNALHANDLER_H

void catchSignals();

void sigIntHandler();

void sigTstpHandler();

void sendSignalToProcess();

int foregroundPID;

#endif