/*!***************************************************************************************
\file       Sequencer.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true if all children succeed. Failure if one fails
*****************************************************************************************/
#include "Sequencer.hpp"

void Sequencer::Update(float dt)
{
    UpdateDebug();

    auto task = getTask();
    auto result = task->GetResult();
    
    // if our child succeeded then move onto next one
    if (result == BehaviorResult::SUCCESS)
    {
        // in each node we keep track of which child index we are using
        task->IncrementChildIndex();// childIndices.top()++;

        // if done with all child nodes then we succeeded
        if ((unsigned)task->GetChildIndex() == this->childNodes.size())
        {
            task->SetResult(BehaviorResult::SUCCESS);
            task->SetPhase(BehaviorPhase::DONE);
        }
        // if we are not done then we send it back down to the next child
        else
        {
            GiveToChild(task);
        }
    }
    else if (result == BehaviorResult::FAILURE)
    {
        task->SetResult(BehaviorResult::FAILURE);
        task->SetPhase(BehaviorPhase::DONE);
    }
}










