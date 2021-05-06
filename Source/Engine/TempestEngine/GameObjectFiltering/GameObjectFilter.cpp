/*!***************************************************************************************
\file       GameObjectFilter.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the GameObjectFilter class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "GameObjectFilter.hpp"
//========1st Party Includes============================================================//
#include "../SceneManagement/GameObject.hpp"
#include "../SceneManagement/Scene.hpp"
#include "../Engine.hpp"
//========3rd Party Includes============================================================//
#include <algorithm>
#include "../Components/ScriptCPP.hpp"

//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

namespace GameObjectFiltering
{

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

    void gameObjectFilter::moveGameObject(std::shared_ptr<gameObject> const & p_game_object, /*space const & p_containing_space,*/ componentTypeSet const & p_previous_componentTypeSet)
    {
        // remove from old list
        removeFrom(p_previous_componentTypeSet, p_game_object);

        // add to new list
        addTo(p_game_object->getComponentTypeSet(), p_game_object);
    }

    void gameObjectFilter::addTo(componentTypeSet p_target_componentTypeSet, std::shared_ptr<gameObject> p_game_object)
    {
        // caching disabled 
        // list_map[p_target_componentTypeSet].push_back(p_game_object);
    }

    void gameObjectFilter::removeFrom(componentTypeSet p_target_componentTypeSet, std::shared_ptr<gameObject> p_game_object)
    {
        // list map removed 
        // auto & l_vec = list_map[p_target_componentTypeSet];
        // // erase-remove idiom to remove object from the list
        // l_vec.erase(std::remove(l_vec.begin(), l_vec.end(), p_game_object), l_vec.end());
    }

    void gameObjectFilter::updateGameObject(std::shared_ptr<gameObject> p_game_object, componentTypeSet const & p_previous_componentTypeSet)
    {
        moveGameObject(p_game_object, /*containing_space,*/ p_previous_componentTypeSet);
    }

    icomponentRepository * gameObjectFilter::getComponentRepository()
    {
        return m_component_repository;
    }

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

    gameObjectFilter::gameObjectFilter(engine * p_engine, icomponentRepository * p_component_repository)
     : the_engine{p_engine}, m_component_repository{p_component_repository}, builder{nullptr}
    {}

	gameObjectFilter::~gameObjectFilter()
	{
		delete builder;
	}

	std::vector<std::shared_ptr<gameObject>> gameObjectFilter::getList(componentTypeSet const & p_pattern)
    {
      std::weak_ptr<scene> l_scene(the_engine->getCurrentScene());
			if (l_scene.expired())
				return std::vector<std::shared_ptr<gameObject>>();
			return getList(p_pattern, l_scene.lock());
    }

	std::vector<std::shared_ptr<gameObject>> gameObjectFilter::getList(
		GameObjectFiltering::componentTypeSet const& p_pattern, const std::shared_ptr<scene>& p_scene)
	{
        if (p_scene == nullptr)
            return std::vector<std::shared_ptr<gameObject>>();
        // build the list each time we need it.  (non-optimized method, but fully correct)
        return builder->buildForPattern(p_pattern, p_scene);
	}

void gameObjectFilter::setBuilder(listBuilder * p_new_builder)
    {
        builder = p_new_builder;
    }


/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

} // namespace GameObjectFiltering


