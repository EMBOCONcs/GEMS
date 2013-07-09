//============================================================================
// Name        : InterfaceConnection.h
// Author      : Christian Schoppmeyer
// Version     :
// Created On  : 22.11.2011
// Copyright   : EMBOCON Copyright
// Description :
//============================================================================

#ifndef INTERFACECONNECTION_H_
#define INTERFACECONNECTION_H_

/**
 * Function to open a shared library located at the position provided in the path parameter.
 * Returns an handle to the library opened.
 */
void* loadDynamicLibrary(char* path);

/**
 * Function to close a shared library provided in the libHandle parameter.
 */
void closeDynamicLibrary(void* libHandle);

/**
 * Function to test whether all ModelInterface function calls are reachable.
 * Please note that this test function does not check whether useful implementation of the interface functions are provided.
 * The parameter path contains the location of the library which implements the ModelInterface.
 */
void testModelInterface(char* path);

/**
 * Function to test whether all OptimizerInterface function calls are reachable.
 * Please note that this test function does not check whether useful implementation of the interface functions are provided.
 * The parameter path contains the location of the library which implements the OptimizerInterface.
 */
void testOptimizerInterface(char* path);

/**
 * Function to test whether all ObserverInterface function calls are reachable.
 * Please note that this test function does not check whether useful implementation of the interface functions are provided.
 * The parameter path contains the location of the library which implements the ObserverInterface.
 */
void testObserverInterface(char* path);

/**
 * Function to test whether all ObserverInterface function calls are reachable.
 * Please note that this test function does not check whether useful implementation of the interface functions are provided.
 * The parameter path contains the location of the library which implements the ObserverInterface.
 */
void testSimAppInterface(char* path, int countU, int countMeas);

/**
 * Test all function calls in the different interfaces. The implementation in the libraries does not need to provide any useful calculations.
 */
void testInterfacesOfLibraries(char* libModel, char* libOptimizer, char* libObserver, char* libSimApp, int countUSimApp, int countMeasSimApp);
void testInterfacesOfLibraries(char* libOptModel, char* libObsModel, char* libOptimizer, char* libObserver, char* libSimApp, int countUSimApp, int countMeasSimApp);

#endif /* INTERFACECONNECTION_H_ */
