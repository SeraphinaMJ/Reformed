/*!***************************************************************************************
\file       BehaviorTask.cpp
\author     Charles Boudousquie
\date       10/17/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is a data structure that keeps track of an agent's state within
            an instance of a BehaviorTree.
*****************************************************************************************/
#include "BehaviorTask.hpp"
#include "BehaviorTree.hpp"

#include "../Nodes/Behaviors/Behavior.hpp"

bool BehaviorTask::NoHistory()
{
    return history.empty();
}

void BehaviorTask::Push_State(BehaviorState state)
{
    history.push(state);
}

void BehaviorTask::Pop_State()
{
    history.pop();
}

void BehaviorTask::IncrementChildIndex()
{
    history.top().childIndex++;
    //currentChildStack.top().++;
}

int BehaviorTask::GetChildIndex()
{
    return history.top().childIndex;
}

void BehaviorTask::SetResult(BehaviorResult r)
{
    result = r;
}

BehaviorResult BehaviorTask::GetResult()
{
    return result;
}

// usually want to set phase during update() or exit()
void BehaviorTask::SetPhase(BehaviorPhase p)
{
    history.top().phase = p;
}

BehaviorPhase BehaviorTask::GetPhase()
{
    return history.top().phase;
}

void BehaviorTask::SetChildBehavior(Behavior* b)
{
    childBehavior = b;
}

Behavior* BehaviorTask::GetChildBehavior()
{
    return childBehavior;
}

void BehaviorTask::SetParentBehavior(Behavior* b)
{
    parentBehavior = b;
}

Behavior* BehaviorTask::GetParentBehavior()
{
    return parentBehavior;
}

void BehaviorTask::SetCurrentBehavior(Behavior* b)
{
    currentBehavior = b;
}

Behavior* BehaviorTask::GetCurrentBehavior()
{
    return currentBehavior;
}

std::string BehaviorTask::GetTreeName()
{
    return treeName;
}

void BehaviorTask::ClearHistory()
{
    history = {};
}

void BehaviorTask::RegisterNewNode(int id)
{
    BehaviorState newState(id, 0, BehaviorPhase::STARTING);
    history.push(newState);
}

