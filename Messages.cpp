//============================================================================
// Name         : Messages.cpp
// Author       : Christian Schoppmeyer
// Version      :
// Created On   : 22.11.2011
// Copyright    : EMBOCON Copyright
// Description  :
// Last modified: 09.11.2012
//============================================================================

#include "Messages.h"

void putWelcomeMsg(ostream& str)
{
	str << "***********************************************\r\n"
			"***********************************************\r\n"
			"**    ______________  ________               **\r\n"
			"**   / ____/ ____/  |/  / ___/               **\r\n"
			"**  / / __/ __/ / /|_/ /\\__ \\                **\r\n"
			"** / /_/ / /___/ /  / /___/ /                **\r\n"
			"** \\____/_____/_/  /_//____/                 **\r\n"
			"**                                           **\r\n"
			"** GEMS - Generic EMBOCON Minimal Supervisor **\r\n"
			"** Version 0.32 - Build on 27.02.2013        **\r\n"
			"***********************************************\r\n"
			"***********************************************\r\n";

}

void putWrongArgsMsg(ostream& str)
{
	str << 	"** --WRONG ARGUMENTS--                       **\r\n"
			"***********************************************\r\n"
			"***********************************************\r\n";
}

void putHelpMsg(ostream& str)
{
	str << "** Help:                                     **\r\n"
			"***********************************************\r\n"
			"** -h for help.                              **\r\n"
			"***********************************************\r\n"
			"** -r <path_to_model_.so> <path_to_opt_.so>  **\r\n"
			"**    <path_to_obs_.so> <path_to_sim_.so>    **\r\n"
			"**    <steps count>                          **\r\n"
			"**    to run the supervisor routine using    **\r\n"
			"**    the same model for the optimizer and   **\r\n"
			"**    for the observer.                      **\r\n"
			"***********************************************\r\n"
			"** -r <path_to_opt_model_.so>                **\r\n"
			"**    <path_to_obs_model_.so>                **\r\n"
			"**    <path_to_opt_.so>                      **\r\n"
			"**    <path_to_obs_.so>                      **\r\n"
			"**    <path_to_sim_.so>                      **\r\n"
			"**    <steps count>                          **\r\n"
			"**    to run the supervisor routine using    **\r\n"
			"**    the same model for the optimizer and   **\r\n"
			"**    for the observer.                      **\r\n"
			"***********************************************\r\n"
			"** -t <path_to_model_.so> <path_to_opt_.so>  **\r\n"
			"**    <path_to_obs_.so> <path_to_sim_.so>    **\r\n"
			"**    countUSimApp countMeasSimApp           **\r\n"
			"**    to run connection test on interfaces.  **\r\n"
			"***********************************************\r\n"
			"** -t <path_to_opt_model_.so>                **\r\n"
			"**    <path_to_obs_model_.so>                **\r\n"
			"**    <path_to_opt_.so>                      **\r\n"
			"**    <path_to_obs_.so> <path_to_sim_.so>    **\r\n"
			"**    countUSimApp countMeasSimApp           **\r\n"
			"**    to run connection test on interfaces.  **\r\n"
			"***********************************************\r\n"
			"** -t 0 <path_to_model_.so> to run model     **\r\n"
			"**    interface connection test.             **\r\n"
			"***********************************************\r\n"
			"** -t 1 <path_to_optimizer_.so> to run       **\r\n"
			"**    optimizer interface connection test.   **\r\n"
			"***********************************************\r\n"
			"** -t 2 <path_to_observer_.so> to run        **\r\n"
			"**    observer interface connection test.    **\r\n"
			"***********************************************\r\n"
			"** -t 3 <path_to_simulationapplication_.so>  **\r\n"
			"**    countUSimApp countMeasSimApp           **\r\n"
			"**    to run simapp interface connection test**\r\n"
			"***********************************************\r\n"
			"***********************************************\r\n";
}

void putVersionInfo(ostream& str)
{
	str << "0.32";
}

