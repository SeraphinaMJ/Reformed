/*!***************************************************************************************
\file       EnemyDeathEvent.hpp
\author     Ryan Booth
\date       12/4/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Enemy died, yo.
*****************************************************************************************/
#pragma once

#include <DataTypes.hpp>
#include "EventBase.hpp"

class EnemyDeathEvent : public EventSystem3::EventBase
{
public:
	EnemyDeathEvent() {}
};
