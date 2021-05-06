/*!***************************************************************************************
\file       ReturnTrue.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true if one child succeeds, False if none succeed.
*****************************************************************************************/
#include "Selector.hpp"
#ifndef TESTING_NODES

void Selector::Update(float dt)
{
    UpdateDebug();

    auto task = getTask();
    auto childResult = task->GetResult();

    // if child succeeded then we succeeded
    if (childResult == BehaviorResult::SUCCESS)
    {
        task->SetResult(BehaviorResult::SUCCESS);
        task->SetPhase(BehaviorPhase::DONE);
    }
    if (childResult == BehaviorResult::FAILURE)
    {
        // if failed then go to next child
        task->IncrementChildIndex();
        
        // if no more child nodes left to try
        if ((unsigned)task->GetChildIndex() == childNodes.size())
        {
            task->SetResult(BehaviorResult::FAILURE);
            task->SetPhase(BehaviorPhase::DONE);
        }
        else
        {
            GiveToChild(task);
        }
    }
}

void Selector::Init()
{
    // set ourselves to being ready and progressing, child nodes
    // are also set as ready
    Composite::Init();
}

#endif