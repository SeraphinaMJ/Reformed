/*!***************************************************************************************
\file       Repeater.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node restarts and processes its child node every time no matter what.
            Because of this it is highly suggested to use this as the root of the
            behavior tree.
*****************************************************************************************/
#include "Repeater.hpp"
#ifndef TESTING_NODES

void Repeater::Init()
{
    // set phase to ready for child
    Decorator::Init();
}

void Repeater::Update(float dt)
{
    UpdateDebug();
    // when child is done just restart
    getTask()->SetPhase(BehaviorPhase::STARTING);
}
#endif
