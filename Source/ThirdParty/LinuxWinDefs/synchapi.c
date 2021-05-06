/*!***************************************************************************************
\file       synchapi.c
\author     Tyler Robbins
\date       9/8/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Defines functions from synchapi.h.
\details See https://docs.microsoft.com/en-us/windows/win32/api/synchapi/ for
         more information.
*****************************************************************************************/

//========Self Include==================================================================//
#include "synchapi.h"

//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include <unistd.h> // usleep

//========Types=========================================================================//
//========Defines=======================================================================//

//========Forward Deceleration==========================================================//

/*!***************************************************************************************
\brief Suspends the execution of the current thread until the time-out interval elapses.

\param dwMilliseconds The time interval for which execution is to be suspended, in milliseconds.
*****************************************************************************************/
void Sleep(DWORD dwMilliseconds) {
    // TODO: Deprecated functino, we should use nanosleep instead
    usleep(dwMilliseconds * 1000);
}

