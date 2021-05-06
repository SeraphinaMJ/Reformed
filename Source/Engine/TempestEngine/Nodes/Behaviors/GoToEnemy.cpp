/*!***************************************************************************************
\file       GoToEnemy.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Pushes npc to the nearest enemy.
*****************************************************************************************/
#include "GoToEnemy.hpp"
#include "BehaviorSystem/BehaviorHelpers.hpp"
#include "RigidBody.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

#include "Formations/Squad.hpp"

const char* minDistStr = "Minimum Distance To Enemy";
const char* speedStr = "Max Speed";

const SpecialData GoToEnemy::specialData
{
    {
        {DATA_TYPE::FLOAT, minDistStr, -1.0f},
        {DATA_TYPE::FLOAT, speedStr, -1.0f}
    }
};

void GoToEnemy::updateFromFile(const rapidjson::Value &value)
{
    this->minDistToEnemy = value[minDistStr].GetFloat();
    auto it = value.FindMember(speedStr);
    this->maxSpeed = value[speedStr].GetFloat();
}

void GoToEnemy::Update(float dt)
{
    UpdateDebug();

    auto currSpace = this->getCurrentSpace();
    Blackboard& blackboard = getBlackboard();
    //auto target = BH::getGameObj(blackboard.getValue<objID>("Chosen Enemy"), currSpace);

    auto actor = this->getActor();
    if (actor == nullptr)
    {
        std::cout << "HOW";
    }

    // if target is still within detection radius
    auto squad = this->getSquad();
    if (squad.expired())
    {
        getTask()->SetPhase(BehaviorPhase::DONE);
        getTask()->SetResult(BehaviorResult::FAILURE);
        return;
    }

    // set target to closest enemy
    auto target = squad.lock()->requestOpponent(actor->getID());
    
    // if target is alive
    if (!target.expired() && BH::NPC_Is_Alive(target.lock()->getID(), currSpace))
    {
        // if hostile is no longer within detection
        if (squad.lock()->hasDetectedHostile(target.lock()->getID()) == false)
        {
            getTask()->SetPhase(BehaviorPhase::DONE);
            getTask()->SetResult(BehaviorResult::FAILURE);
        }
        

        assert(target.lock() != getActor());
        auto dist = BH::GetDistance(getActor(), target);

        const vector3& theirPos = target.lock()->getComponent<transform>()->getPosition();
        const vector3& ourPos = getActor()->getComponent<transform>()->getPosition();

        auto dirToTarget = theirPos - ourPos;
        dirToTarget.normalized(); // normalize vector

        getActor()->getComponent<rigidBody>()->setVelocity(dirToTarget * maxSpeed);

        if (dist <= this->minDistToEnemy)
        {
            getTask()->SetPhase(BehaviorPhase::DONE);
            getTask()->SetResult(BehaviorResult::SUCCESS);
        }
    }
    else
    {
        getTask()->SetPhase(BehaviorPhase::DONE);
        getTask()->SetResult(BehaviorResult::FAILURE);
    }
}




