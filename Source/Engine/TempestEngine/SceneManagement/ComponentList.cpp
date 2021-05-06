/*!***************************************************************************************
\file       ComponentList.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the ComponentList class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "ComponentList.hpp"
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include <GameObject.hpp>
#include <IComponentRepository.hpp>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//


componentList::componentList() : currentComponentTypeSet{GameObjectFiltering::componentTypeSet()}
{
}

componentList::~componentList()
{
	removeAllComponents();
}

GameObjectFiltering::componentTypeSet componentList::getComponentTypeSet() const
{
	return currentComponentTypeSet;
}

icomponentRepository* componentList::getRepository() 
{
	scene* scene_ptr = parentGameObject->getSpace()->getScene();
	return scene_ptr->getGameObjectCollector()->getComponentRepository();
}

void componentList::removeComponent(baseComponentHandle& p_handle)
{
	for(auto l_iter = map.begin(); l_iter != map.end(); ++l_iter)
	{
		if(l_iter->second == p_handle)
		{
      DEALLOCATE(getRepository(), map.begin()->second);
			map.erase(l_iter);
			break;
		}
	}
}

void componentList::removeAllComponents()
{
	while (!map.empty())
	{
    DEALLOCATE(getRepository(), map.begin()->second);
		map.erase(map.begin());
	}
	currentComponentTypeSet.reset();
}

std::vector<baseComponentHandle> componentList::getAllComponents() const
{
	std::vector<baseComponentHandle> l_result;

	// build list of all components
	for (auto l_iter = map.begin(); l_iter != map.end(); ++l_iter)
	{
		l_result.push_back(l_iter->second);
	}
	return l_result;
}

baseComponentHandle componentList::getComponentByString(std::string p_component_name)
{
	for (auto l_iter = map.begin(); l_iter != map.end(); ++l_iter)
	{
		std::string l_component_name(l_iter->first.name());
		if (l_component_name.find(p_component_name) != std::string::npos)
			return l_iter->second;
	}

	return nullptr;
}

std::vector<baseComponentHandle> componentList::getComponentsByString(std::string p_component_name)
{
	std::vector<baseComponentHandle> l_result;
	for (auto l_iter = map.begin(); l_iter != map.end(); ++l_iter)
	{
		std::string l_component_name(l_iter->first.name());
		if (l_component_name.find(p_component_name) != std::string::npos)
		{
			l_result.push_back(l_iter->second);
		}
	}

	return l_result;
}

baseComponentHandle componentList::getComponentByType(componentType p_type)
{
	for (auto l_iter = map.begin(); l_iter != map.end(); ++l_iter)
	{
		if (p_type == l_iter->first)
		{
			return  l_iter->second;
		}
	}

  // should be a nullptr
  baseComponentHandle defaultResult = nullptr;
  return defaultResult;
}

std::vector<baseComponentHandle> componentList::getComponentsByType(componentType p_type)
{
	std::vector<baseComponentHandle> l_result;
	for (auto l_iter = map.begin(); l_iter != map.end(); ++l_iter)
	{
		if (p_type <= l_iter->first)
		{
			l_result.push_back(l_iter->second);
		}
	}

	return l_result;
}

void componentList::buildCurrentComponentTypeSet()
{
	currentComponentTypeSet.reset();
	for(auto l_iter = map.begin(); l_iter != map.end(); ++l_iter)
	{
		currentComponentTypeSet.setType(l_iter->first);
	}
}


