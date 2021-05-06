/*!***************************************************************************************
\file       IsLowHealth.hpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if npc has low health.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

#include "BehaviorSystem/BehaviorSpecialData.hpp"

class IsLowHealth : public Decorator
{
    int minimumHealth;
public:

    const static SpecialData specialData;

    // when reading from file this function will be called
    // to set maxTime
    void updateFromFile(const rapidjson::Value &) override;
    // END OF SERIALIZATION

    void Init()override;

    void Update(float) override;

};



