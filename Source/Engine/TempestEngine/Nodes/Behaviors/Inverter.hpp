/*!***************************************************************************************
\file       Inverter.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node inverts the result of its child node.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"
#ifndef TESTING_NODES

class Inverter : public Decorator
{
  public:

      void Init()override;
      void Update(float)override;
};



#endif