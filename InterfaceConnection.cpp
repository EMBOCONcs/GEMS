//============================================================================
// Name        : InterfaceConnection.cpp
// Author      : Christian Schoppmeyer
// Version     :
// Created On  : 22.11.2011
// Copyright   : EMBOCON Copyright
// Description :
//============================================================================

#include "InterfaceConnection.h"
//Include library for dynamic library handling
#include <dlfcn.h>
//Include EMBOCON interfaces
#include "./Interfaces/EMBOCON_DataTypes.h"
#include "./Interfaces/EMBOCON_ModelInterface.h"
#include "./Interfaces/EMBOCON_OptimizerInterface.h"
#include "./Interfaces/EMBOCON_ObserverInterface.h"
#include "./Interfaces/EMBOCON_SimulationApplicationInterface.h"
#include <iostream>

using namespace std;

void* loadDynamicLibrary(char* path)
{
	void* libHandle = dlopen(path, RTLD_LAZY);
	return libHandle;
}

void closeDynamicLibrary(void* libHandle)
{
	dlclose(libHandle);
}

void testModelInterface(char* path)
{
	void* handle = loadDynamicLibrary(path);

	cout << "\r\n";
	cout << "***********************************************\r\n";
	cout << "***********************************************\r\n";
	cout << "** Testing EMBOCON Model Interface           **\r\n";
	cout << "**   function calls...                       **\r\n";
	cout << "***********************************************\r\n";

	emb_model (*createModel)();
	createModel = (emb_model (*)())dlsym(handle, "createModel");
	emb_model modelHandle = (emb_model)createModel();
	cout << "** (01/15) - createModel():           passed **\r\n";

	emb_size_type (*getParameterCount)(emb_model);
	getParameterCount = (emb_size_type (*)(emb_model))dlsym(handle, "getParameterCount");
	emb_size_type countParam = (emb_size_type)getParameterCount(modelHandle);
	cout << "** (02/15) - getParameterCount():     passed **\r\n";

	emb_size_type (*getStateCount)(emb_model);
	getStateCount = (emb_size_type (*)(emb_model))dlsym(handle, "getStateCount");
	emb_size_type countState = (emb_size_type)getStateCount(modelHandle);
	cout << "** (03/15) - getStateCount():         passed **\r\n";

	emb_size_type (*getInputCount)(emb_model);
	getInputCount = (emb_size_type (*)(emb_model))dlsym(handle, "getInputCount");
	emb_size_type countInput = (emb_size_type)getInputCount(modelHandle);
	cout << "** (04/15) - getInputCount():         passed **\r\n";

	emb_size_type (*getOutputCount)(emb_model);
	getOutputCount = (emb_size_type (*)(emb_model))dlsym(handle, "getOutputCount");
	emb_size_type countOutput = (emb_size_type)getOutputCount(modelHandle);
	cout << "** (05/15) - getOutputCount():        passed **\r\n";

	int is_algebraic[countState];
	int (*getStateType)(emb_model, int[]);
	getStateType = (int (*)(emb_model, int[]))dlsym(handle, "getStateType");
	int resGST = (int)getStateType(modelHandle, is_algebraic);
	if(resGST == 0)
	{
		cout << "** (06/15) - getStateType():          passed **\r\n";
	}
	else
	{
		cout << "** (06/15) - getStateType():          failed **\r\n";
	}

	double p[countParam];
	int (*setParameter)(emb_model, double[]);
	setParameter = (int (*)(emb_model, double[]))dlsym(handle, "setParameter");
	int resSP = (int)setParameter(modelHandle, p);
	if(resSP == 0)
	{
		cout << "** (07/15) - setParameter():          passed **\r\n";
	}
	else
	{
		cout << "** (07/15) - setParameter():          failed **\r\n";
	}

	double u0[countInput];
	int (*getInitialInputs)(emb_model, double[]);
	getInitialInputs = (int (*)(emb_model, double[]))dlsym(handle, "getInitialInputs");
	int resGII = (int)getInitialInputs(modelHandle, u0);
	if(resGII == 0)
	{
		cout << "** (08/15) - getInitialInputs():       passed **\r\n";
	}
	else
	{
		cout << "** (08/15) - getInitialInputs():       failed **\r\n";
	}

	double u[countInput];
	double y[countOutput];
	double x0[countState];
	int (*getInitialState)(emb_model, const double[], const double[], double[]);
	getInitialState = (int (*)(emb_model, const double[], const double[], double[]))dlsym(handle, "getInitialState");
	int resGIS = (int)getInitialState(modelHandle, u, y, x0);
	if(resGIS == 0)
	{
		cout << "** (09/15) - getInitialState():       passed **\r\n";
	}
	else
	{
		cout << "** (09/15) - getInitialState():       failed **\r\n";
	}

	double x[countState];
	double u1[countOutput];
	double deriv_resid[countState];
	double objective;
	int (*getDerivatives)(emb_model, const double[], const double[], double[], double*);
	getDerivatives = (int (*)(emb_model, const double[], const double[], double[], double*))dlsym(handle, "getDerivatives");
	int resGD = (int)getDerivatives(modelHandle, x, u1, deriv_resid, &objective);
	if(resGD == 0)
	{
		cout << "** (10/15) - getDerivatives():        passed **\r\n";
	}
	else
	{
		cout << "** (10/15) - getDerivatives():        failed **\r\n";
	}

	int p_name_idx = 0;
	size_t p_maxlen = 10;
	char par_name[p_maxlen];
	size_t p_reqLen;
	int (*getParameterName)(emb_model, int, char[], size_t, size_t*);
	getParameterName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(handle, "getParameterName");
	int resGPN = (int)getParameterName(modelHandle, p_name_idx, par_name, p_maxlen, &p_reqLen);
	if(resGPN == 0)
	{
		cout << "** (11/15) - getParameterName():      passed **\r\n";
	}
	else
	{
		cout << "** (11/15) - getParameterName():      failed **\r\n";
	}

	int x_name_idx = 0;
	size_t x_maxlen = 10;
	char state_name[x_maxlen];
	size_t x_reqLen;
	int (*getStateName)(emb_model, int, char[], size_t, size_t*);
	getStateName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(handle, "getStateName");
	int resGSN = (int)getStateName(modelHandle, x_name_idx, state_name, x_maxlen, &x_reqLen);
	if(resGSN == 0)
	{
		cout << "** (12/15) - getStateName():          passed **\r\n";
	}
	else
	{
		cout << "** (12/15) - getStateName():          failed **\r\n";
	}

	int u_name_idx = 0;
	size_t u_maxlen = 10;
	char input_name[u_maxlen];
	size_t u_reqLen;
	int (*getInputName)(emb_model, int, char[], size_t, size_t*);
	getInputName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(handle, "getInputName");
	int resGIN = (int)getInputName(modelHandle, u_name_idx, input_name, u_maxlen, &u_reqLen);
	if(resGIN == 0)
	{
		cout << "** (13/15) - getInputName():          passed **\r\n";
	}
	else
	{
		cout << "** (13/15) - getInputName():          failed **\r\n";
	}

	int y_name_idx = 0;
	size_t y_maxlen = 10;
	char output_name[y_maxlen];
	size_t y_reqLen;
	int (*getOutputName)(emb_model, int, char[], size_t, size_t*);
	getOutputName = (int (*)(emb_model, int, char[], size_t, size_t*))dlsym(handle, "getOutputName");
	int resGON = (int)getOutputName(modelHandle, y_name_idx, output_name, y_maxlen, &y_reqLen);
	if(resGON == 0)
	{
		cout << "** (14/15) - getOutputName():         passed **\r\n";
	}
	else
	{
		cout << "** (14/15) - getOutputName():         failed **\r\n";
	}

	void (*freeModel)(emb_model);
	freeModel = (void (*)(emb_model))dlsym(handle, "freeModel");
	freeModel(modelHandle);
	cout << "** (15/15) - freeModel():             passed **\r\n";
	cout << "***********************************************\r\n";

	closeDynamicLibrary(handle);
}

void testOptimizerInterface(char* path)
{
	void* handle = loadDynamicLibrary(path);

	cout << "\r\n";
	cout << "***********************************************\r\n";
	cout << "***********************************************\r\n";
	cout << "** Testing EMBOCON Optimizer Interface       **\r\n";
	cout << "**   function calls...                       **\r\n";
	cout << "***********************************************\r\n";

	emb_model modelHandle = 0;

	emb_optimizer (*createOptimizer)(emb_model);
	createOptimizer = (emb_optimizer (*)(emb_model))dlsym(handle, "createOptimizer");
	emb_optimizer optHandle = (emb_optimizer)createOptimizer(modelHandle);
	cout << "** (01/21) - createOptimizer():       passed **\r\n";

	emb_opt_context (*initOptContext)(emb_optimizer);
	initOptContext = (emb_opt_context (*)(emb_optimizer))dlsym(handle, "initOptContext");
	emb_opt_context contextHandle = (emb_opt_context)initOptContext(optHandle);
	cout << "** (02/21) - initOptContext():        passed **\r\n";

	emb_opt_context (*cloneOptContext)(emb_optimizer, emb_opt_context);
	cloneOptContext = (emb_opt_context (*)(emb_optimizer, emb_opt_context))dlsym(handle, "cloneOptContext");
	emb_opt_context cloneHandle = (emb_opt_context)cloneOptContext(optHandle, contextHandle);
	cout << "** (03/21) - cloneOptContext():       passed **\r\n";

	emb_size_type (*getOptModelParameterCount)(emb_optimizer);
	getOptModelParameterCount = (emb_size_type (*)(emb_optimizer))dlsym(handle, "getOptModelParameterCount");
	emb_size_type countParam = (emb_size_type)getOptModelParameterCount(optHandle);
	cout << "** (04/21) - getOptModelParameterCount(): p. **\r\n";

	emb_size_type (*getOptModelStateCount)(emb_optimizer);
	getOptModelStateCount = (emb_size_type (*)(emb_optimizer))dlsym(handle, "getOptModelStateCount");
	emb_size_type countState = (emb_size_type)getOptModelStateCount(optHandle);
	cout << "** (05/21) - getOptModelStateCount(): passed **\r\n";

	emb_size_type (*getOptModelInputCount)(emb_optimizer);
	getOptModelInputCount = (emb_size_type (*)(emb_optimizer))dlsym(handle, "getOptModelInputCount");
	emb_size_type countInput = (emb_size_type)getOptModelInputCount(optHandle);
	cout << "** (06/21) - getOptModelInputCount(): passed **\r\n";

	emb_size_type (*getOptModelOutputCount)(emb_optimizer);
	getOptModelOutputCount = (emb_size_type (*)(emb_optimizer))dlsym(handle, "getOptModelOutputCount");
	emb_size_type countOutput = (emb_size_type)getOptModelOutputCount(optHandle);
	cout << "** (07/21) - getOptModelOutputCount(): pass. **\r\n";

	int is_algebraic[countState];
	int (*getOptModelStateType)(emb_optimizer, int[]);
	getOptModelStateType = (int (*)(emb_optimizer, int[]))dlsym(handle, "getOptModelStateType");
	int resGST = (int)getOptModelStateType(optHandle, is_algebraic);
	if(resGST == 0)
	{
		cout << "** (08/21) - getOptModelStateType():  passed **\r\n";
	}
	else
	{
		cout << "** (08/21) - getOptModelStateType():  failed **\r\n";
	}

	double p[countParam];
	int p_flag[countParam];
	int (*setOptModelParameter)(emb_optimizer, emb_opt_context, double[], int[]);
	setOptModelParameter = (int (*)(emb_optimizer, emb_opt_context, double[], int[]))dlsym(handle, "setOptModelParameter");
	int resSOMP = (int)setOptModelParameter(optHandle, contextHandle, p, p_flag);
	if(resSOMP == 0)
	{
		cout << "** (09/21) - setOptModelParameter():  passed **\r\n";
	}
	else
	{
		cout << "** (09/21) - setOptModelParameter():  failed **\r\n";
	}

	int uncType[countParam];
	double lower[countParam];
	double upper[countParam];
	double cov[countParam];
	int (*setOptModelParameterUncertainty)(emb_optimizer, int[], double[], double[], double[]);
	setOptModelParameterUncertainty = (int (*)(emb_optimizer, int[], double[], double[], double[]))dlsym(handle, "setOptModelParameterUncertainty");
	int resSOMPU = (int)setOptModelParameterUncertainty(optHandle, uncType, lower, upper, cov);
	if(resSOMPU == 0)
	{
		cout << "** (10/21) - setOptModelPar.Uncertainty(): p.**\r\n";
	}
	else
	{
		cout << "** (10/21) - setOptModelPar.Uncertainty(): f.**\r\n";
	}

	emb_size_type (*getOptParameterCount)(emb_optimizer);
	getOptParameterCount = (emb_size_type (*)(emb_optimizer))dlsym(handle, "getOptParameterCount");
	emb_size_type countOptParam = (emb_size_type)getOptParameterCount(optHandle);
	cout << "** (11/21) - getOptParameterCount():  passed **\r\n";

	double optp[countOptParam];
	int (*setOptParameter)(emb_optimizer, emb_opt_context, double[]);
	setOptParameter = (int (*)(emb_optimizer, emb_opt_context, double[]))dlsym(handle, "setOptParameter");
	int resSOP = (int)setOptParameter(optHandle, contextHandle, optp);
	if(resSOP == 0)
	{
		cout << "** (12/21) - setOptParameter():       passed **\r\n";
	}
	else
	{
		cout << "** (12/21) - setOptParameter():       failed **\r\n";
	}

	double xcur[countState];
	double uprev[countInput];
	double ucur[countInput];
	int (*makeOptStep)(emb_optimizer, emb_opt_context, double[], double[], double[]);
	makeOptStep = (int (*)(emb_optimizer, emb_opt_context, double[], double[], double[]))dlsym(handle, "makeOptStep");
	int resMOS = (int)makeOptStep(optHandle, contextHandle, xcur, uprev, ucur);
	if(resMOS == 0)
	{
		cout << "** (13/21) - makeOptStep():           passed **\r\n";
	}
	else
	{
		cout << "** (13/21) - makeOptStep():           failed **\r\n";
	}

	emb_size_type predHor = 5;
	emb_size_type actHor;
	double uPredict[predHor];
	double yPredict[predHor];
	double xPredict[predHor];
	int (*getOptStepDetails)(emb_optimizer, emb_opt_context, emb_size_type, emb_size_type*, double[], double[], double[]);
	getOptStepDetails = (int (*)(emb_optimizer, emb_opt_context, emb_size_type, emb_size_type*, double[], double[], double[]))dlsym(handle, "getOptStepDetails");
	int resGOSD = (int)getOptStepDetails(optHandle, contextHandle, predHor, &actHor, uPredict, yPredict, xPredict);
	if(resGOSD == 0)
	{
		cout << "** (14/21) - getOptStepDetails():     passed **\r\n";
	}
	else
	{
		cout << "** (14/21) - getOptStepDetails():     failed **\r\n";
	}

	int p_name_idx = 0;
	size_t p_maxlen = 10;
	char par_name[p_maxlen];
	size_t p_reqLen;
	int (*getOptModelParameterName)(emb_optimizer, int, char[], size_t, size_t*);
	getOptModelParameterName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(handle, "getOptModelParameterName");
	int resGPN = (int)getOptModelParameterName(optHandle, p_name_idx, par_name, p_maxlen, &p_reqLen);
	if(resGPN == 0)
	{
		cout << "** (15/21) - getOptModelParameterName(): pa. **\r\n";
	}
	else
	{
		cout << "** (15/21) - getOptModelParameterName(): fa. **\r\n";
	}

	int x_name_idx = 0;
	size_t x_maxlen = 10;
	char state_name[x_maxlen];
	size_t x_reqLen;
	int (*getOptModelStateName)(emb_optimizer, int, char[], size_t, size_t*);
	getOptModelStateName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(handle, "getOptModelStateName");
	int resGSN = (int)getOptModelStateName(optHandle, x_name_idx, state_name, x_maxlen, &x_reqLen);
	if(resGSN == 0)
	{
		cout << "** (16/21) - getOptModelStateName():  passed **\r\n";
	}
	else
	{
		cout << "** (16/21) - getOptModelStateName():  failed **\r\n";
	}

	int u_name_idx = 0;
	size_t u_maxlen = 10;
	char input_name[u_maxlen];
	size_t u_reqLen;
	int (*getOptModelInputName)(emb_optimizer, int, char[], size_t, size_t*);
	getOptModelInputName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(handle, "getOptModelInputName");
	int resGIN = (int)getOptModelInputName(optHandle, u_name_idx, input_name, u_maxlen, &u_reqLen);
	if(resGIN == 0)
	{
		cout << "** (17/21) - getOptModelInputName():  passed **\r\n";
	}
	else
	{
		cout << "** (17/21) - getOptModelInputName():  failed **\r\n";
	}

	int y_name_idx = 0;
	size_t y_maxlen = 10;
	char output_name[y_maxlen];
	size_t y_reqLen;
	int (*getOptModelOutputName)(emb_optimizer, int, char[], size_t, size_t*);
	getOptModelOutputName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(handle, "getOptModelOutputName");
	int resGON = (int)getOptModelOutputName(optHandle, y_name_idx, output_name, y_maxlen, &y_reqLen);
	if(resGON == 0)
	{
		cout << "** (18/21) - getOptModelOutputName(): passed **\r\n";
	}
	else
	{
		cout << "** (18/21) - getOptModelOutputName(): failed **\r\n";
	}

	int optp_name_idx = 0;
	size_t optp_maxlen = 10;
	char optp_name[optp_maxlen];
	size_t optp_reqLen;
	int (*getOptParameterName)(emb_optimizer, int, char[], size_t, size_t*);
	getOptParameterName = (int (*)(emb_optimizer, int, char[], size_t, size_t*))dlsym(handle, "getOptParameterName");
	int resGOPN = (int)getOptParameterName(optHandle, optp_name_idx, optp_name, optp_maxlen, &optp_reqLen);
	if(resGOPN == 0)
	{
		cout << "** (19/21) - getOptParameterName():   passed **\r\n";
	}
	else
	{
		cout << "** (19/21) - getOptParameterName():   failed **\r\n";
	}

	void (*freeOptContext)(emb_optimizer, emb_opt_context);
	freeOptContext = (void (*)(emb_optimizer, emb_opt_context))dlsym(handle, "freeOptContext");
	freeOptContext(optHandle, contextHandle);
	freeOptContext(optHandle, cloneHandle);
	cout << "** (20/21) - freeOptContext():        passed **\r\n";

	void (*freeOptimizer)(emb_optimizer);
	freeOptimizer = (void (*)(emb_optimizer))dlsym(handle, "freeOptimizer");
	freeOptimizer(optHandle);
	cout << "** (21/21) - freeOptimizer():         passed **\r\n";
	cout << "***********************************************\r\n";

	closeDynamicLibrary(handle);
}

void testObserverInterface(char* path)
{
	void* handle = loadDynamicLibrary(path);

	cout << "\r\n";
	cout << "***********************************************\r\n";
	cout << "***********************************************\r\n";
	cout << "** Testing EMBOCON Observer Interface        **\r\n";
	cout << "**   function calls...                       **\r\n";
	cout << "***********************************************\r\n";

	emb_model modelHandle = 0;

	emb_observer (*createObserver)(emb_model);
	createObserver = (emb_observer (*)(emb_model))dlsym(handle, "createObserver");
	emb_observer obsHandle = (emb_observer)createObserver(modelHandle);
	cout << "** (01/21) - createObserver():        passed **\r\n";

	emb_obs_context (*initObsContext)(emb_observer);
	initObsContext = (emb_obs_context (*)(emb_observer))dlsym(handle, "initObsContext");
	emb_obs_context contextHandle = (emb_obs_context)initObsContext(obsHandle);
	cout << "** (02/21) - initObsContext():        passed **\r\n";

	emb_obs_context (*cloneObsContext)(emb_observer, emb_obs_context);
	cloneObsContext = (emb_obs_context (*)(emb_observer, emb_obs_context))dlsym(handle, "cloneObsContext");
	emb_obs_context cloneHandle = (emb_obs_context)cloneObsContext(obsHandle, contextHandle);
	cout << "** (03/21) - cloneObsContext():       passed **\r\n";

	emb_size_type (*getObsModelParameterCount)(emb_observer);
	getObsModelParameterCount = (emb_size_type (*)(emb_observer))dlsym(handle, "getObsModelParameterCount");
	emb_size_type countParam = (emb_size_type)getObsModelParameterCount(obsHandle);
	cout << "** (04/21) - getObsModelParameterCount(): p. **\r\n";

	emb_size_type (*getObsModelStateCount)(emb_observer);
	getObsModelStateCount = (emb_size_type (*)(emb_observer))dlsym(handle, "getObsModelStateCount");
	emb_size_type countState = (emb_size_type)getObsModelStateCount(obsHandle);
	cout << "** (05/21) - getObsModelStateCount(): passed **\r\n";

	emb_size_type (*getObsModelInputCount)(emb_observer);
	getObsModelInputCount = (emb_size_type (*)(emb_observer))dlsym(handle, "getObsModelInputCount");
	emb_size_type countInput = (emb_size_type)getObsModelInputCount(obsHandle);
	cout << "** (06/21) - getObsModelInputCount(): passed **\r\n";

	emb_size_type (*getObsModelOutputCount)(emb_observer);
	getObsModelOutputCount = (emb_size_type (*)(emb_observer))dlsym(handle, "getObsModelOutputCount");
	emb_size_type countOutput = (emb_size_type)getObsModelOutputCount(obsHandle);
	cout << "** (07/21) - getObsModelOutputCount(): pass. **\r\n";

	int is_algebraic[countState];
	int (*getObsModelStateType)(emb_observer, int[]);
	getObsModelStateType = (int (*)(emb_observer, int[]))dlsym(handle, "getObsModelStateType");
	int resGST = (int)getObsModelStateType(obsHandle, is_algebraic);
	if(resGST == 0)
	{
		cout << "** (08/21) - getObsModelStateType():  passed **\r\n";
	}
	else
	{
		cout << "** (08/21) - getObsModelStateType():  failed **\r\n";
	}

	double p[countParam];
	int p_flag[countParam];
	int (*setObsModelParameter)(emb_observer, emb_obs_context, double[], int[]);
	setObsModelParameter = (int (*)(emb_observer, emb_obs_context, double[], int[]))dlsym(handle, "setObsModelParameter");
	int resSOMP = (int)setObsModelParameter(obsHandle, contextHandle, p, p_flag);
	if(resSOMP == 0)
	{
		cout << "** (09/21) - setObsModelParameter():  passed **\r\n";
	}
	else
	{
		cout << "** (09/21) - setObsModelParameter():  failed **\r\n";
	}

	int uncType[countParam];
	double lower[countParam];
	double upper[countParam];
	double cov[countParam];
	int (*setObsModelParameterUncertainty)(emb_observer, int[], double[], double[], double[]);
	setObsModelParameterUncertainty = (int (*)(emb_observer, int[], double[], double[], double[]))dlsym(handle, "setObsModelParameterUncertainty");
	int resSOMPU = (int)setObsModelParameterUncertainty(obsHandle, uncType, lower, upper, cov);
	if(resSOMPU == 0)
	{
		cout << "** (10/21) - setObsModelPar.Uncertainty(): p.**\r\n";
	}
	else
	{
		cout << "** (10/21) - setObsModelPar.Uncertainty(): f.**\r\n";
	}

	emb_size_type (*getObsParameterCount)(emb_observer);
	getObsParameterCount = (emb_size_type (*)(emb_observer))dlsym(handle, "getObsParameterCount");
	emb_size_type countObsParam = (emb_size_type)getObsParameterCount(obsHandle);
	cout << "** (11/21) - getObsParameterCount():  passed **\r\n";

	double obsp[countObsParam];
	int (*setObsParameter)(emb_observer, emb_obs_context, double[]);
	setObsParameter = (int (*)(emb_observer, emb_obs_context, double[]))dlsym(handle, "setObsParameter");
	int resSOP = (int)setObsParameter(obsHandle, contextHandle, obsp);
	if(resSOP == 0)
	{
		cout << "** (12/21) - setObsParameter():       passed **\r\n";
	}
	else
	{
		cout << "** (12/21) - setObsParameter():       failed **\r\n";
	}

	double ucur[countInput];
	double ycur[countOutput];
	double xprev[countState];
	double xcur[countState];
	int (*makeObsStep)(emb_observer, emb_obs_context, double[], double[], double[], double[]);
	makeObsStep = (int (*)(emb_observer, emb_obs_context, double[], double[], double[], double[]))dlsym(handle, "makeObsStep");
	int resMOS = (int)makeObsStep(obsHandle, contextHandle, ucur, ycur, xprev, xcur);
	if(resMOS == 0)
	{
		cout << "** (13/21) - makeObsStep():           passed **\r\n";
	}
	else
	{
		cout << "** (13/21) - makeObsStep():           failed **\r\n";
	}

	emb_size_type pastHor = 5;
	emb_size_type actHor;
	double uPast[pastHor];
	double yPast[pastHor];
	double xPast[pastHor];
	int (*getObsStepDetails)(emb_observer, emb_obs_context, emb_size_type, emb_size_type*, double[], double[], double[]);
	getObsStepDetails = (int (*)(emb_observer, emb_obs_context, emb_size_type, emb_size_type*, double[], double[], double[]))dlsym(handle, "getObsStepDetails");
	int resGOSD = (int)getObsStepDetails(obsHandle, contextHandle, pastHor, &actHor, uPast, yPast, xPast);
	if(resGOSD == 0)
	{
		cout << "** (14/21) - getObsStepDetails():     passed **\r\n";
	}
	else
	{
		cout << "** (14/21) - getObsStepDetails():     failed **\r\n";
	}

	int p_name_idx = 0;
	size_t p_maxlen = 10;
	char par_name[p_maxlen];
	size_t p_reqLen;
	int (*getObsModelParameterName)(emb_observer, int, char[], size_t, size_t*);
	getObsModelParameterName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(handle, "getObsModelParameterName");
	int resGPN = (int)getObsModelParameterName(obsHandle, p_name_idx, par_name, p_maxlen, &p_reqLen);
	if(resGPN == 0)
	{
		cout << "** (15/21) - getObsModelParameterName(): pa. **\r\n";
	}
	else
	{
		cout << "** (15/21) - getObsModelParameterName(): fa. **\r\n";
	}

	int x_name_idx = 0;
	size_t x_maxlen = 10;
	char state_name[x_maxlen];
	size_t x_reqLen;
	int (*getObsModelStateName)(emb_observer, int, char[], size_t, size_t*);
	getObsModelStateName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(handle, "getObsModelStateName");
	int resGSN = (int)getObsModelStateName(obsHandle, x_name_idx, state_name, x_maxlen, &x_reqLen);
	if(resGSN == 0)
	{
		cout << "** (16/21) - getObsModelStateName():  passed **\r\n";
	}
	else
	{
		cout << "** (16/21) - getObsModelStateName():  failed **\r\n";
	}

	int u_name_idx = 0;
	size_t u_maxlen = 10;
	char input_name[u_maxlen];
	size_t u_reqLen;
	int (*getObsModelInputName)(emb_observer, int, char[], size_t, size_t*);
	getObsModelInputName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(handle, "getObsModelInputName");
	int resGIN = (int)getObsModelInputName(obsHandle, u_name_idx, input_name, u_maxlen, &u_reqLen);
	if(resGIN == 0)
	{
		cout << "** (17/21) - getObsModelInputName():  passed **\r\n";
	}
	else
	{
		cout << "** (17/21) - getObsModelInputName():  failed **\r\n";
	}

	int y_name_idx = 0;
	size_t y_maxlen = 10;
	char output_name[y_maxlen];
	size_t y_reqLen;
	int (*getObsModelOutputName)(emb_observer, int, char[], size_t, size_t*);
	getObsModelOutputName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(handle, "getObsModelOutputName");
	int resGON = (int)getObsModelOutputName(obsHandle, y_name_idx, output_name, y_maxlen, &y_reqLen);
	if(resGON == 0)
	{
		cout << "** (18/21) - getObsModelOutputName(): passed **\r\n";
	}
	else
	{
		cout << "** (18/21) - getobsModelOutputName(): failed **\r\n";
	}

	int obsp_name_idx = 0;
	size_t obsp_maxlen = 10;
	char obsp_name[obsp_maxlen];
	size_t obsp_reqLen;
	int (*getObsParameterName)(emb_observer, int, char[], size_t, size_t*);
	getObsParameterName = (int (*)(emb_observer, int, char[], size_t, size_t*))dlsym(handle, "getObsParameterName");
	int resGOPN = (int)getObsParameterName(obsHandle, obsp_name_idx, obsp_name, obsp_maxlen, &obsp_reqLen);
	if(resGOPN == 0)
	{
		cout << "** (19/21) - getObsParameterName():   passed **\r\n";
	}
	else
	{
		cout << "** (19/21) - getObsParameterName():   failed **\r\n";
	}

	void (*freeObsContext)(emb_observer, emb_obs_context);
	freeObsContext = (void (*)(emb_observer, emb_obs_context))dlsym(handle, "freeObsContext");
	freeObsContext(obsHandle, contextHandle);
	freeObsContext(obsHandle, cloneHandle);
	cout << "** (20/21) - freeObsContext():        passed **\r\n";

	void (*freeObserver)(emb_observer);
	freeObserver = (void (*)(emb_observer))dlsym(handle, "freeObserver");
	freeObserver(obsHandle);
	cout << "** (21/21) - freeObserver():          passed **\r\n";
	cout << "***********************************************\r\n";

	closeDynamicLibrary(handle);
}

void testSimAppInterface(char* path, int countU, int countMeas)
{
	void* handle = loadDynamicLibrary(path);

	cout << "\r\n";
	cout << "***********************************************\r\n";
	cout << "***********************************************\r\n";
	cout << "** Testing EMBOCON SimApp Interface          **\r\n";
	cout << "**   function calls...                       **\r\n";
	cout << "***********************************************\r\n";

	emb_simapp (*createSimApp)();
	createSimApp = (emb_simapp (*)())dlsym(handle, "createSimApp");
	emb_simapp simappHandle = (emb_simapp)createSimApp();
	cout << "** (01/08) - createSimApp():          passed **\r\n";

	void (*initSimApp)(emb_simapp);
	initSimApp = (void (*)(emb_simapp))dlsym(handle, "initSimApp");
	initSimApp(simappHandle);
	cout << "** (02/08) - initSimApp():            passed **\r\n";

	emb_size_type (*getSimStateCount)(emb_simapp);
	getSimStateCount = (emb_size_type (*)(emb_simapp))dlsym(handle, "getSimStateCount");
	emb_size_type countState = (emb_size_type)getSimStateCount(simappHandle);
	cout << "** (03/08) - getSimStateCount():      passed **\r\n";

	double ucur[countU];
	double meascur[countMeas];
	double pcur[countState];
	int (*makeSimStep)(emb_simapp, double[], double[], double[]);
	makeSimStep = (int (*)(emb_simapp, double[], double[], double[]))dlsym(handle, "makeSimStep");
	int resISA = (int)makeSimStep(simappHandle, ucur, meascur, pcur);
	if(resISA == 0)
	{
		cout << "** (04/08) - makeSimStep():           passed **\r\n";
	}
	else
	{
		cout << "** (04/08) - makeSimStep():           failed **\r\n";
	}

	emb_size_type (*getSimParameterCount)(emb_simapp);
	getSimParameterCount = (emb_size_type (*)(emb_simapp))dlsym(handle, "getSimParameterCount");
	emb_size_type countParam = (emb_size_type)getSimParameterCount(simappHandle);
	cout << "** (05/08) - getSimParameterCount():  passed **\r\n";

	double xtrue[countState];
	int (*getTrueStates)(emb_simapp, double[]);
	getTrueStates = (int (*)(emb_simapp, double[]))dlsym(handle, "getTrueStates");
	int resGTS = (int)getTrueStates(simappHandle, xtrue);
	if(resGTS == 0)
	{
		cout << "** (06/08) - getTrueStates():         passed **\r\n";
	}
	else
	{
		cout << "** (06/08) - getTrueStates():         failed **\r\n";
	}

	double ptrue[countParam];
	int (*getTrueParameters)(emb_simapp, double[]);
	getTrueParameters = (int (*)(emb_simapp, double[]))dlsym(handle, "getTrueParameters");
	int resGTP = (int)getTrueParameters(simappHandle, ptrue);
	if(resGTP == 0)
	{
		cout << "** (07/08) - getTrueParameters():     passed **\r\n";
	}
	else
	{
		cout << "** (07/08) - getTrueParameters():     failed **\r\n";
	}

	void (*freeSimApp)(emb_simapp);
	freeSimApp = (void (*)(emb_simapp))dlsym(handle, "freeSimApp");
	freeSimApp(simappHandle);
	cout << "** (08/08) - freeSimApp():            passed **\r\n";
	cout << "***********************************************\r\n";

	closeDynamicLibrary(handle);
}

void testInterfacesOfLibraries(char* libModel, char* libOptimizer, char* libObserver, char* libSimApp, int countUSimApp, int countMeasSimApp)
{
	testModelInterface(libModel);
	testOptimizerInterface(libOptimizer);
	testObserverInterface(libObserver);
	testSimAppInterface(libSimApp, countUSimApp, countMeasSimApp);
}

void testInterfacesOfLibraries(char* libOptModel, char* libObsModel, char* libOptimizer, char* libObserver, char* libSimApp, int countUSimApp, int countMeasSimApp)
{
	testModelInterface(libOptModel);
	testModelInterface(libObsModel);
	testOptimizerInterface(libOptimizer);
	testObserverInterface(libObserver);
	testSimAppInterface(libSimApp, countUSimApp, countMeasSimApp);
}



