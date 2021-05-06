/*!***************************************************************************************
\file       DefaultLeaf.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a default leaf node for testing purposes. It always succeeds.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

class DefaultLeaf : public Leaf
{
  public:

      void Update(float dt)override;
  
};
