/*
 ******************************************************************************
 ***** EMBOCON DATA TYPES *****************************************************
 ******************************************************************************
 * EMBOCON_DataTypes.h
 *
 *  Created on: 10.10.2011
 *      Author: schoppmeyerc
 *      E-Mail: christian.schoppmeyer@bci.tu-dortmund.de
 *      Version: 1.0
 ******************************************************************************
 */

#ifndef EMBOCON_DATATYPES_H_
#define EMBOCON_DATATYPES_H_

/**
 * The emb_size_type is used for functions returning counts of
 * parameters, state, elements etc. of the EMBOCON set of interfaces.
 */
typedef unsigned int emb_size_type;

/**
 * The emb_handle type is used as a handle. It is up to the implementation
 * of the interfaces to decided their actual interpretation. The users of
 * the interfaces use the handle to identify an object. This may mean that
 * the handle points to a data structure, but that is not necessarily the case.
 * Handles are created by a function like the createModel function of the
 * ModelInterface specified in D2.2. These handles must be destroyed using the
 * corresponding cleanup function, in this example the freeModel function.
 * Memory leaks and other resource leaks my occur if a handle is not disposed
 * by calling the proper cleanup function. A handle should not be cleaned up
 * more than once, and handle cleanup is the responsibility of the end-user.
 */
typedef void *emb_handle;


#endif /* EMBOCON_DATATYPES_H_ */
