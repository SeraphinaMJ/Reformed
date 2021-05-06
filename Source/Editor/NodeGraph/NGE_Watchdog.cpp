/*!***************************************************************************************
\file       NGE_Watchdog.cpp
\author     Charles Boudousquie
\date       10/23/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Checks to see if user changed something in inspector window that Node
            Graph Window would want to know about.
*****************************************************************************************/
#include "NGE_Watchdog.hpp"


void NGE_Watchdog::Reset()
{
    changedActor = -1;
    inspectorChanged = false;
}

bool NGE_Watchdog::inspectorChangedTree()
{
    return inspectorChanged;
}

void NGE_Watchdog::Alert(objID actor)
{
    changedActor = actor;
    inspectorChanged = true;
}

objID NGE_Watchdog::getChangedAgent()
{
    return changedActor;
}
