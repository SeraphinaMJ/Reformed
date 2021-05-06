/*!***************************************************************************************
\file       DefaultLeaf.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a default leaf node for testing purposes. It always succeeds.
*****************************************************************************************/
#include "DefaultLeaf.hpp"

void DefaultLeaf::Update(float dt)
{
    UpdateDebug();

    this->getTask()->SetPhase(BehaviorPhase::DONE);
    this->getTask()->SetResult(BehaviorResult::SUCCESS);
}
