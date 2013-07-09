//============================================================================
// Name        : Messages.h
// Author      : Christian Schoppmeyer
// Version     :
// Created On  : 22.11.2011
// Copyright   : EMBOCON Copyright
// Description :
//============================================================================

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <iostream>

using namespace std;

/**
 * The GEMS welcome message.
 */
void putWelcomeMsg(ostream& str);

/**
 * The GEMS help message with the command line arguments.
 */
void putHelpMsg(ostream& str);

/**
 * The GEMS version info.
 */
void putVersionInfo(ostream& str);

/**
 * The wrong arguments message
 */
void putWrongArgsMsg(ostream& str);

#endif /* MESSAGES_H_ */
