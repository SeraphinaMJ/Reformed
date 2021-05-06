/*!***************************************************************************************
\file       ActorDatabase.hpp
\author     Charles Boudousquie
\date       10/26/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the class keeps track of which actor has been selected by the
            Behavior Tree Manager
*****************************************************************************************/
#include "BehaviorTreeDataBase.hpp"

#include "BehaviorTreeManager.hpp"

void ActorDatabase::AssignCurrentActor(GameObjectPtr actor)
{
    this->currentActor = actor;
}

GameObjectPtr ActorDatabase::GetCurrentActor()
{
    return currentActor;
}

void ActorDatabase::SetManager(BehaviorTreeManager * manager)
{
    this->btm = manager;
}

BehaviorTreeManager* ActorDatabase::GetManager()
{
    return this->btm;
}


