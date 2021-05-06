/*!***************************************************************************************
\file       GameObjectGatherer.hpp
\author     Aaron Damyen
\date       5/30/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This file defines the interface for a game object gatherer.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//

// forward declarations
class gameObject;
namespace GameObjectFiltering {
	class componentTypeSet;
}
class scene;
class icomponentRepository;

////////==========================================================================////////
////////  Classes/Interfaces                                                      ////////
////////==========================================================================////////

/*!***************************************************************************************
\par interface: gameObjectGatherer
\brief   This is the interface for the game object gatherer.
*****************************************************************************************/
class gameObjectGatherer
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Retrieves a list of game objects that fit the provided pattern
	\param p_pattern - The pattern of component types that must apply to returned game objects
	\return std::vector<std::shared_ptr<gameObject>> - List of game objects
	*****************************************************************************************/
	virtual std::vector<std::shared_ptr<gameObject>> getList(GameObjectFiltering::componentTypeSet const & p_pattern) = 0;
	/*!***************************************************************************************
	\brief  Retrieves a list of game objects that fit the provided pattern overriding the search scene.  Not recommended for typical use.
	\param p_pattern - The pattern of component types that must apply to returned game objects
	\param p_scene - The specific scene to search.
	\return std::vector<std::shared_ptr<gameObject>> - List of game objects
	*****************************************************************************************/
	virtual std::vector<std::shared_ptr<gameObject>> getList(GameObjectFiltering::componentTypeSet const & p_pattern, const std::shared_ptr<scene> & p_scene) = 0;
	
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




