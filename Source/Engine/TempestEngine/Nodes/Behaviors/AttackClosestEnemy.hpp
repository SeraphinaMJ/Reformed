/*!***************************************************************************************
\file       AttackClosestEnemy.hpp
\author     Charles Boudousquie
\date       11/4/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Leaf behavior that tells npc to attack closest enemy.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

#include "BehaviorSystem/BehaviorSpecialData.hpp"

// NOTE: this behavior expects "minimumRange" to be set by a node above it
class AttackClosestEnemy : public Leaf
{
    // check if enemy is still in range
    bool EnemyInRange(GameObjectPtr);

    // obviously an npc cannot attack another npc 60 times per second
    float timeBetweenAttacks;

    // how much damage the enemy will take
    float attackDamage;

    // how far our attacks can reach
    float attackRange;

public:

    static const SpecialData specialData;

    // when reading from file this function will be called
    // to set maxTime
    void updateFromFile(const rapidjson::Value &) override;

    void Init()override;
    void Update(float) override;

};


