//============================================================================
// Name        : LogManager.h
// Author      : Christian Schoppmeyer
// Version     :
// Created On  : 22.11.2011
// Copyright   : EMBOCON Copyright
// Description :
//============================================================================

#ifndef LOGMANAGER_H_
#define LOGMANAGER_H_

#include <stdlib.h>
#include <stdint.h>

using namespace std;

/**
 * Open a new log file for writing the results of the simulation/optimization run into the file.
 */
void openLogFile(char* path);
void openCPUTimeMeasLogFile(char* path);

/**
 * Close the open log file.
 */
void closeLogFile();
void closeCPUTimeMeasLogFile();

/**
 * Write the command line arguments used to call GEMS to the log file
 */
void writeCMDLineArgsToLog(char* argv[]);

/**
 * Write a CPU time measurement of a function call to the CPU time measurement log
 */
void writeCPUTimeMeasToLog(char* msg, uint64_t timeElapsed);

/**
 * Write test message in log file with comment sign (#) ahead.
 */
void writeLineToLog(char* msg);

/**
 * Write text to log file.
 */
void writeTextToLog(char* msg);

/**
 * Write state, output, input and parameter names to log file.
 */
void writeNameToLog(char* name);

/**
 * Write step to log file
 */
void writeStepToLog(double step, double xcur[], unsigned int countState, double ycur[], unsigned int countOutput, double ucur[], unsigned int countInput, double pcur[], unsigned int countParam);

/**
 * Write the general information about the optimization to the header of the log file
 */
void writeGeneralInfoToLog(unsigned int countState, unsigned int countOutput, unsigned int countInput, unsigned int countParam, unsigned int countOptParam, unsigned int countObsParam, double numberOfsteps);

/**
 * Round a double to the given number of digits
 */
double round(double x, int digits);

#endif /* LOGMANAGER_H_ */
