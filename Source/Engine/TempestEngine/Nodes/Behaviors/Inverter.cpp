
/*!***************************************************************************************
\file       Inverter.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node inverts the result of its child node.
*****************************************************************************************/
#include "Inverter.hpp"

void Inverter::Init()
{
    // sends task to child
    Decorator::Init();
}

void Inverter::Update(float dt)
{
    UpdateDebug();

    auto task = getTask();
    auto childResult = task->GetResult();

    // if child succeeds then we have failed
    if (childResult == BehaviorResult::SUCCESS)
    {
        task->SetResult(BehaviorResult::FAILURE);
        task->SetPhase(BehaviorPhase::DONE);
    }
    // child failed so return success
    else if(childResult == BehaviorResult::FAILURE)
    {
        task->SetResult(BehaviorResult::SUCCESS);
        task->SetPhase(BehaviorPhase::DONE);
    }
}
