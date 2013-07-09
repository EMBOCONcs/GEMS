/*
 ******************************************************************************
 ***** EMBOCON OBSERVER INTERFACE *********************************************
 ******************************************************************************
 * EMBOCON_ObserverInterface.h
 *
 *  Created on: 10.10.2011
 *      Author: schoppmeyerc
 *      E-Mail: christian.schoppmeyer@bci.tu-dortmund.de
 *      Version: 1.0
 ******************************************************************************
 * The Observer Interface of the EMBOCON software platform connects the
 * supervisor with a selected observer package of an optimization module. Each
 * observer of an optimization package integrated in the EMBOCON platform has
 * to implement this interface to enable the connection to the supervisor.
 ******************************************************************************
 */

#ifndef EMBOCON_OBSERVERINTERFACE_H_
#define EMBOCON_OBSERVERINTERFACE_H_

#include "EMBOCON_DataTypes.h"
#include "EMBOCON_ModelInterface.h"
#include "stdlib.h"

/*
 * The emb_observer handle is used in all subsequent calls to identify
 * an working instance of the observer.
 */
typedef emb_handle emb_observer;

/*
 * The emb_obs_context handle is used in all subsequent calls to identify
 * an working instance of the observer data context.
 */
typedef emb_handle emb_obs_context;

/**
 * An instance of the observer is created by calling the createObserver() function.
 * This function returns a handle that identifies this observer instance. The model
 * that is used during the optimization is passed to the createObserver function.
 * It becomes the responsibility of the observer then to free the model when it is
 * no longer needed. No ways are foreseen to change the model for an observer, once
 * the observer has been created, so the model will need to be freed when the observer
 * itself is freed
 */
emb_observer createObserver(emb_model model);

/**
 * If the observer is no longer needed, it needs to be freed by calling the freeObserver() function. The
 * implementation needs to free up any resources associated with the observer. As mentioned in the
 * previous paragraph, this will include freeing the model that was passed into the createObserver() call
 * that created the observer.
 */
void freeObserver(emb_observer obs);

/**
 * It may be beneficial for the observer to keep the results of certain computations between one cycle and
 * the next. The Observer Interface has been designed to support this, but also allows the supervisor
 * to decide to rerun an optimization by introducing the observer context. The observer context is
 * represented by a handle and is to be treated by users of the observer as an opaque data structure.
 * This context must collect the internal observer state. This aspect of the observer context is discussed
 * further after the description of the cloneObsContext() function. An initial version of the context is
 * created by the initObsContext() call.
 */
emb_obs_context initObsContext(emb_observer obs);

/**
 * It may be beneficial for the observer to keep the results of certain computations between one cycle and
 * the next. The Observer Interface has been designed to support this, but also allows the supervisor
 * to decide to rerun an optimization by introducing the observer context. The observer context is
 * represented by a handle and is to be treated by users of the observer as an opaque data structure.
 * This context must collect the internal observer state. This aspect of the observer context is discussed
 * further after the description of the cloneObsContext() function. An initial version of the context is
 * created by the initObsContext() call.
 */
emb_obs_context cloneObsContext(emb_observer obs, emb_obs_context sourceContext);

/**
 * When an observer context is no longer needed, it must be freed by calling freeObsContext(). It is
 * an error if a context is freed more than once. The result of doing that is undefined. After a context
 * has been freed, it must not be used anymore. Failing to free an observer context may result in loss of
 * information, memory leaks or other undesirable situations.
 */
void freeObsContext(emb_observer obs, emb_obs_context obsContext);

/**
 * The getObsModelParameterCount(), getObsModelStateCount(), getObsModelInputCount() and
 * getObsModelOutputCount() calls return the number of parameters, states, inputs and outputs of the
 * model connected to the observer, respectively. The state count includes the differential states as well
 * as the algebraic states. These numbers will remain the same throughout the lifetime of the model
 * instance. The return values of these functions will be denoted np, nx, nu and ny in the descriptions
 * below. The return value is undefined if an invalid observer handle is passed for the obs argument.
 */
emb_size_type getObsModelParameterCount(emb_observer obs);

/**
 * The getObsModelParameterCount(), getObsModelStateCount(), getObsModelInputCount() and
 * getObsModelOutputCount() calls return the number of parameters, states, inputs and outputs of the
 * model connected to the observer, respectively. The state count includes the differential states as well
 * as the algebraic states. These numbers will remain the same throughout the lifetime of the model
 * instance. The return values of these functions will be denoted np, nx, nu and ny in the descriptions
 * below. The return value is undefined if an invalid observer handle is passed for the obs argument.
 */
emb_size_type getObsModelStateCount(emb_observer obs);

/**
 * The getObsModelParameterCount(), getObsModelStateCount(), getObsModelInputCount() and
 * getObsModelOutputCount() calls return the number of parameters, states, inputs and outputs of the
 * model connected to the observer, respectively. The state count includes the differential states as well
 * as the algebraic states. These numbers will remain the same throughout the lifetime of the model
 * instance. The return values of these functions will be denoted np, nx, nu and ny in the descriptions
 * below. The return value is undefined if an invalid observer handle is passed for the obs argument.
 */
emb_size_type getObsModelInputCount(emb_observer obs);

/**
 * The getObsModelParameterCount(), getObsModelStateCount(), getObsModelInputCount() and
 * getObsModelOutputCount() calls return the number of parameters, states, inputs and outputs of the
 * model connected to the observer, respectively. The state count includes the differential states as well
 * as the algebraic states. These numbers will remain the same throughout the lifetime of the model
 * instance. The return values of these functions will be denoted np, nx, nu and ny in the descriptions
 * below. The return value is undefined if an invalid observer handle is passed for the obs argument.
 */
emb_size_type getObsModelOutputCount(emb_observer obs);

/**
 * The getObsModelStateType() function can be used to distinguish algebraic states from differential
 * states. The caller should provide an array of int containing at least nx elements in the argument
 * is algebraic. The function will write 0 to the ith element of this array if the ith state element is not
 * an algebraic state and 1 if it is an algebraic state.
 */
int getObsModelStateType(emb_observer obs, int is_algebraic[]);

/**
 * The setObsModelParameter() function of the Observer Interface updates the parameter data of the
 * model with new values for the parameters. The input parameter p contains the new values of the
 * parameters of the model. The input parameter p_flag contains additional information about the
 * semantics of the parameters. The entries in p_flag can have the following different values: 0 - fixed,
 * 1 - free where free indicates that a parameter can be changed by the observer and fixed indicates
 * that a parameter can not be changed. The function returns 0 upon success. If the function fails, an
 * implementation-defined, non-zero error code is returned.
 *
 * The list of different values for the parameter flags used in the setObsModelParameter() function
 * can be extended by new entries encoding new requirements arising from the developments in work
 * packages 3 to 6.
 */
int setObsModelParameter(emb_observer obs, emb_obs_context obs_context, const double p[], const int p_flag[]);

/**
 * The setObsModelParameterUncertainty() function of the Observer Interface defines the parameter
 * uncertainty for the model parameters. The input parameter uncType contains the type of the parameter
 * uncertainty for each parameter in the model in an array of the type int. The entries in uncType
 * can have the following three different values: 0 - certain, 1 - deterministically uncertain, 2 -
 * statistically uncertain. The input parameters lower and upper define lower and upper bounds for
 * type-1 uncertain parameters. If none of the parameters are of type-1, the parameters lower and upper
 * may be NULL. These arrays must contain np parameters each. The input parameter conv contains an
 * np x np covariance matrix with dimensions related to the number of type-2 uncertain parameters in a
 * matrix of type double. See section 5.1 for the representation of a matrix in a single dimensional array.
 * The rows and columns in cov that correspond to parameters that are not of type-2 will be ignored by
 * the function. If none of the parameters is of type-2, cov can be NULL.
 *
 * The setObsModelParameterUncertainty() function returns 0 upon success. If the function fails, an
 * implementation-defined, non-zero error code is returned. The optimizer is not required to support
 * parameter uncertainties of a specific type or to treat them in a specific way. An implementation is
 * allowed to do nothing in this function and return success.
 */
int setObsModelParameterUncertainty(emb_observer obs, const int uncType[], const double lower[], const double upper[], const double cov[]);

/**
 * The getObsParameterCount() function returns the number of intrinsic observer parameters such as
 * tolerances and maximum iteration counts. This number will remain the same throughout the lifetime
 * of the observer instance. The value returned will be denoted nobsp in the descriptions below. The
 * return value is undefined if an invalid observer handle is passed for the obs argument.
 */
emb_size_type getObsParameterCount(emb_observer obs);

/**
 * The setObsParameter() function of the Observer Interface updates the intrinsic observer parameters
 * with new values. The input parameter obsp contains the new values of the parameters. The function
 * returns 0 upon success. If the function fails, an implementation-defined, non-zero error code is
 * returned.
 */
int setObsParameter(emb_observer obs, emb_obs_context obs_context, const double obsp[]);

/**
 * The makeObsStep() function of the Observer Interface triggers the observer to do the next observation
 * step. The input parameters ucur and ycur contain the measurements of the outputs and of the
 * controls of an application or simulation environment in the order given by the model. These are
 * the measurements of the system at the current sample time. The input parameter xprev contains
 * the previous values of the differential and algebraic states in the order expected by the model, see
 * section 5.2. Depending on the application, this may or may not differ from the values of the states
 * that the observer computed in the last cycle. The new observed values for the states are returned in
 * the output parameter xcur in the same order as used for the input parameter xprev. The obs context
 * parameter is both an input and an output parameter. Upon entry, it contains the internal state of the
 * observer before the current observation. It is updated during the call to makeStep to reflect the state
 * after the observation. The function returns 0 upon success. If the function fails, an implementation
 * defined, non-zero error code is returned.
 *
 * The observer context is updated in the call to makeStep(). If the user needs to be able to \rerun" an observation,
 * a copy of the observer context must be made that can be preserved. The cloneObsContext()
 * function does that: It makes a copy of the observer context. The copy is completely independent of
 * the original, i.e. changes made in one context will not be visible in the other.
 *
 * Suppose an observer context A is copied to a context B and then one or more calls to makeObsStep()
 * and/or setObsParameterUncertainty() are made using context A. If then the sequence of function
 * calls is repeated using context B, with all other input arguments to the functions being the same, all
 * output arguments of the functions must return the same values and all return values of the functions
 * must be the same as during the first sequence of calls that involved context A.
 *
 * For the copying and \rerunning" of an observation to be valid, the observer context needs to store all
 * model parameter and parameter uncertainty values.
 */
int makeObsStep(emb_observer obs, emb_obs_context obs_context, const double ucur[], const double ycur[], const double xprev[], double xcur[]);

/**
 * The function getObsStepDetails() can be used to provide additional details about an observation
 * run. Contrary to the optimizer, the observer does not return details over a prediction horizon, but
 * for the past and current sample times. \Current sample time" means the sample time at which
 * the measurements ucur and ycur were taken for the last call to makeObsStep. Not all observers use
 * information, other than the model state, from before the current sample time.
 *
 * The user can query the observer's estimate of the past \noise free" control values. An observer is not
 * required to treat controls as noisy, but those that do can return their estimates in uPast. Similarly to
 * getOptStepDetails(), uPast is interpreted as a matrix with pastHor rows and nu columns. The first
 * row corresponds to the current sample, the second to the sample before that and so on. This is different
 * from uPredict in getOptStepDetails(), where increasing row numbers correspond to increasing time.
 * If uPast is NULL upon entry to the function, no values are written to it. This can be used if the user
 * wants to get only some of the step details, but not the estimates of the past controls.
 *
 * The past horizon that has been used by the observer may be different from pastHor. In fact, many
 * observers use a horizon of only 1 sample. The function returns in the integer that actHor points to
 * how many samples it can return. This number is always written, even if uPast, yPast and xPast are
 * all NULL. It is an error if actHor is NULL.
 *
 * The estimates of the noise-free process outputs can be queried in the same way as the controls can be
 * queried. They are returned in the yPast argument instead of the uPast argument. The estimates for
 * the model state are queried through the xPast argument. These arguments may be NULL to specify
 * that the function does not need to return this information.
 *
 * An implementation is not required to store the information that can be queried in getObsStepDetails()
 * in the observer context. If any value is queried that is not available in the context, the function returns
 * 1 to signal the error. The function returns 0 upon success. If the function fails for another reason, an
 * implementation-defined error code is returned upon error.
 */
int getObsStepDetails(emb_observer obs, emb_obs_context obs_context, emb_size_type pastHor, emb_size_type *actHor, double uPast[], double yPast[], double xPast[]);

/**
 * The remaining five functions can be used to provide names for the model parameters, model states,
 * model inputs, model outputs and intrinsic observer parameters, respectively. An implementation is
 * not required to provide meaningful and/or unique names for these variables. A function that returns
 * only empty strings would be a trivial minimal implementation for these functions. Logging and tracing
 * functionality in the supervisor will likely benefit from getting meaningful names from these functions.
 *
 * The remainder of this description is for getObsModelParameterName(), but a similar description applies
 * for the other three functions. If implemented in the observer, the function call is passed through to the
 * Model Interface of the connected model to receive the corresponding values. It is up to the caller of the
 * function to provide a buffer to receive the requested name. The name is returned for a single parameter
 * at a time. Which parameter's name is returned is determined by the input argument name idx. This
 * is a value between 0 and np (inclusive). The size of the buffer is passed into the input argument
 * maxlen. If maxlen is zero, the par name argument is not touched. Otherwise, a string is written to
 * par name of no more than maxlen characters, including the terminating null character. If the actual
 * parameter name is longer than maxlen characters, the string is truncated. The truncated string will
 * still be terminated by a null character. The number of characters that are required to represent the
 * complete name, including the terminating null, is written to the output parameter reqlen. This can
 * be used to detect whether the parameter name was truncated. The function returns 0 upon success.
 *
 * If the function fails, an implementation-defined, non-zero error code is returned. It is not considered
 * a failure if the parameter name was truncated.
 */
int getObsModelParameterName(emb_observer obs, int name_idx, char par_name[], size_t maxlen, size_t *reqlen);

/**
 * See above.
 */
int getObsModelStateName(emb_observer obs, int name_idx, char x_name[], size_t maxlen, size_t *reqlen);

/**
 * See above.
 */
int getObsModelInputName(emb_observer obs, int name_idx, char u_name[], size_t maxlen, size_t *reqlen);

/**
 * See above.
 */
int getObsModelOutputName(emb_observer obs, int name_idx, char y_name[], size_t maxlen, size_t *reqlen);

/**
 * See above.
 */
int getObsParameterName(emb_observer obs, int name_idx, char par_name[], size_t maxlen, size_t *reqlen);

#endif /* EMBOCON_OBSERVERINTERFACE_H_ */
