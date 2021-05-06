/*!***************************************************************************************
\file       Timer.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is a simple Decorator timer that activates its child node when
            timer hits 0.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

#include "BehaviorSystem/BehaviorSpecialData.hpp"

class Timer : public Decorator
{

    float maxTime;

public:

    const static SpecialData specialData;

    // when reading from file this function will be called
    // to set maxTime
    void updateFromFile(const rapidjson::Value &) override;

    void Init()override;

    void Update(float) override;
    
};

