/*!***************************************************************************************
\file       PickEnemyToFight.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This behavior chooses who to fight from a list of enemies detected by the squad.
*****************************************************************************************/
#include "PickEnemyToFight.hpp"
#include "Formations/Squad.hpp"

void PickEnemyToFight::Update(float)
{
    UpdateDebug();
    // get squad
    auto squad = getSquad();

    // get enemy to fight
    GameObjectPtr opponent = this->getClosestEnemy();

    auto task = getTask();
    task->SetPhase(BehaviorPhase::DONE);

    // if we have an opponent
    if (!opponent.expired())
    {
        Blackboard& blackboard = getBlackboard();
        blackboard.setValue("Chosen Enemy", opponent.lock()->getID());

        task->SetResult(BehaviorResult::SUCCESS);
    }
    // no enemy assigned to us
    else
    {
        task->SetResult(BehaviorResult::FAILURE);
    }
}

