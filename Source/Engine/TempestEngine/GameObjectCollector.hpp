/*!***************************************************************************************
\file       GameObjectCollector.hpp
\author     Aaron Damyen
\date       5/30/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of th einterface for the collector of game objects.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <memory>

// forward declarations
class gameObject;
namespace GameObjectFiltering {
	class componentTypeSet;
}
class icomponentRepository;

////////==========================================================================////////
////////  Classes/Interfaces                                                      ////////
////////==========================================================================////////

/*!***************************************************************************************
\par interface: gameObjectCollector
\brief   This interface is used for collection of game object details.
*****************************************************************************************/
class gameObjectCollector
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
    virtual ~gameObjectCollector() = default;

	/*!***************************************************************************************
	\brief  Notifies that a game object has changed its configuration
	\param p_game_object - the game object that has changed
	\param p_previous_componentTypeSet - the previous configuration of the game object
	*****************************************************************************************/
	virtual void updateGameObject(std::shared_ptr<gameObject> p_game_object, GameObjectFiltering::componentTypeSet const & p_previous_componentTypeSet) = 0;

	/*!***************************************************************************************
	\brief  Retrieve the component repository in this engine/simulation
	\return icomponentRepository * - Pointer to the component repository
	*****************************************************************************************/
	virtual icomponentRepository * getComponentRepository() = 0;
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};




