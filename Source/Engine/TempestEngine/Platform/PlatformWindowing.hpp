/*!***************************************************************************************
\file       PlatformWindowing.hpp
\author     Aaron Damyen
\date       8/10/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the system which communicates with the external window creation and external
        rendering context creation.  Note: This system doesn't actually create the window
        or the rendering context.  That responsibility lies outside the engine, but this
        system helps facilitate communication between the external structures and the engine.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../SystemBase.hpp"

//========3rd Party Includes============================================================//
class externalWindowInterface;
class renderContextInterface;

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: platformWindowing
\brief  This system sets up communication between the externally created window and the
        externally created rendering context and the engine.  This class setups the
        rendering context for each frame.
*****************************************************************************************/
class platformWindowing final : public systemBase
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("platformWindowing"); return n; }
	virtual const std::string& name() const override { return getName(); }

	/*!***************************************************************************************
	\brief  Constructs this system.  Required references to the external window and render
	        context.
	\param p_window_ptr - Pointer to an object that implements the externalWindowInterface interface.
	\param p_render_context_ptr - Pointer to an object that implements the renderContextInterface interface.
	*****************************************************************************************/
	platformWindowing(externalWindowInterface * p_window_ptr, renderContextInterface * p_render_context_ptr);

	std::pair<int, int> getWindowDimentions();

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	void presentFrame();

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
protected:

	/*!***************************************************************************************
	\brief  Handles the start frame event to clear the screen and prepare for rendering
	*****************************************************************************************/
	void onStartFrame() override;
	/*!***************************************************************************************
	\brief  Handles the update event to do nothing.
	*****************************************************************************************/
	//void onUpdate() override;
	/*!***************************************************************************************
	\brief  Handles the end frame event to force the render context buffer swap
	*****************************************************************************************/
	void onEndFrame() override;
	/*!***************************************************************************************
	\brief  Handles the shutdown event to do nothing.
	*****************************************************************************************/
	//void onShutdown() override;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	/*!***************************************************************************************
	\brief  Pointer to the object implementing the externalWindowInterface interface.
	*****************************************************************************************/
	externalWindowInterface * m_window_ptr = nullptr;
	/*!***************************************************************************************
	\brief  Pointer to the object implementing the renderContextInterface interface.
	*****************************************************************************************/
	renderContextInterface * m_render_context_ptr = nullptr;
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};

