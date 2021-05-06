/*!***************************************************************************************
\file       IsDead.hpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if npc is dead.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

class IsDead : public Decorator
{
  public:
  
    void Init() override;
    void Update(float) override;
    
};



