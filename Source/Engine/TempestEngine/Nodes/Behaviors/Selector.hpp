/*!***************************************************************************************
\file       Selector.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true if one child succeeds, False if none succeed.
*****************************************************************************************/
#pragma once
#include "Composite.hpp"

#ifndef TESTING_NODES

class Selector : public Composite
{

protected:

    void Init() override;
    void Update(float) override;
};
#endif