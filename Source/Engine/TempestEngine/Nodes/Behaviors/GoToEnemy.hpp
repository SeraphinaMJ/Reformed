/*!***************************************************************************************
\file       GoToEnemy.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Pushes npc to the nearest enemy.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

#include "BehaviorSystem/BehaviorSpecialData.hpp"

#ifndef TESTING_NODES

class GoToEnemy : public Leaf
{
    
    float minDistToEnemy;
    float maxSpeed;
public:

    static const SpecialData specialData;
    void updateFromFile(const rapidjson::Value &) override;

    //void Init() override;
    void Update(float) override;
};
#endif
