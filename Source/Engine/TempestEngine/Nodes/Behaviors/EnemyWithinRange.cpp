/*!***************************************************************************************
\file       EnemyWithinRange.cpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if enemy is within a certain predetermined
            distance.
*****************************************************************************************/
#include "EnemyWithinRange.hpp"
#include "Transform.hpp"
#include "Allegiance.hpp"
#include "GameObjectFilter.hpp"
#include "BehaviorSystem/BehaviorHelpers.hpp"

const char minRangeStr[] = "minimumRange";

const SpecialData EnemyWithinRange::specialData =
{
    {SpecialData::Entry{DATA_TYPE::FLOAT, "minimumRange", -1.0f}}
};

void EnemyWithinRange::Init()
{
    InitDebug();

    // get our actor
    auto actor = getActor();

    // get closest enemy
    auto closest_enemy = getClosestEnemy();
    if (!closest_enemy.expired())
    {
        getTask()->SetPhase(BehaviorPhase::DONE);
        getTask()->SetResult(BehaviorResult::FAILURE);
        return;
    }

    auto enemy_pos = BH::getPosition(closest_enemy);
    float distance = BH::GetDistance(actor, closest_enemy);

    bool enemyWithinRange = distance < this->minimumRange;

    // if closest enemy distance is within range
    if (enemyWithinRange)
    {
        Decorator::Init();
    }
    else
    {
        getTask()->SetPhase(BehaviorPhase::DONE);
        getTask()->SetResult(BehaviorResult::FAILURE);
    }

}

void EnemyWithinRange::Update(float)
{
    UpdateDebug();

    getTask()->SetPhase(BehaviorPhase::DONE);
}

// when reading from file this function will be called
// to set maxTime
void EnemyWithinRange::updateFromFile(const rapidjson::Value &value)
{
    this->minimumRange = value[minRangeStr].GetFloat();
}


