/*!***************************************************************************************
\file       HasSquad.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This decorator returns whether or not this npc has a squad.
*****************************************************************************************/
#include "HasSquad.hpp"


void HasSquad::Init()
{
    auto squad = getSquad();
    
    if(!squad.expired())
    {
        Decorator::Init();
    }
    else
    {
        InitDebug();

        // if there is no squad then return failure
        auto task = getTask();
        
        // done and failed
        task->SetPhase(BehaviorPhase::DONE);
        task->SetResult(BehaviorResult::FAILURE);
    }
    
}

