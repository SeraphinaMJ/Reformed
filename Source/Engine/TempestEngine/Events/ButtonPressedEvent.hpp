/*!***************************************************************************************
\file       ButtonPressedEvent.hpp
\author     Ryan Booth
\date       12/3/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include <DataTypes.hpp>
#include "EventBase.hpp"


class ButtonPressedEvent : public EventSystem3::EventBase
{
public:
  ButtonPressedEvent(std::string p_message, objID p_object) : m_message(p_message), object(p_object) {}
  std::string m_message;
  //data here
  objID object;
};
