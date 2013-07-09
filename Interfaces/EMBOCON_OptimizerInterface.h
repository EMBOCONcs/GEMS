/*
 ******************************************************************************
 ***** EMBOCON OPTIMIZER INTERFACE ********************************************
 ******************************************************************************
 * EMBOCON_OptimizerInterface.h
 *
 *  Created on: 10.10.2011
 *      Author: schoppmeyerc
 *      E-Mail: christian.schoppmeyer@bci.tu-dortmund.de
 *      Version: 1.0
 ******************************************************************************
 * The Optimizer Interface of the EMBOCON software platform connects the
 * supervisor with a selected optimization package of an optimization module.
 * Each optimization package integrated in the EMBOCON platform has to
 * implement this interface to enable the connection to the supervisor.
 ******************************************************************************
 */

#ifndef EMBOCON_OPTIMIZERINTERFACE_H_
#define EMBOCON_OPTIMIZERINTERFACE_H_

#include "EMBOCON_DataTypes.h"
#include "EMBOCON_ModelInterface.h"
#include "stdlib.h"

/*
 * The emb_optimizer handle is used in all subsequent calls to identify
 * an working instance of the optimizer.
 */
typedef emb_handle emb_optimizer;

/*
 * The emb_opt_context handle is used in all subsequent calls to identify
 * an working instance of the optimizer data context.
 */
typedef emb_handle emb_opt_context;

/**
 * An instance of the optimizer is created by calling the createOptimizer() function.
 * This function returns a handle that identifies this optimizer instance. The model
 * that is used during the optimization is passed to the createOptimizer function.
 * It becomes the responsibility of the optimizer then to free the model when it is
 * no longer needed. No ways are foreseen to change the model for an optimizer, once
 * the optimizer has been created, so the model will need to be freed when the optimizer
 * itself is freed.
 */
emb_optimizer createOptimizer(emb_model model);

/**
 * If the optimizer is no longer needed, it must be freed by calling the freeOptimizer()
 * function. The user's implementation must also free up any resources associated with
 * the optimizer. As mentioned in the previous paragraph, this will include freeing the
 * model that was passed into the createOptimizer() call that created the optimizer.
 */
void freeOptimizer(emb_optimizer optim);

/**
 * It may be beneficial for the optimizer to keep the results of certain computations
 * between one cycle and the next. The Optimizer Interface has been designed to support
 * this, but also allows the supervisor to decide to rerun an optimization by introducing
 * the optimizer context. The optimizer context is represented by a handle and is to be
 * treated by users of the optimizer as an opaque data structure. This context must collect
 * the internal optimizer state. This aspect of the optimization context is discussed
 * further after the description of the cloneOptContext() function. An initial version of
 * the context is created by the initOptContext() call.
 */
emb_opt_context initOptContext(emb_optimizer optim);

/**
 * It may be beneficial for the optimizer to keep the results of certain computations
 * between one cycle and the next. The Optimizer Interface has been designed to support
 * this, but also allows the supervisor to decide to rerun an optimization by introducing
 * the optimizer context. The optimizer context is represented by a handle and is to be
 * treated by users of the optimizer as an opaque data structure. This context must collect
 * the internal optimizer state. This aspect of the optimization context is discussed
 * further after the description of the cloneOptContext() function. An initial version of
 * the context is created by the initOptContext() call.
 */
emb_opt_context cloneOptContext(emb_optimizer optim, emb_opt_context sourceContext);

/**
 * When an optimization context is no longer needed, it must be freed by calling
 * freeOptContext(). It is an error if a context is freed more than once. The result of
 * doing that is undefined. After a context has been freed, it must not be used anymore.
 * Failing to free an optimization context may result in loss of information, memory
 * leaks or other undesirable behavior.
 */
void freeOptContext(emb_optimizer optim, emb_opt_context optContext);

/**
 * The getOptModelParameterCount() call returns the number of parameters of the
 * model connected to the optimizer, respectively. The number will remain the same
 * throughout the lifetime of the model instance. The return value of this function
 * will be denoted np in the descriptions below. The return value is undefined
 * if an invalid optimizer handle is passed for the optim argument.
 */
emb_size_type getOptModelParameterCount(emb_optimizer optim);

/**
 * The getOptModelStateCount() call returns the number of states of the
 * model connected to the optimizer, respectively. The state count includes
 * the differential states as well as the algebraic states. The number will remain
 * the same throughout the lifetime of the model instance. The return value of this function
 * will be denoted nx in the descriptions below. The return value is undefined
 * if an invalid optimizer handle is passed for the optim argument.
 */
emb_size_type getOptModelStateCount(emb_optimizer optim);

/**
 * The getOptModelInputCount() call returns the number of parameters of the
 * model connected to the optimizer, respectively. The number will remain the same
 * throughout the lifetime of the model instance. The return value of this function
 * will be denoted nu in the descriptions below. The return value is undefined
 * if an invalid optimizer handle is passed for the optim argument.
 */
emb_size_type getOptModelInputCount(emb_optimizer optim);

/**
 * The getOptModelOutputCount() call returns the number of parameters of the
 * model connected to the optimizer, respectively. The number will remain the same
 * throughout the lifetime of the model instance. The return value of this function
 * will be denoted ny in the descriptions below. The return value is undefined
 * if an invalid optimizer handle is passed for the optim argument.
 */
emb_size_type getOptModelOutputCount(emb_optimizer optim);

/**
 * The getOptModelStateType() function can be used to distinguish algebraic states from
 * differential states. The caller should provide an array of int containing at least nx
 * elements in the argument is algebraic. The function will write 0 to the ith element
 * of this array if the ith state element is not an algebraic state and 1 if it is an
 * algebraic state.
 */
int getOptModelStateType(emb_optimizer optim, int is_algebraic[]);

/**
 * The setOptModelParameter() function of the Optimizer Interface updates the parameter
 * data of the model with new values for the parameters. The input parameter p contains
 * the new values of the parameters of the model. The input parameter p flag contains
 * additional information about the semantics of the parameters. The entries in p flag
 * can have the following different values: 0 - fixed, 1 - free where free indicates that
 * a parameter can be changed by the optimizer and fixed indicates that a parameter cannot
 * be changed. The function returns 0 upon success. If the function fails, an
 * implementation-defined, non-zero error code is returned.
 */
int setOptModelParameter(emb_optimizer optim, emb_opt_context opt_context, const double p[], const int p_flag[]);

/**
 * The setOptModelParameterUncertainty() function of the Optimizer Interface defines the
 * parameter uncertainty for the model parameters. The input parameter uncType contains
 * the type of the parameter uncertainty for each parameter in the model in an array of
 * the type int. The entries in uncType can have the following three different values:
 * 0 - certain,
 * 1 - deterministically uncertain,
 * 2 - statistically uncertain.
 * The input parameters lower and upper define lower and upper bounds for type-1 uncertain
 * parameters. If none of the parameters are of type-1, the parameters lower and upper may
 * be NULL. These arrays must contain np parameters each. The input parameter conv contains an
 * np x np covariance matrix with dimensions related to the number of type-2 uncertain
 * parameters in a matrix of type double. See section 5.1 for the representation of a matrix
 * in a single dimensional array. The rows and columns in cov that correspond to parameters
 * that are not of type-2 will be ignored by the function. If none of the parameters is of
 * type-2, cov can be NULL.
 *
 * The setOptModelParameterUncertainty() function returns 0 upon success. If the function fails,
 * an implementation-defined, non-zero error code is returned. The optimizer is not required to
 * support parameter uncertainties of a specific type or to treat them in a specific way. An
 * implementation is allowed to do nothing in this function and return success.
 */
int setOptModelParameterUncertainty(emb_optimizer optim, const int uncType[], const double lower[], const double upper[], const double cov[]);

/**
 * The getOptParameterCount() function returns the number of intrinsic optimizer and optimization
 * parameters such as tolerances and maximum iteration counts. This number will remain the same
 * throughout the lifetime of the optimizer instance. The value returned will be denoted noptp in
 * the descriptions below. The return value is undefined if an invalid optimizer handle is passed
 * for the optim argument.
 */
emb_size_type getOptParameterCount(emb_optimizer optim);

/**
 * The setOptParameter() function of the Optimizer Interface updates the intrinsic optimizer and
 * optimization parameters with new values. The input parameter optp contains the new values of the
 * parameters. The function returns 0 upon success. If the function fails, an implementation-defined,
 * non-zero error code is returned.
 */
int setOptParameter(emb_optimizer optim, emb_opt_context opt_context, const double optp[]);

/**
 * The makeOptStep() function of the Optimizer Interface triggers the optimizer to do the next optimization
 * step. The input parameter xcur contains the differential and algebraic states determined by the
 * observer in the order expected by the model, see section 5.2. It is the model state at the current sample
 * time. Typically, this state has been determined by the observer, based on the latest measurements
 * of the outputs and of the controls. The input parameter uprev contains the previous values of the
 * controls. Depending on the application, this may or may not differ from the values of the controls that
 * the optimizer computed in the last cycle. The new optimized values for the controls are returned in the
 * output parameter ucur. The opt context parameter is both an input and an output parameter. Upon
 * entry, it contains the internal state of the optimizer before the current optimization. It is updated
 * during the call to makeOptStep to reflect the state after the optimization. The function returns 0 upon
 * success. If the function fails, an implementation-defined, non-zero error code is returned.
 *
 * The optimization context is updated in the call to makeOptStep(). If the user needs to be able to
 * "rerun" an optimization, a copy of the optimization context must be made that can be preserved.
 *
 * The cloneOptContext() function does exactly that; it makes a copy of the optimization context. The
 * copy is completely independent of the original, i.e. changes made in one context will not be visible in
 * the other.
 *
 * Suppose an optimization context A is copied to a context B and then one or more calls to makeOptStep()
 * and/or setOptParameterUncertainty() are made using context A. If then the sequence of function calls
 * is repeated using context B, with all other input arguments to the functions being the same, all output
 * arguments of the functions must return the same values and all return values of the functions must
 * be the same as during the first sequence of calls that involved context A.
 *
 * For the copying and "rerunning" of an optimization to be valid, the optimization context needs to
 * store all model parameter and parameter uncertainty values.
 */
int makeOptStep(emb_optimizer optim, emb_opt_context opt_context, const double xcur[], const double uprev[], double ucur[]);

/**
 * The function getOptStepDetails() can be used to provide additional details about an optimization
 * run. This information is retrieved from the optimization context that is passed into the function in
 * the opt context argument. The results returned will therefore apply to the last call to makeOptStep()
 * that updated the optimization context.
 *
 * The user can query the current and future values that the optimization returned for the controls.
 * These are returned in uPredict. The number of controls is determined by the number of inputs of the
 * model. That number is denoted nu. The number of samples per input will be equal to predHor. The
 * first sample of a control will have the same value as was returned in ucur in the call to makeStep()
 * that last updated the context. The second sample will be one sample time later, and so on. Note that
 * the input sequence does not necessarily change at every sample. uPredict is interpreted as a matrix
 * with predHor rows and nu columns. The first column contains all samples for the first control, the
 * second for the second control and so on. If uPredict is NULL upon entry to the function, no values are
 * written to it. This can be used if the caller wants to get only some of the step details, but not the
 * predictions for the controls.
 *
 * The prediction horizon that has been used by the optimizer may have been different from predHor.
 * The function returns in the integer that actHor points to how many samples it can return. This
 * number is always written, even if all of uPredict, yPredict and xPredict are NULL. An error will be
 * returned if actHor is NULL.
 *
 * The predictions for the model outputs can be queried in the same way as the controls can be queried.
 * They are returned in the yPredict argument instead of the uPredict argument. The predictions for
 * the model state are queried through the xPredict argument. These arguments may be NULL to specify
 * that the function does not need to return this information.
 *
 * An implementation is not required to store the information that can be queried in getOptStepDetails()
 * in the optimization context. If any value is queried that is not available in the context, the function
 * returns 1 to signal the error. The function returns 0 upon success. An implementation-dened error
 * code is returned upon error.
 *
 * If the value of *actHor is less than predHor, only the rst *actHor rows of uPredict, yPredict and/or
 * xPredict are written. This is not considered an error, so the function may still return 0. If *actHor is
 * greater than predHor, only the rst predHor rows are written.
 *
 */
int getOptStepDetails(emb_optimizer optim, emb_opt_context opt_context, emb_size_type predHor, emb_size_type *actHor, double uPredict[], double yPredict[], double xPredict[]);

/**
 * The remaining five functions can be used to provide names for the model parameters, model states,
 * model inputs, model outputs and intrinsic optimizer and optimization parameters, respectively. An
 * implementation is not required to provide meaningful and/or unique names for these variables. A
 * function that returns only empty strings would be a trivial minimal implementation for these functions.
 * Logging and tracing functionality in the supervisor will likely benefit from getting meaningful names
 * from these functions.
 *
 * The remainder of this description is for getOptModelParameterName(), but a similar description applies
 * for the other three functions. If implemented in the optimizer, the function call is passed through
 * to the Model Interface of the connected model to receive the corresponding values. It is up to the
 * caller of the function to provide a buffer to receive the requested name. The name is returned for a
 * single parameter at a time. Which parameter's name is returned is determined by the input argument
 * name idx. This is a value between 0 and np (inclusive). The size of the buffer is passed into the
 * input argument maxlen. If maxlen is zero, the par name argument is not touched. Otherwise, a string is
 * written to par name of no more than maxlen characters, including the terminating null character. If the
 * actual parameter name is longer than maxlen characters, the string is truncated. The truncated string
 * will still be terminated by a null character. The number of characters that are required to represent the
 * complete name, including the terminating null, is written to the output parameter reqlen. This can
 * be used to detect whether the parameter name was truncated. The function returns 0 upon success.
 * If the function fails, an implementation-dened, non-zero error code is returned. It is not considered
 * a failure if the parameter name was truncated.
 */
int getOptModelParameterName(emb_optimizer optim, int name_idx, char par_name[], size_t maxlen, size_t *reqlen);

/**
 * See above.
 */
int getOptModelStateName(emb_optimizer optim, int name_idx, char x_name[], size_t maxlen, size_t *reqlen);

/**
 * See above.
 */
int getOptModelInputName(emb_optimizer optim, int name_idx, char u_name[], size_t maxlen, size_t *reqlen);

/**
 * See above.
 */
int getOptModelOutputName(emb_optimizer optim, int name_idx, char y_name[], size_t maxlen, size_t *reqlen);

/**
 * See above.
 */
int getOptParameterName(emb_optimizer optim, int name_idx, char par_name[], size_t maxlen, size_t *reqlen);

#endif /* EMBOCON_OPTIMIZERINTERFACE_H_ */
