//============================================================================
// Name        : LogManager.cpp
// Author      : Christian Schoppmeyer
// Version     :
// Created On  : 22.11.2011
// Copyright   : EMBOCON Copyright
// Description :
//============================================================================

#include "LogManager.h"
#include "Messages.h"
#include <fstream>
#include <math.h>

using namespace std;

fstream logfile;
fstream measLog;

double round(double x, int digits)
{
	return floor(x * pow(10, digits) + 0.5) / pow(10,digits);
}
void openLogFile(char* path)
{
	logfile.open(path, ios::out);
	logfile << "#GEMS ";
	putVersionInfo(logfile);
	logfile << " (" __DATE__ ") - Generic EMBOCON Minimal Supervisor" << endl;
}
void openCPUTimeMeasLogFile(char* path)
{
	measLog.open(path, ios::out);
	measLog << "#GEMS ";
	putVersionInfo(measLog);
	measLog << " (" __DATE__ ") - Generic EMBOCON Minimal Supervisor" << endl;
}

void writeCMDLineArgsToLog(char* argv[])
{
	logfile << endl;
	logfile << "# Arguments:";
	for(unsigned int i = 0; i < sizeof(argv); i++)
	{
		logfile << " " << argv[i];
	}
	logfile << endl;

	logfile.flush();
}

void closeLogFile()
{
	logfile.flush();
	logfile.close();
}
void closeCPUTimeMeasLogFile()
{
	measLog.flush();
	measLog.close();
}

void writeCPUTimeMeasToLog(char* msg, uint64_t timeElapsed)
{
	measLog  << msg << "\t" << "with measure CPU time:" << "\t" << timeElapsed << "\t" << "ns" << endl;
}

void writeLineToLog(char* msg)
{
	logfile << "#" << msg << endl;
}

void writeTextToLog(char* msg)
{
	logfile << msg;
}

void writeNameToLog(char* name)
{
	logfile << name;
	logfile << "\t";
}

void writeGeneralInfoToLog(unsigned int countState, unsigned int countOutput, unsigned int countInput, unsigned int countParam, unsigned int countOptParam, unsigned int countObsParam, double numberOfsteps)
{
	logfile << endl;
	logfile << "# GENERAL STATISTICS:" << endl;
	logfile << "# number of states: " << countState << endl;
	logfile << "# number of outputs: " << countOutput << endl;
	logfile << "# number of inputs: " << countInput << endl;
	logfile << "# number of parameters: " << countParam << endl;
	logfile << "# number of optimizer parameters: " << countOptParam << endl;
	logfile << "# number of observer parameters: " << countObsParam << endl;
	logfile << "# number of steps: " << numberOfsteps << endl;
	logfile << endl;

	logfile.flush();
}

void writeStepToLog(double step, double xcur[], unsigned int countState, double ycur[], unsigned int countOutput, double ucur[], unsigned int countInput, double pcur[], unsigned int countParam)
{
	logfile << step << "\t";
	for(unsigned int i = 0; i < countState; i++)
	{
		logfile << round(xcur[i],5) << "\t";
	}
	for(unsigned int i = 0; i < countOutput; i++)
	{
		logfile << round(ycur[i],5) << "\t";
	}
	for(unsigned int i = 0; i < countInput; i++)
	{
		logfile << round(ucur[i],5) << "\t";
	}
	for(unsigned int i = 0; i < countParam; i++)
	{
		logfile << round(pcur[i],5) << "\t";
	}
	logfile << endl;
	logfile.flush();
}


