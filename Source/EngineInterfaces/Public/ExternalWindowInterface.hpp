/*!***************************************************************************************
\file       ExternalWindowInterface.hpp
\author     Aaron Damyen
\date       8/10/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Defines the interface that all external windows must implement to communicate
        with the engine.
*****************************************************************************************/
#pragma once

#include <utility>

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//

////////==========================================================================////////
////////  Interfaces                                                              ////////
////////==========================================================================////////

/*!***************************************************************************************
\par interface: externalWindowInterface
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
class externalWindowInterface
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Notifies the external window of an opportunity to process its events.
	*****************************************************************************************/
	virtual void handleProcessEvents() = 0;
	/*!***************************************************************************************
	\brief  Notifies the external windows that the engine is requesting a resolution change.
	\param p_width - The requested width for the new resolution
	\param p_height - The requested height for the new resolution
	*****************************************************************************************/
	virtual void handleResizeRequest(int p_width, int p_height) = 0;
	/*!***************************************************************************************
	\brief  Notifies the external windows that something in the engine has requested that
	        the engine quit.  It is the responsibility of the implementer of this function
	        to actually stop the engine and shut it down.
	*****************************************************************************************/
	virtual void handleQuitRequest() = 0;

	/*!***************************************************************************************
	\brief  Notifies the external windows that something in the engine has requested that
			window toggles fullscreen. It is the responsibility of the implementer of this function
			to decide how to toggle fullscreen.
	*****************************************************************************************/
	virtual void handleFullscreenToggleRequest() = 0;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////
	virtual std::pair<int, int> getWindowDimentions() = 0;
	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};

