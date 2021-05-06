/*!***************************************************************************************
\file       HasSquad.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This decorator returns whether or not this npc has a squad.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

class HasSquad : public Decorator
{
    public:
        void Init() override;
};

