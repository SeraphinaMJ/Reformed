/*!***************************************************************************************
\file       EnemyWithinRange.hpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if enemy is within a certain predetermined
            distance.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"
#include <vector>

#include "BehaviorSystem/BehaviorSpecialData.hpp"

class EnemyWithinRange : public Decorator
{
    float minimumRange;

public:

    const static SpecialData specialData;

    // when reading from file this function will be called
    void updateFromFile(const rapidjson::Value &) override;

    void Init() override;

    void Update(float) override;

};



