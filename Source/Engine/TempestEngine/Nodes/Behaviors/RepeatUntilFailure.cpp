/*!***************************************************************************************
\file       RepeatUntilFailure.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node that continually activates its child until 
            it fails.
*****************************************************************************************/
#include "RepeatUntilFailure.hpp"
#ifndef TESTING_NODES

void RepeatUntilFailure::Update(float dt)
{
    auto task = getTask();
    auto childResult = task->GetResult();

    // if child fails then mission accomplished
    if (childResult == BehaviorResult::FAILURE)
    {
        task->SetResult(BehaviorResult::SUCCESS);
        task->SetPhase(BehaviorPhase::DONE);
    }
    else if (childResult == BehaviorResult::SUCCESS)
    {
        // restart operation
        GiveToChild(task);
    }
}

#endif

