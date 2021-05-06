/*!***************************************************************************************
\file       IsSquadEngaged.cpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Return whether or not squad is engaged with an enemy.
*****************************************************************************************/
#include "IsSquadEngaged.hpp"
#include "Formations/Squad.hpp"

void IsSquadEngaged::Init()
{
    InitDebug();

    auto squad = getSquad();

    // if squad is fighting opponents
    if (!squad.expired())
    {
        if (squad.lock()->isEngaged())
        {
            // send to child
            Decorator::Init();
        }
        else
        {
            auto task = getTask();
            task->SetPhase(BehaviorPhase::DONE);
            task->SetResult(BehaviorResult::FAILURE);
        }
    }
    else
    {
        auto task = getTask();
        task->SetPhase(BehaviorPhase::DONE);
        task->SetResult(BehaviorResult::FAILURE);
    }
}

void IsSquadEngaged::Update(float)
{
    UpdateDebug();

    auto task = getTask();
    task->SetPhase(BehaviorPhase::DONE);
    
}



