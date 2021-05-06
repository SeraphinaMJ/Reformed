/*!***************************************************************************************
\file       IsSquadEngaged.hpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Return whether or not squad is engaged with an enemy.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

class IsSquadEngaged : public Decorator
{
    public:
        void Init() override;
        void Update(float) override;
};