/*!***************************************************************************************
\file       ReturnTrue.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true regardless of result of child.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"
#ifndef TESTING_NODES

class ReturnTrue : public Decorator
{
public:

    void Update(float) override;
};

#endif