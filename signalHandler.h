#ifndef __SIGNALHANDLER_H
#define __SIGNALHANDLER_H

void catchSignals();

void sigIntHandler();

void sigTstpHandler();

int foregroundPID;

#endif