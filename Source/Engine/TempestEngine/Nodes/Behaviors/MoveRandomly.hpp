/*!***************************************************************************************
\file       MoveRandomly.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This behavior just tells the npc to move in some random direction.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

#include "BehaviorSystem/BehaviorSpecialData.hpp"

class MoveRandomly : public Leaf
{

  float maxDistance;
  float minDistance;

  public:
  
  const static SpecialData specialData;

  void updateFromFile(const rapidjson::Value &) override;

  void Init()override;

  void Update(float) override;
};

