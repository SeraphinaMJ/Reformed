/*!***************************************************************************************
\file       BehaviorEnums.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This file contains enumeration definitions for the state of an Agent within
            a behavior tree.
*****************************************************************************************/
#pragma once

enum class BehaviorResult { SUCCESS = 0, RUNNING, FAILURE };

enum class BehaviorPhase
{
    STARTING, // we can indeed enter the node
    PROGRESSING, // node is in the middle of execution
    WAITING, // node is inactive
    DONE // node either succeeded or failed
};

enum class BehaviorType
{
    COMPOSITE, DECORATOR, LEAF
};


