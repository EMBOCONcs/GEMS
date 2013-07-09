/*
 ******************************************************************************
 ***** EMBOCON SIMULATIONAPPLICATION INTERFACE ********************************
 ******************************************************************************
 * EMBOCON_SimulationApplicationInterface.h
 *
 *  Created on: 10.10.2011
 *      Author: schoppmeyerc
 *      E-Mail: christian.schoppmeyer@bci.tu-dortmund.de
 *      Version: 1.0
 ******************************************************************************
 * The SimulationApplication Interface of the EMBOCON software platform
 * connects the supervisor with an application, or simulation environment. The
 * interface is used to pass measurements and parameters of the current state
 * of the application or simulation to the observer to calculate the state of
 * the system. New control inputs calculated by the optimizer are passed back
 * through the interface to initiate the next steps of the controlled system.
 * The connection to an application can be highly specific in terms of
 * implementation language and information passing. Therefore the
 * SimulationApplication Interface specification consists of two layers, the
 * top layer defines the information which should be exchanged via the interface
 * by specifying function calls, the bottom layer contains the actual
 * implementation of the function calls which could involve an application
 * specific programming language as well as additional data transformation and
 * wrapper functions. Due to the demand for customized implementations for
 * particular applications, the bottom level of the SimulationApplication
 * Interface will not be specified in more details here.
 *
 * The SimulationApplication Interface offers additional functions for the
 * connection of simulation environments to the software platform to get values
 * for the real states and parameters calculated in the simulation. This additional
 * information enables a better performance evaluation of both, the observer and
 * the optimizer. The model of the system connected to the simulation environment
 * can be used to measure plant-model-mismatch or for evaluation the optimizer
 * and observer performance on slightly changed models.
 ******************************************************************************
 */

#ifndef EMBOCON_SIMULATIONAPPLICATIONINTERFACE_H_
#define EMBOCON_SIMULATIONAPPLICATIONINTERFACE_H_

#include "EMBOCON_DataTypes.h"

/*
 * The emb_simapp handle is used in all subsequent calls to identify
 * an working instance of the simulation or application.
 */
typedef emb_handle emb_simapp;

/**
 * An instance of the simulation or application is created by calling the createSimApp() function. This
 * function returns a handle that identifies this simulation or application instance.
 */
emb_simapp createSimApp();

/**
 * If the simulation or application is no longer needed, it needs to be freed by calling the freeSimApp()
 * function. The implementation needs to free up any resources associated with the simulation or application.
 */
void freeSimApp(emb_simapp simapp);

/**
 * The initSimApp() function runs the initialization routine of the simulation or application.
 */
void initSimApp(emb_simapp simapp);

/**
 * The makeStep() function of the SimulationApplication Interface triggers the simulation or application
 * to do the next step applying the new control inputs. The input parameter ucur contains the new controls
 * for the application or simulation calculated by the last optimization step. These are the
 * controls for the current sample time. The new observed measurements are returned in the output
 * parameter meascur. The new observed parameter values are returned in the output parameter pcur.
 * The function returns 0 upon success. If the function fails, an implementation-defined, non-zero error
 * code is returned.
 */
int makeSimStep(emb_simapp simapp, const double ucur[], double meascur[], double pcur[]);

/**
 * The getSimParameterCount() and getSimStateCount(), calls return the number of parameters and states
 * of the simulation or application connected to the supervisor, respectively. The state count includes
 * the differential states as well as the algebraic states. These numbers will remain the same throughout
 * the lifetime of the simulation or application instance. The return values of these functions will be
 * denoted np and nx in the descriptions below. The return value is undefined if an invalid simulation or
 * application handle is passed for the simapp argument.
 */
emb_size_type getSimParameterCount(emb_simapp simapp);

/**
 *
 */
emb_size_type getSimStateCount(emb_simapp simapp);

/**
 * The getTrueStates() function of the SimulationApplication Interface returns the ideal calculated
 * states of the simulation in the simulation environment. The output parameter xtrue contains the
 * values of the differential and algebraic states of the simulated system in the order as expected by the
 * model, see section 5.2. This must be an array containing at least getSimStateCount() elements. The
 * function returns 0 upon success. If the true states cannot be calculated or returned by the simulation
 * environment, the function returns 1 to signal the error. If the function fails for another reason, an
 * implementation-defined, non-zero error code is returned.
 */
int getTrueStates(emb_simapp simapp, double xtrue[]);

/**
 * The getTrueParameters() function of the SimulationApplication Interface returns the ideal calculated+#
 * parameter values of the simulation in the simulation environment. The output parameter ptrue
 * contains the values of the parameters of the simulated system. This must be an array containing at
 * least getSimParameterCount() elements. The function returns 0 upon success. If the true parameter
 * values cannot be calculated or returned by the simulation environment, the function returns 1 to signal
 * the error. If the function fails for another reason, an implementation-defined, non-zero error code is
 * returned.
 */
int getTrueParameters(emb_simapp simapp, double ptrue[]);

/**
 * Test the library integration into GEMS
 */

int testfct(int i);

#endif /* EMBOCON_SIMULATIONAPPLICATIONINTERFACE_H_ */
