//============================================================================
// Name        : Gems.cpp
// Author      : Christian Schoppmeyer
// Version     : 0.3
// Created On  : 07.11.2012
// Copyright   : EMBOCON Copyright
// Description : The main supervisor file of GEMS
//============================================================================


//Include library for dynamic library handling
#include <dlfcn.h>
//Include libraries for IO interaction
#include <stdio.h>
#include <iostream>
//Include standard template library
#include <stdlib.h>
//Include string library for (easier) user feedback
#include <string>
#include <sstream>
//Including time library for measuring the function call times
#include <time.h>
#include <stdint.h>
//Including string.h for memcpy
#include <string.h>
//Include EMBOCON interfaces
#include "./Interfaces/EMBOCON_DataTypes.h"
#include "./Interfaces/EMBOCON_ModelInterface.h"
#include "./Interfaces/EMBOCON_OptimizerInterface.h"
#include "./Interfaces/EMBOCON_ObserverInterface.h"
#include "./Interfaces/EMBOCON_SimulationApplicationInterface.h"
//Include of GEMS
#include "Messages.h"
#include "InterfaceConnection.h"
#include "LogManager.h"

using namespace std;

/**
 * Declaration of library symbols
 */
//OPT_Model
emb_model (*optmodel_createModel)();
void (*optmodel_freeModel)(emb_model);
emb_size_type (*optmodel_getParameterCount)(emb_model);
emb_size_type (*optmodel_getStateCount)(emb_model);
emb_size_type (*optmodel_getInputCount)(emb_model);
emb_size_type (*optmodel_getOutputCount)(emb_model);
int (*optmodel_getStateType)(emb_model, int[]);
int (*optmodel_setParameter)(emb_model, double[]);
int (*optmodel_getInitialInputs)(emb_model, double[]);
int (*optmodel_getInitialState)(emb_model, const double[], const double[], double[]);
int (*optmodel_getDerivatives)(emb_model, const double[], const double[], double[], double*);
int (*optmodel_getParameterName)(emb_model, int, char[], size_t, size_t*);
int (*optmodel_getStateName)(emb_model, int, char[], size_t, size_t*);
int (*optmodel_getInputName)(emb_model, int, char[], size_t, size_t*);
int (*optmodel_getOutputName)(emb_model, int, char[], size_t, size_t*);

//OBS_Model
emb_model (*obsmodel_createModel)();
void (*obsmodel_freeModel)(emb_model);
emb_size_type (*obsmodel_getParameterCount)(emb_model);
emb_size_type (*obsmodel_getStateCount)(emb_model);
emb_size_type (*obsmodel_getInputCount)(emb_model);
emb_size_type (*obsmodel_getOutputCount)(emb_model);
int (*obsmodel_getStateType)(emb_model, int[]);
int (*obsmodel_setParameter)(emb_model, double[]);
int (*obsmodel_getInitialInputs)(emb_model, double[]);
int (*obsmodel_getInitialState)(emb_model, const double[], const double[], double[]);
int (*obsmodel_getDerivatives)(emb_model, const double[], const double[], double[], double*);
int (*obsmodel_getParameterName)(emb_model, int, char[], size_t, size_t*);
int (*obsmodel_getStateName)(emb_model, int, char[], size_t, size_t*);
int (*obsmodel_getInputName)(emb_model, int, char[], size_t, size_t*);
int (*obsmodel_getOutputName)(emb_model, int, char[], size_t, size_t*);

//Optimizer
emb_optimizer (*opt_createOptimizer)(emb_model);
emb_opt_context (*opt_initOptContext)(emb_optimizer);
emb_opt_context (*opt_cloneOptContext)(emb_optimizer, emb_opt_context);
emb_size_type (*opt_getOptModelParameterCount)(emb_optimizer);
emb_size_type (*opt_getOptModelStateCount)(emb_optimizer);
emb_size_type (*opt_getOptModelInputCount)(emb_optimizer);
emb_size_type (*opt_getOptModelOutputCount)(emb_optimizer);
int (*opt_getOptModelStateType)(emb_optimizer, int[]);
int (*opt_setOptModelParameter)(emb_optimizer, emb_opt_context, double[], int[]);
int (*opt_setOptModelParameterUncertainty)(emb_optimizer, int[], double[], double[], double[]);
emb_size_type (*opt_getOptParameterCount)(emb_optimizer);
int (*opt_setOptParameter)(emb_optimizer, emb_opt_context, double[]);
int (*opt_makeOptStep)(emb_optimizer, emb_opt_context, double[], double[], double[]);
int (*opt_getOptStepDetails)(emb_optimizer, emb_opt_context, emb_size_type, emb_size_type*, double[], double[], double[]);
int (*opt_getOptModelParameterName)(emb_optimizer, int, char[], size_t, size_t*);
int (*opt_getOptModelStateName)(emb_optimizer, int, char[], size_t, size_t*);
int (*opt_getOptModelInputName)(emb_optimizer, int, char[], size_t, size_t*);
int (*opt_getOptModelOutputName)(emb_optimizer, int, char[], size_t, size_t*);
int (*opt_getOptParameterName)(emb_optimizer, int, char[], size_t, size_t*);
void (*opt_freeOptContext)(emb_optimizer, emb_opt_context);
void (*opt_freeOptimizer)(emb_optimizer);

//Observer
emb_observer (*obs_createObserver)(emb_model);
emb_obs_context (*obs_initObsContext)(emb_observer);
emb_obs_context (*obs_cloneObsContext)(emb_observer, emb_obs_context);
emb_size_type (*obs_getObsModelParameterCount)(emb_observer);
emb_size_type (*obs_getObsModelStateCount)(emb_observer);
emb_size_type (*obs_getObsModelInputCount)(emb_observer);
emb_size_type (*obs_getObsModelOutputCount)(emb_observer);
int (*obs_getObsModelStateType)(emb_observer, int[]);
int (*obs_setObsModelParameter)(emb_observer, emb_obs_context, double[], int[]);
int (*obs_setObsModelParameterUncertainty)(emb_observer, int[], double[], double[], double[]);
emb_size_type (*obs_getObsParameterCount)(emb_observer);
int (*obs_setObsParameter)(emb_observer, emb_obs_context, double[]);
int (*obs_makeObsStep)(emb_observer, emb_obs_context, double[], double[], double[], double[]);
int (*obs_getObsStepDetails)(emb_observer, emb_obs_context, emb_size_type, emb_size_type*, double[], double[], double[]);
int (*obs_getObsModelParameterName)(emb_observer, int, char[], size_t, size_t*);
int (*obs_getObsModelStateName)(emb_observer, int, char[], size_t, size_t*);
int (*obs_getObsModelInputName)(emb_observer, int, char[], size_t, size_t*);
int (*obs_getObsModelOutputName)(emb_observer, int, char[], size_t, size_t*);
int (*obs_getObsParameterName)(emb_observer, int, char[], size_t, size_t*);
void (*obs_freeObsContext)(emb_observer, emb_obs_context);
void (*obs_freeObserver)(emb_observer);

//Simulation/Application
emb_simapp (*sim_createSimApp)();
void (*sim_initSimApp)(emb_simapp);
int (*sim_makeSimStep)(emb_simapp, double[], double[], double[]);
emb_size_type (*sim_getSimParameterCount)(emb_simapp);
emb_size_type (*sim_getSimStateCount)(emb_simapp);
int (*sim_getTrueStates)(emb_simapp, double[]);
int (*sim_getTrueParameters)(emb_simapp, double[]);
void (*sim_freeSimApp)(emb_simapp);

/**
 *  Defining time different function for measuring execution times of function calls
 */
uint64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
	return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
			((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

/**
 * Deep copy of a double array.
 */
void copyDoubleArr(double sourceArr[], double destArr[])
{
	memcpy(destArr, sourceArr, sizeof(sourceArr));
}

/**
 * Concatenate a given name with an integer number to a cstr for the log file
 */
char* createDummyName(string name, int number)
{
	ostringstream outStream;
	outStream << name << number;
	//cout << "DEBUG: " << outStream.str().c_str() << "\r\n";
	return (char*)(outStream.str().c_str());
}

/**
 * Load symbols for OPT_model library
 */
void loadOptModelLibrarySymbols(void* optmodelLibHandle)
{
	optmodel_createModel = (emb_model (*)())dlsym(optmodelLibHandle, "createModel");
	optmodel_freeModel = (void (*)(emb_model))dlsym(optmodelLibHandle, "freeModel");
	optmodel_getParameterCount = (emb_size_type (*)(emb_model))dlsym(optmodelLibHandle, "getParameterCount");
	optmodel_getStateCount = (emb_size_type (*)(emb_model))dlsym(optmodelLibHandle, "getStateCount");
	optmodel_getInputCount = (emb_size_type (*)(emb_model))dlsym(optmodelLibHandle, "getInputCount");
	optmodel_getOutputCount = (emb_size_type (*)(emb_model))dlsym(optmodelLibHandle, "getOutputCount");
	optmodel_getStateType = (int (*)(emb_model, int[]))dlsym(optmodelLibHandle, "getStateType");
	optmodel_setParameter = (int (*)(emb_model, double[]))dlsym(optmodelLibHandle, "setParameter");
	optmodel_getInitialInputs = (int (*)(emb_model, double[]))dlsym(optmodelLibHandle, "getInitialInputs");
	optmodel_getInitialState = (int (*)(emb_model, const double[], const double[], double[]))dlsym(optmodelLibHandle, "getInitialState");
	optmodel_getDerivatives = (int (*)(emb_model, const double[], const double[], double[], double*))dlsym(optmodelLibHandle, "getDerivatives");
	optmodel_getParameterName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(optmodelLibHandle, "getParameterName");
	optmodel_getStateName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(optmodelLibHandle, "getStateName");
	optmodel_getInputName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(optmodelLibHandle, "getInputName");
	optmodel_getOutputName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(optmodelLibHandle, "getOutputName");
}

/**
 * Load symbols of the OBS_model library
 */
void loadObsModelLibrarySymbols(void* obsmodelLibHandle)
{
	obsmodel_createModel = (emb_model (*)())dlsym(obsmodelLibHandle, "createModel");
	obsmodel_freeModel = (void (*)(emb_model))dlsym(obsmodelLibHandle, "freeModel");
	obsmodel_getParameterCount = (emb_size_type (*)(emb_model))dlsym(obsmodelLibHandle, "getParameterCount");
	obsmodel_getStateCount = (emb_size_type (*)(emb_model))dlsym(obsmodelLibHandle, "getStateCount");
	obsmodel_getInputCount = (emb_size_type (*)(emb_model))dlsym(obsmodelLibHandle, "getInputCount");
	obsmodel_getOutputCount = (emb_size_type (*)(emb_model))dlsym(obsmodelLibHandle, "getOutputCount");
	obsmodel_getStateType = (int (*)(emb_model, int[]))dlsym(obsmodelLibHandle, "getStateType");
	obsmodel_setParameter = (int (*)(emb_model, double[]))dlsym(obsmodelLibHandle, "setParameter");
	obsmodel_getInitialInputs = (int (*)(emb_model, double[]))dlsym(obsmodelLibHandle, "getInitialInputs");
	obsmodel_getInitialState = (int (*)(emb_model, const double[], const double[], double[]))dlsym(obsmodelLibHandle, "getInitialState");
	obsmodel_getDerivatives = (int (*)(emb_model, const double[], const double[], double[], double*))dlsym(obsmodelLibHandle, "getDerivatives");
	obsmodel_getParameterName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(obsmodelLibHandle, "getParameterName");
	obsmodel_getStateName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(obsmodelLibHandle, "getStateName");
	obsmodel_getInputName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(obsmodelLibHandle, "getInputName");
	obsmodel_getOutputName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(obsmodelLibHandle, "getOutputName");
}

/**
 * Load symbols of the optimizer library
 */
void loadOptimizerLibrarySymbols(void* optimizerLibHandle)
{
	opt_createOptimizer = (emb_optimizer (*)(emb_model))dlsym(optimizerLibHandle, "createOptimizer");
	opt_initOptContext = (emb_opt_context (*)(emb_optimizer))dlsym(optimizerLibHandle, "initOptContext");
	opt_cloneOptContext = (emb_opt_context (*)(emb_optimizer, emb_opt_context))dlsym(optimizerLibHandle, "cloneOptContext");
	opt_getOptModelParameterCount = (emb_size_type (*)(emb_optimizer))dlsym(optimizerLibHandle, "getOptModelParameterCount");
	opt_getOptModelStateCount = (emb_size_type (*)(emb_optimizer))dlsym(optimizerLibHandle, "getOptModelStateCount");
	opt_getOptModelInputCount = (emb_size_type (*)(emb_optimizer))dlsym(optimizerLibHandle, "getOptModelInputCount");
	opt_getOptModelOutputCount = (emb_size_type (*)(emb_optimizer))dlsym(optimizerLibHandle, "getOptModelOutputCount");
	opt_getOptModelStateType = (int (*)(emb_optimizer, int[]))dlsym(optimizerLibHandle, "getOptModelStateType");
	opt_setOptModelParameter = (int (*)(emb_optimizer, emb_opt_context, double[], int[]))dlsym(optimizerLibHandle, "setOptModelParameter");
	opt_setOptModelParameterUncertainty = (int (*)(emb_optimizer, int[], double[], double[], double[]))dlsym(optimizerLibHandle, "setOptModelParameterUncertainty");
	opt_getOptParameterCount = (emb_size_type (*)(emb_optimizer))dlsym(optimizerLibHandle, "getOptParameterCount");
	opt_setOptParameter = (int (*)(emb_optimizer, emb_opt_context, double[]))dlsym(optimizerLibHandle, "setOptParameter");
	opt_makeOptStep = (int (*)(emb_optimizer, emb_opt_context, double[], double[], double[]))dlsym(optimizerLibHandle, "makeOptStep");
	opt_getOptStepDetails = (int (*)(emb_optimizer, emb_opt_context, emb_size_type, emb_size_type*, double[], double[], double[]))dlsym(optimizerLibHandle, "getOptStepDetails");
	opt_getOptModelParameterName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(optimizerLibHandle, "getOptModelParameterName");
	opt_getOptModelStateName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(optimizerLibHandle, "getOptModelStateName");
	opt_getOptModelInputName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(optimizerLibHandle, "getOptModelInputName");
	opt_getOptModelOutputName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(optimizerLibHandle, "getOptModelOutputName");
	opt_getOptParameterName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(optimizerLibHandle, "getOptParameterName");
	opt_freeOptContext = (void (*)(emb_optimizer, emb_opt_context))dlsym(optimizerLibHandle, "freeOptContext");
	opt_freeOptimizer = (void (*)(emb_optimizer))dlsym(optimizerLibHandle, "freeOptimizer");
}

/**
 * Load symbols of the observer library
 */
void loadObserverLibrarySymbols(void* observerLibHandle)
{
	obs_createObserver = (emb_observer (*)(emb_model))dlsym(observerLibHandle, "createObserver");
	obs_initObsContext = (emb_obs_context (*)(emb_observer))dlsym(observerLibHandle, "initObsContext");
	obs_cloneObsContext = (emb_obs_context (*)(emb_observer, emb_obs_context))dlsym(observerLibHandle, "cloneObsContext");
	obs_getObsModelParameterCount = (emb_size_type (*)(emb_observer))dlsym(observerLibHandle, "getObsModelParameterCount");
	obs_getObsModelStateCount = (emb_size_type (*)(emb_observer))dlsym(observerLibHandle, "getObsModelStateCount");
	obs_getObsModelInputCount = (emb_size_type (*)(emb_observer))dlsym(observerLibHandle, "getObsModelInputCount");
	obs_getObsModelOutputCount = (emb_size_type (*)(emb_observer))dlsym(observerLibHandle, "getObsModelOutputCount");
	obs_getObsModelStateType = (int (*)(emb_observer, int[]))dlsym(observerLibHandle, "getObsModelStateType");
	obs_setObsModelParameter = (int (*)(emb_observer, emb_obs_context, double[], int[]))dlsym(observerLibHandle, "setObsModelParameter");
	obs_setObsModelParameterUncertainty = (int (*)(emb_observer, int[], double[], double[], double[]))dlsym(observerLibHandle, "setObsModelParameterUncertainty");
	obs_getObsParameterCount = (emb_size_type (*)(emb_observer))dlsym(observerLibHandle, "getObsParameterCount");
	obs_setObsParameter = (int (*)(emb_observer, emb_obs_context, double[]))dlsym(observerLibHandle, "setObsParameter");
	obs_makeObsStep = (int (*)(emb_observer, emb_obs_context, double[], double[], double[], double[]))dlsym(observerLibHandle, "makeObsStep");
	obs_getObsStepDetails = (int (*)(emb_observer, emb_obs_context, emb_size_type, emb_size_type*, double[], double[], double[]))dlsym(observerLibHandle, "getObsStepDetails");
	obs_getObsModelParameterName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(observerLibHandle, "getObsModelParameterName");
	obs_getObsModelStateName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(observerLibHandle, "getObsModelStateName");
	obs_getObsModelInputName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(observerLibHandle, "getObsModelInputName");
	obs_getObsModelOutputName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(observerLibHandle, "getObsModelOutputName");
	obs_getObsParameterName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(observerLibHandle, "getObsParameterName");
	obs_freeObsContext = (void (*)(emb_observer, emb_obs_context))dlsym(observerLibHandle, "freeObsContext");
	obs_freeObserver = (void (*)(emb_observer))dlsym(observerLibHandle, "freeObserver");
}

/**
 * Load symbols of the simulation/application library
 */
void loadSimAppLibrarySymbols(void* simappLibHandle)
{
	sim_createSimApp = (emb_simapp (*)())dlsym(simappLibHandle, "createSimApp");
	sim_initSimApp = (void (*)(emb_simapp))dlsym(simappLibHandle, "initSimApp");
	sim_makeSimStep = (int (*)(emb_simapp, double[], double[], double[]))dlsym(simappLibHandle, "makeSimStep");
	sim_getSimParameterCount = (emb_size_type (*)(emb_simapp))dlsym(simappLibHandle, "getSimParameterCount");
	sim_getSimStateCount = (emb_size_type (*)(emb_simapp))dlsym(simappLibHandle, "getSimStateCount");
	sim_getTrueStates = (int (*)(emb_simapp, double[]))dlsym(simappLibHandle, "getTrueStates");
	sim_getTrueParameters = (int (*)(emb_simapp, double[]))dlsym(simappLibHandle, "getTrueParameters");
	sim_freeSimApp = (void (*)(emb_simapp))dlsym(simappLibHandle, "freeSimApp");
}

/**
 * The main EMBOCON supervisor calculation routine!
 * Different libraries for the OptimizerModel and the ObserverModel are loaded.
 */
void startSupervisorRoutine(char* libOptModel, char* libObsModel, char* libOptimizer, char* libObserver, char* libSimApp, int steps)
{
	cout << "** Loading...                                **\r\n";

	openLogFile((char*)"./GEMSTrace.log");
	openCPUTimeMeasLogFile((char*)"./MEAS.log");

	/*
	 * Loading OptModelLib and all symbols...
	 */
	cout << "**   ...OptimizerModelLib...                 **\r\n";
	void* optmodelLibHandle = loadDynamicLibrary(libOptModel);
	loadOptModelLibrarySymbols(optmodelLibHandle);

	/*
	 * Loading ObsModelLib and all symbols...
	 */
	cout << "**   ...ObserverModelLib...                  **\r\n";
	void* obsmodelLibHandle = loadDynamicLibrary(libObsModel);
	loadObsModelLibrarySymbols(obsmodelLibHandle);

	/*
	 * Loading OptimizerLib and all symbols...
	 */
	cout << "**   ...OptimizerLib...                      **\r\n";
	void* optimizerLibHandle = loadDynamicLibrary(libOptimizer);
	loadOptimizerLibrarySymbols(optimizerLibHandle);

	/*
	 * Loading ObserverLib and all symbols...
	 */
	cout << "**   ...ObserverLib...                       **\r\n";
	void* observerLibHandle = loadDynamicLibrary(libObserver);
	loadObserverLibrarySymbols(observerLibHandle);

	/*
	 * Loading SimulationApplicationLib and all symbols...
	 */
	cout << "**   ...SimulationApplicationLib...          **\r\n"
		 << "***********************************************\r\n";
	void* simappLibHandle = loadDynamicLibrary(libSimApp);
	loadSimAppLibrarySymbols(simappLibHandle);


	cout << "** Starting optimization run...              **\r\n"
		 << "**    ...initializing...                     **\r\n";

	//Initialize resources
	emb_model opt_model = (emb_model)optmodel_createModel();
	emb_optimizer opt = (emb_optimizer)opt_createOptimizer(opt_model);
	emb_opt_context cur_opt_context = (emb_opt_context)opt_initOptContext(opt);
	emb_model obs_model = (emb_model)obsmodel_createModel();
	emb_observer obs = (emb_observer)obs_createObserver(obs_model);
	emb_obs_context cur_obs_context = (emb_obs_context)obs_initObsContext(obs);
	emb_simapp simapp = (emb_simapp)sim_createSimApp();

	emb_size_type model_param_count = opt_getOptModelParameterCount(opt);
	emb_size_type model_state_count = opt_getOptModelStateCount(opt);
	emb_size_type model_input_count = opt_getOptModelInputCount(opt);
	emb_size_type model_output_count = opt_getOptModelOutputCount(opt);
	int is_algebraic[model_state_count];
	int resGOMST = (int)opt_getOptModelStateType(opt, is_algebraic);
	if(resGOMST != 0)
	{
		cout << "error in getOptModelStateType() of OptimizerInterface. error-code: " << resGOMST << "\r\n";
		return;
	}
	emb_size_type opt_param_count = opt_getOptParameterCount(opt);
	emb_size_type obs_param_count = obs_getObsParameterCount(obs);

	writeGeneralInfoToLog(model_state_count, model_output_count, model_input_count, model_param_count, opt_param_count, obs_param_count, steps);

	size_t maxlen = 20;

	writeTextToLog((char*)"#step\t");
	size_t x_name_reqlen[model_state_count];
	for(unsigned int i = 0; i < model_state_count; i++)
	{
		char* model_state_name = createDummyName("x_",i);
		int resGOMSN = (int)opt_getOptModelStateName(opt, i, model_state_name, maxlen, &x_name_reqlen[i]);
		if(resGOMSN != 0)
		{
			cout << "error in getOptModelStateName() of OptimizerInterface. error-code: " << resGOMSN << "\r\n";
			cout << "...putting dummy name instead" << "\r\n";
			x_name_reqlen[i] = sizeof(model_state_name);
		}
		writeNameToLog(model_state_name);
	}
	size_t y_name_reqlen[model_output_count];
	for(unsigned int i = 0; i < model_output_count; i++)
	{
		char* model_output_name = createDummyName("y_",i);
		int resGOMON = (int)opt_getOptModelOutputName(opt, i, model_output_name, maxlen, &y_name_reqlen[i]);
		if(resGOMON != 0)
		{
			cout << "error in getOptModelOutputName() of OptimizerInterface. error-code: " << resGOMON << "\r\n";
			cout << "...putting dummy name instead" << "\r\n";
			y_name_reqlen[i] = sizeof(model_output_name);
		}
		writeNameToLog(model_output_name);
	}
	size_t u_name_reqlen[model_input_count];
	for(unsigned int i = 0; i < model_input_count; i++)
	{
		char* model_input_name = createDummyName("u_",i);
		int resGOMIN = (int)opt_getOptModelInputName(opt, i, model_input_name, maxlen, &u_name_reqlen[i]);
		if(resGOMIN != 0)
		{
			cout << "error in getOptModelInputName() of OptimizerInterface. error-code: " << resGOMIN << "\r\n";
			cout << "...putting dummy name instead" << "\r\n";
			u_name_reqlen[i] = sizeof(model_input_name);
		}
		writeNameToLog(model_input_name);
	}
	size_t p_name_reqlen[model_param_count];
	for(unsigned int i = 0; i < model_param_count; i++)
	{
		char* model_param_name = createDummyName("p_",i);
		int resGOMPN = (int)opt_getOptModelParameterName(opt, i, model_param_name, maxlen, &p_name_reqlen[i]);
		if(resGOMPN != 0)
		{
			cout << "error in getOptModelParameterName() of OptimizerInterface. error-code: " << resGOMPN << "\r\n";
			cout << "...putting dummy name instead" << "\r\n";
			p_name_reqlen[i] = sizeof(model_param_name);
		}
		writeNameToLog(model_param_name);
	}
	size_t popt_name_reqlen[opt_param_count];
	for(unsigned int i = 0; i < opt_param_count; i++)
	{
		char* opt_param_name = createDummyName("opt_p_",i);
		int resGOPN = (int)opt_getOptParameterName(opt, i, opt_param_name, maxlen, &popt_name_reqlen[i]);
		if(resGOPN != 0)
		{
			cout << "error in getOptParameterName() of OptimizerInterface. error-code: " << resGOPN << "\r\n";
			cout << "...putting dummy name instead" << "\r\n";
			popt_name_reqlen[i] = sizeof(opt_param_name);
		}
		writeNameToLog(opt_param_name);
	}
	size_t pobs_name_reqlen[obs_param_count];
	for(unsigned int i = 0; i < obs_param_count; i++)
	{
		char* obs_param_name  = createDummyName("obs_p_",i);
		int resGObsPN = (int)obs_getObsParameterName(obs, i, obs_param_name, maxlen, &pobs_name_reqlen[i]);
		if(resGObsPN != 0)
		{
			cout << "error in getObsParameterName() of ObserverInterface. error-code: " << resGObsPN << "\r\n";
			cout << "...putting dummy name instead" << "\r\n";
			pobs_name_reqlen[i] = sizeof(obs_param_name);
		}
		writeNameToLog(obs_param_name);
	}
	writeTextToLog((char*)"\r\n");

	/**cout << "nu: " << model_input_count << "\r\n";
	cout << "nuprev: " << model_input_count << "\r\n";
	cout << "ny: " << model_output_count << "\r\n";
	cout << "nx: " << model_state_count<< "\r\n";
	cout << "nxprev: " << model_state_count << "\r\n";
	cout << "np: " << model_param_count << "\r\n";*/

	double ucur[model_input_count];
	double uprev[model_input_count];
	double ycur[model_output_count];
	double xcur[model_state_count];
	double xprev[model_state_count];
	double pcur[model_param_count];

	/**cout << "init: nu: " << sizeof(ucur)/sizeof(ucur[0]) << "\r\n";
	cout << "init: nuprev: " << sizeof(uprev)/sizeof(uprev[0]) << "\r\n";
	cout << "init: ny: " << sizeof(ycur)/sizeof(ycur[0])  << "\r\n";
	cout << "init: nx: " << sizeof(xcur)/sizeof(xcur[0]) << "\r\n";
	cout << "init: nxprev: " << sizeof(xprev)/sizeof(xprev[0]) << "\r\n";
	cout << "init: np: " << sizeof(pcur)/ sizeof(pcur[0]) << "\r\n";*/

	int resGII = (int)optmodel_getInitialInputs(opt_model, ucur);
	if(resGII != 0)
	{
		cout << "error in getInitialInputs() of ModelInterface for optimizer model. error-code: " << resGII << "\r\n";
		return;
	}

	int resGIS = (int)optmodel_getInitialState(opt_model, ucur, ycur, xcur);
	if(resGIS != 0)
	{
		cout << "error in getInitialState() of ModelInterface for optimizer model. error-code: " << resGIS << "\r\n";
		return;
	}
	sim_initSimApp(simapp);

	cout << "**    ...starting routine...                 **\r\n";

	//Starting the routine;
	/**
	 * Variables for measuring function call times
	 */
	struct timespec start;
	struct timespec end;

	for(int i = 0; i < steps; i++)
	{
		cout << "**    ...step: "<< i <<"...                          **\r\n";

		//MAKE AN OPTIMIZER STEP
		clock_gettime(CLOCK_MONOTONIC, &start);
		int resMOptS = (int)opt_makeOptStep(opt, cur_opt_context, xcur, uprev, ucur);
		clock_gettime(CLOCK_MONOTONIC, &end);
		uint64_t timeElapsed = timespecDiff(&end, &start);
		writeCPUTimeMeasToLog((char*)"optimizer: makeOptStep", timeElapsed);

		if(resMOptS != 0)
		{
			cout << "error in makeOptStep() of OptimizerInterface. error-code: " << resMOptS << "\r\n";
			return;
		}
		copyDoubleArr(ucur, uprev);

		/**cout << "opt:nu: " << sizeof(ucur)/sizeof(ucur[0]) << "\r\n";
		cout << "opt:nuprev: " << sizeof(uprev)/sizeof(uprev[0]) << "\r\n";
		cout << "opt:ny: " << sizeof(ycur)/sizeof(ycur[0])  << "\r\n";
		cout << "opt:nx: " << sizeof(xcur)/sizeof(xcur[0]) << "\r\n";
		cout << "opt:nxprev: " << sizeof(xprev)/sizeof(xprev[0]) << "\r\n";
		cout << "opt:np: " << sizeof(pcur)/ sizeof(pcur[0]) << "\r\n";*/

		//MAKE A SIMULATOR/APPLICATION STEP
		clock_gettime(CLOCK_MONOTONIC, &start);
		int resMSS = (int)sim_makeSimStep(simapp, ucur, ycur, pcur);
		clock_gettime(CLOCK_MONOTONIC, &end);
		timeElapsed = timespecDiff(&end, &start);
		writeCPUTimeMeasToLog((char*)"simulator/application: makeSimStep", timeElapsed);

		if(resMSS != 0)
		{
			cout << "error in makeSimStep() of SimulationApplicationInterface. error-code: " << resMSS << "\r\n";
			return;
		}

		/**cout << "sim:nu: " << sizeof(ucur)/sizeof(ucur[0]) << "\r\n";
		cout << "sim:nuprev: " << sizeof(uprev)/sizeof(uprev[0]) << "\r\n";
		cout << "sim:ny: " << sizeof(ycur)/sizeof(ycur[0])  << "\r\n";
		cout << "sim:nx: " << sizeof(xcur)/sizeof(xcur[0]) << "\r\n";
		cout << "sim:nxprev: " << sizeof(xprev)/sizeof(xprev[0]) << "\r\n";
		cout << "sim:np: " << sizeof(pcur)/ sizeof(pcur[0]) << "\r\n";*/

		//MAKE AN OBSERVER STEP
		clock_gettime(CLOCK_MONOTONIC, &start);
		int resMObsS = (int)obs_makeObsStep(obs, cur_obs_context, ucur, ycur, xprev, xcur);
		clock_gettime(CLOCK_MONOTONIC, &end);
		timeElapsed = timespecDiff(&end, &start);
		writeCPUTimeMeasToLog((char*)"observer: makeObsStep", timeElapsed);

		if(resMObsS != 0)
		{
			cout << "error in makeObsStep() of ObserverInterface. error-code: " << resMObsS << "\r\n";
			return;
		}
		copyDoubleArr(xcur, xprev);

		/**cout << "obs:nu: " << sizeof(ucur)/sizeof(ucur[0]) << "\r\n";
		cout << "obs:nuprev: " << sizeof(uprev)/sizeof(uprev[0]) << "\r\n";
		cout << "obs:ny: " << sizeof(ycur)/sizeof(ycur[0])  << "\r\n";
		cout << "obs:nx: " << sizeof(xcur)/sizeof(xcur[0]) << "\r\n";
		cout << "obs:nxprev: " << sizeof(xprev)/sizeof(xprev[0]) << "\r\n";
		cout << "obs:np: " << sizeof(pcur)/ sizeof(pcur[0]) << "\r\n";*/

		writeStepToLog(i, xcur, model_state_count, ycur, model_output_count, ucur, model_input_count, pcur, model_param_count);
	}

	cout << "**    ...finished routine...                 **\r\n"
	     << "**    ...freeing resources...                **\r\n"
		 << "***********************************************\r\n";

	//Free resources
	opt_freeOptContext(opt, cur_opt_context);
	opt_freeOptimizer(opt);
	optmodel_freeModel(opt_model);
	obs_freeObsContext(obs, cur_obs_context);
	obs_freeObserver(obs);
	obsmodel_freeModel(obs_model);
	sim_freeSimApp(simapp);

	//Close all opened libraries
	closeDynamicLibrary(optmodelLibHandle);
	closeDynamicLibrary(optimizerLibHandle);
	closeDynamicLibrary(obsmodelLibHandle);
	closeDynamicLibrary(observerLibHandle);
	closeDynamicLibrary(simappLibHandle);

	closeCPUTimeMeasLogFile();
	cout << "** Finished.                                 **\r\n"
	     << "***********************************************\r\n";
}

/**
 * Start main function of the supervisor.
 */
int main(int argc, char* argv[])
{
	putWelcomeMsg(cout);
	if(argc == 1)
	{
		//Only GEMS called, no arguments.
		putHelpMsg(cout);
	}
	else if(argc == 2)
	{
		string help = "-h";
		string help1 = "-?";
		if(help.compare(argv[1]) == 0 || help1.compare(argv[1])== 0)
		{
			putHelpMsg(cout);
		}
		else
		{
			putWrongArgsMsg(cout);
		}
	}
	else if(argc == 4)
	{
		string interfaceTest = "-t";
		string zero = "0";
		string one = "1";
		string two = "2";
		if(interfaceTest.compare(argv[1]) == 0)
		{
			char* pathToLib = argv[3];
			if(zero.compare(argv[2]) == 0)
			{
				testModelInterface(pathToLib);
			}
			else if(one.compare(argv[2]) == 0)
			{
				testOptimizerInterface(pathToLib);
			}
			else if(two.compare(argv[2]) == 0)
			{
				testObserverInterface(pathToLib);
			}
		}
		else
		{
			putWrongArgsMsg(cout);
		}
	}
	else if(argc == 6)
	{
		string interfaceTest = "-t";
		string three = "3";

		if(interfaceTest.compare(argv[1]) == 0 && three.compare(argv[2]) == 0)
		{
			char* pathToLib = argv[3];
			testSimAppInterface(pathToLib, atoi(argv[4]), atoi(argv[5]));
		}
		else
		{
			putWrongArgsMsg(cout);
		}
	}
	else if(argc == 7)
	{
		string supervisor = "-r";
		string modelLib = (string)argv[2];
		string optLib1 = (string)argv[3];
		string obsLib1 = (string)argv[4];
		string simLib1 = (string)argv[5];
		if(supervisor.compare(argv[1]) == 0
				&& modelLib.find(".so") < modelLib.size()
				&& optLib1.find(".so") < optLib1.size()
				&& obsLib1.find(".so") < obsLib1.size()
				&& simLib1.find(".so") < simLib1.size())
		{
			cout << "** Starting EMBOCON supervisor routine...    **\r\n"
					<< "***********************************************\r\n";

			startSupervisorRoutine(argv[2], argv[2], argv[3], argv[4], argv[5], atoi(argv[6]));
			writeCMDLineArgsToLog(argv);
			closeLogFile();
		}
		else
		{
			putWrongArgsMsg(cout);
		}
	}
	else if(argc == 8)
	{
		string supervisor = "-r";
		string modelOptLib = (string)argv[2];
		string modelObsLib = (string)argv[3];
		string optLib = (string)argv[4];
		string obsLib = (string)argv[5];
		string simLib = (string)argv[6];

		string interfaceTest = "-t";
		string tmodelLib = (string)argv[2];
		string toptLib = (string)argv[3];
		string tobsLib = (string)argv[4];
		string tsimLib = (string)argv[5];

		if(supervisor.compare(argv[1]) == 0
				&& modelOptLib.find(".so") < modelOptLib.size()
				&& modelObsLib.find(".so") < modelObsLib.size()
				&& optLib.find(".so") < optLib.size()
				&& obsLib.find(".so") < obsLib.size()
				&& simLib.find(".so") < simLib.size())
		{
			cout << "** Starting EMBOCON supervisor routine...    **\r\n"
					<< "***********************************************\r\n";
			startSupervisorRoutine(argv[2], argv[3], argv[4], argv[5], argv[6], atoi(argv[7]));
			writeCMDLineArgsToLog(argv);
			closeLogFile();
		}
		else if(interfaceTest.compare(argv[1]) == 0
				&& tmodelLib.find(".so") < tmodelLib.size()
				&& toptLib.find(".so") < toptLib.size()
				&& tobsLib.find(".so") < tobsLib.size()
				&& tsimLib.find(".so") < tsimLib.size())
		{
			testInterfacesOfLibraries(argv[2], argv[3], argv[4], argv[5], atoi(argv[6]), atoi(argv[7]));
		}
		else
		{
			putWrongArgsMsg(cout);
		}
	}
	else if(argc == 9)
	{
		string interfaceTest = "-t";
		string modelOptLib = (string)argv[2];
		string modelObsLib = (string)argv[3];
		string optLib = (string)argv[4];
		string obsLib = (string)argv[5];
		string simLib = (string)argv[6];

		if(interfaceTest.compare(argv[1]) == 0
				&& modelOptLib.find(".so") < modelOptLib.size()
				&& modelObsLib.find(".so") < modelObsLib.size()
				&& optLib.find(".so") < optLib.size()
				&& obsLib.find(".so") < obsLib.size()
				&& simLib.find(".so") < simLib.size())
		{
			testInterfacesOfLibraries(argv[2], argv[3], argv[4], argv[5], argv[6], atoi(argv[7]), atoi(argv[8]));
		}
		else
		{
			putWrongArgsMsg(cout);
		}
	}
	else
	{
		for(int i = 0; i < argc; i++)
		{
			cout << "argv[" << i << "] = " << argv[i] << "\r\n";
		}
	}
	return EXIT_SUCCESS;
}
