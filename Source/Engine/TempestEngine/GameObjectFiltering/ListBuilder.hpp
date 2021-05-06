/*!***************************************************************************************
\file       ListBuilder.hpp
\author     Aaron Damyen
\date       5/29/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the ListBuilder class for creating lists of game objects for systems.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../SceneManagement/GameObject.hpp"

//========3rd Party Includes============================================================//

// forward declarations
class scene;

namespace GameObjectFiltering
{

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: listBuilder
\brief   Creates different collections of game objects that match a certain configuration
*****************************************************************************************/
class listBuilder final
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Creates a vector of game objects in the specified scene that match the specified
	        component pattern.
	\param p_pattern - The pattern the game objects must exhibit.
	\param p_scene - The scene to search within.
	\return std::vector<std::shared_ptr<gameObject>> - List of game objects found.
	*****************************************************************************************/
	std::vector<std::shared_ptr<gameObject>> buildForPattern(componentTypeSet const & p_pattern, std::shared_ptr<scene> const & p_scene);

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

} // namespace GameObjectFiltering

