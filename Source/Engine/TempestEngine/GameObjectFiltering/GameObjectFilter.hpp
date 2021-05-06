/*!***************************************************************************************
\file       GameObjectFilter.hpp
\author     Aaron Damyen
\date       5/09/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the gameObjectFilter class and its public interface.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "ComponentTypeSet.hpp"
#include "../GameObjectCollector.hpp"
#include "../GameObjectGatherer.hpp"
#include "ListBuilder.hpp"
#include <ComponentRepository.hpp>


//========3rd Party Includes============================================================//

// forward declares
class gameObject;
class scene;
class engine;

namespace GameObjectFiltering
{

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: gameObjectFilter
\brief   This class manages data structures of GameObject references for the systems.  The lists
         are built as needed and cached.  Each list contains only the GameObjects that match
         the component type set specified.
*****************************************************************************************/
class gameObjectFilter : public gameObjectCollector, public gameObjectGatherer
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Allocates resources and associates this filter with the specified engine
	\param p_engine - Reference to the engine to use for accessing systems.
	*****************************************************************************************/
	gameObjectFilter(engine * p_engine, icomponentRepository * p_component_repository);
	/*!***************************************************************************************
	\brief  Releases all resources acquired.  Destroys the listBuilder.
	*****************************************************************************************/
	virtual ~gameObjectFilter();
	/*!***************************************************************************************
	\brief  Notification that a game object has changed from the previous component pattern to
	        a new one.
	\param p_game_object - The game object that has changed.
	\param p_previous_componentTypeSet - The previous component pattern of the game object.
	*****************************************************************************************/
	void moveGameObject(std::shared_ptr<gameObject> const & p_game_object, /*space const & p_containing_space,*/ componentTypeSet const & p_previous_componentTypeSet);
	/*!***************************************************************************************
	\brief  Assigns a different list builder instance to this filter.
	\param p_new_builder - Reference to the new list builder to use when creating lists.
	*****************************************************************************************/
	void setBuilder(listBuilder * p_new_builder);
	
	//======================================================================================//
	//         gameObjectCollector Interface Implementation                                 //
	//======================================================================================//
	/*!***************************************************************************************
	\brief  Notification that a game object has changed from the previous component pattern.
	\param p_game_object - The game object that has changed.
	\param p_previous_componentTypeSet - The previous component pattern of the game object.
	*****************************************************************************************/
	void updateGameObject(std::shared_ptr<gameObject> p_game_object, componentTypeSet const & p_previous_componentTypeSet) override;
	/*!***************************************************************************************
	\brief  Retrieve the component repository in this engine/simulation
	\return icomponentRepository * - Pointer to the component repository
	*****************************************************************************************/
	icomponentRepository * getComponentRepository() override;

	//======================================================================================//
	//         gameObjectGatherer Interface Implementation                                  //
	//======================================================================================//
	/*!***************************************************************************************
	\brief  Request to create a list of game objects that match the specified pattern.
	\param p_pattern - The component pattern the game objects must match.
	\return std::vector<std::shared_ptr<gameObject>> - List of matching game objects.
	*****************************************************************************************/
	std::vector<std::shared_ptr<gameObject>> getList(componentTypeSet const & p_pattern) override;
	/*!***************************************************************************************
	\brief  Retrieves a list of game objects that fit the provided pattern overriding the search scene.  Not recommended for typical use.
	\param p_pattern - The pattern of component types that must apply to returned game objects
	\param p_scene - The specific scene to search.
	\return std::vector<std::shared_ptr<gameObject>> - List of game objects
	*****************************************************************************************/
	std::vector<std::shared_ptr<gameObject>> getList(GameObjectFiltering::componentTypeSet const& p_pattern, const std::shared_ptr<scene>& p_scene) override;


	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:
	/*!***************************************************************************************
	\brief  Performs the action of adding a game object to lists matching the component pattern.
	\param p_target_componentTypeSet - The component pattern of the lists the game object should be
	       added to.
	\param p_game_object - The game object that should be added to the lists.
	*****************************************************************************************/
	void addTo(componentTypeSet p_target_componentTypeSet, std::shared_ptr<gameObject> p_game_object);
	/*!***************************************************************************************
	\brief  Performs the action of removing a game object from lists matching the component pattern.
	\param p_target_componentTypeSet - The component pattern of the lists the game object should
	       be removed from.
	\param p_game_object - The game object that should be removed from the lists.
	*****************************************************************************************/
	void removeFrom(componentTypeSet p_target_componentTypeSet, std::shared_ptr<gameObject> p_game_object);
	
	engine * the_engine; //!< Reference to the engine object
	icomponentRepository * m_component_repository; //!< Reference to the component repository
	
	listBuilder * builder = nullptr; //!< Reference to the list builder factory
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};

} // namespace GameObjectFiltering

