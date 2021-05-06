/*!***************************************************************************************
\file       TestEvent.hpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include "EventBase.hpp"

class testEvent : public EventSystem3::EventBase
{
public:
	testEvent(std::string p_message, int p_int): m_message(p_message), m_int(p_int){}
	std::string m_message;
	int m_int;
};
