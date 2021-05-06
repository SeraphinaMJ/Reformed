/*!***************************************************************************************
\file       ActionSystem.hpp
\author     Aaron Damyen
\date       6/13/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This file defines the actionSystem class.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../SystemBase.hpp"
#include "Action.hpp"

//========3rd Party Includes============================================================//
#include <functional>
#include <unordered_map>

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: actionSystem
\brief   This is the class definition for a simple action processing system.
*****************************************************************************************/
class actionSystem final : public systemBase
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Constructs and queues a LERP action, associating it with a parent action, if provided.
	\param p_start_value - The starting value of the interpolation.
	\param p_end_value - The ending value of the interpolation.
	\param p_duration - The total duration the interpolation should take, in seconds.
	\param p_update_function - The function that should be handling update events.
	\param p_parent_action - Optional parent to associate the new action to.
	\return actionHandle - Returns the handle to the new action.
	*****************************************************************************************/
	actionHandle enqueueLerpAction(float p_start_value, float p_end_value, float p_duration, std::function<void(float)> p_update_function, actionHandle p_parent_action = 0);
	/*!***************************************************************************************
	\brief  Constructs and queues a delay action, associating it with a parent action, if provided.
	\param p_delay - The amount of time to delay the action, in seconds.
	\param p_complete_function - The function to handle the complete event.
	\param p_parent_action - Optional parent to associate the new action to.
	\return actionHandle - Returns the handle to the new action.
	*****************************************************************************************/
	actionHandle enqueueDelayAction(float p_delay, std::function<void()> p_complete_function,actionHandle p_parent_action = 0);
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////
	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("actionSystem"); return n; }
	virtual const std::string& name() const override { return getName(); }

	////////========================================================================////////
	////////   Protected                                                            ////////
	////////========================================================================////////
protected:

	/*!***************************************************************************************
	\brief  Handles the update event from the system manager, updating all actions
	*****************************************************************************************/
	void onUpdate() override;
	/*!***************************************************************************************
	\brief  Handles the shutdown event from the system manager, shutting down this system.
	*****************************************************************************************/
	void onShutdown() override;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	/*!***************************************************************************************
	\brief  Helper method to remove an action from the actions list.
	\param p_action_handle - The handle of the action to remove.
	*****************************************************************************************/
	void removeAction(actionHandle p_action_handle);
	/*!***************************************************************************************
	\brief  Helper method to start all child actions for a specific action.
	\param p_parent_handle - The handle of the parent action.
	*****************************************************************************************/
	void startChildren(actionHandle p_parent_handle);
	/*!***************************************************************************************
	\brief  Retrieves a unique handle.
	\return actionHandle - a unique handle for an action.
	*****************************************************************************************/
	actionHandle getNextHandle();

    std::unordered_map<actionHandle, action *> actions; //!< The map of all actions known.
	/*! The map of dependencies between actions. */
    std::unordered_multimap<actionHandle /*parent*/, actionHandle /*child*/> dependencies;
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};

