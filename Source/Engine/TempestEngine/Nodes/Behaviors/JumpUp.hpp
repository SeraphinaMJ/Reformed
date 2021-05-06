/*!***************************************************************************************
\file       JumpUp.hpp
\author     Charles Boudousquie
\date       10/18/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node makes the character jump upwards.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

class JumpUp : public Leaf
{
public:

    void Update(float) override;

};

