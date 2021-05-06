/*!***************************************************************************************
\file       Message.hpp
\author     Aaron Damyen
\date       8/12/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a structure for containing a message and its result
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <functional>
#include <string>
#include "EventBase.hpp"

//========Forward declarations==========================================================//
//struct message;
//========Typedefs======================================================================//
//typedef std::function<void(message &, unsigned long)> messageCompletedFunction;

////////==========================================================================////////
////////  Structures                                                              ////////
////////==========================================================================////////

/*!***************************************************************************************
\par struct: message
\brief   This is an envelope for containing a message, the source and destination and its result.
*****************************************************************************************/
//template <typename T>
struct message: public EventSystem3::EventBase
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	std::string m_sender; //!< Optional(empty): name of sending system
	std::string m_destination; //!< Required: name of destination system
	unsigned int m_message_code; //!< Optional(0): code defining this message's purpose
	void * m_data;
	//messageCompletedFunction m_completed_handler; //!< Required if m_expect_result == true, function to handle message response.

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};


