/*!***************************************************************************************
\file       Sequencer.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true if all children succeed. Failure if one fails
*****************************************************************************************/
#pragma once
#include "Composite.hpp"

class Sequencer : public Composite
{
  public:

      void Update(float)override;

};


