/*!***************************************************************************************
\file       ComponentList.hpp
\author     Aaron Damyen
\date       5/14/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of a helper class for managing a list of components.
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Component.hpp"
#include "../GameObjectFiltering/ComponentTypeSet.hpp"

//======== 3rd Party Includes ==========================================================//
#include <unordered_map>
#include <IComponentRepository.hpp>

//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class component;
class gameObject;


/*!***************************************************************************************
\par class: componentList
\brief   This is a helper class for managing a list of components.
*****************************************************************************************/
class componentList final
{
public:

	/*!***************************************************************************************
	\brief  Creates a component list and initializes it.
	*****************************************************************************************/
	componentList();
	/*!***************************************************************************************
	\brief  Destroys a component list.  This does not destroy and components contained within.
	*****************************************************************************************/
	virtual ~componentList();

	/*!***************************************************************************************
	\brief  Retrieves the componentTypeSet representing the current component make-up of this list
	\return componentTypeSet - The pattern of components in this list.
	*****************************************************************************************/
	GameObjectFiltering::componentTypeSet getComponentTypeSet() const;

	template<typename T>
	std::pair<componentHandle<T>, bool> add()
	{
		componentHandle<T> l_comp = 
			ALLOCATE(getRepository(), T);
		return std::make_pair(l_comp, insert<T>(l_comp));
	}

	/*!***************************************************************************************
	\brief  Removes a component from the list.
	\param p_component - Pointer to the component to be removed
	\return bool - True if the topography changed, false otherwise
	*****************************************************************************************/
	template<typename T>
	bool remove(componentHandle<T> p_component)
	{
		componentType l_type = p_component->type();
		auto l_range = map.equal_range(l_type);

		// check if that type was present
		if (l_range.first == map.end())
			return false;  // no topography changes

						   // find the component instance
		while (l_range.first != l_range.second)
		{
			if (((*(l_range.first)).second) == p_component)
			{
				// found it, remove it
        DEALLOCATE(getRepository(), ((*(l_range.first)).second));
				map.erase(l_range.first);
				break; // can only remove the first one
			}
		}

		// if there are no others of the same type
		l_range = map.equal_range(l_type);

		if (l_range.first == map.end())
		{
			// rebuild the currentComponentTypeSet
			buildCurrentComponentTypeSet();
			return true;
		}

		return false; // no topography changes
	}

	/*!***************************************************************************************
	\brief  Retireves the first component of the specified type
	\return T * - Pointer to the first component of the specified type
	*****************************************************************************************/
	// TODO(coseo) this should have a list of component handles internally...
	template<typename T>
	componentHandle<T> getComponent()
	{
		// find a component of the specified type
		componentType l_type = T::getType();
		auto l_iter = map.find(l_type);

		if (l_iter != map.end())
		{
			// return first found

      return castHandle<T>(l_iter->second);
			// old : 
      // return (*static_cast<componentHandle<T> *>(l_iter->second));
		}

		// none found,
		return componentHandle<T>();
	}

	/*!***************************************************************************************
	\brief  Retrieves all components in the list
	\return std::vector<component *> - List of all components in the list.
	*****************************************************************************************/
	std::vector<baseComponentHandle> getAllComponents() const;

	void removeComponent(baseComponentHandle& p_handle);
	
	/*!***************************************************************************************
	\brief  Properly removes all components from the list
	*****************************************************************************************/
	void removeAllComponents();

	/*!*******************************************************************************
	\brief  Retrieves the first component by its internal name
	\return component * - pointer to the first component with the provided name
	*********************************************************************************/
	baseComponentHandle getComponentByString(std::string p_component_name);
	std::vector<baseComponentHandle> getComponentsByString(std::string p_component_name);

	baseComponentHandle getComponentByType(componentType p_type);
	std::vector<baseComponentHandle> getComponentsByType(componentType p_type);

	/*!*******************************************************************************
	\brief  Retrieves the number of components of a particular type in this list
	\return unsigned int - the number of components
	*********************************************************************************/
	template<typename T>
	unsigned int getComponentCount() const
	{
		// find all components of the specified type
		componentType l_type = T::getType();
		auto l_range = map.equal_range(l_type);

		// verify there were any
		if (l_range.first == map.end())
			return 0;

		unsigned int l_count = 0;
		for (; l_range.first != l_range.second; ++l_range.first)
		{
			++l_count;
		}

		return l_count;
	}

	void setParentGameObject(gameObject * p_new_parent)
	{
		parentGameObject = p_new_parent;
	}

	gameObject * getParentGameObject()
	{
		return parentGameObject;
	}

private:
	/*!***************************************************************************************
	\brief  Inserts a component to the list.  The same component instance will not be added twice.
	\param p_component - Pointer to the component to be added
	\return bool - True if the topography changed, false otherwise
	*****************************************************************************************/
	template<typename T>
	bool insert(componentHandle<T> p_component)
	{
		if (p_component)
		{
			bool l_hasType = false;

			// get the type of the component and search the map for that type
			componentType l_type = p_component->type();
			auto l_range = map.equal_range(l_type);

			if (l_range.first != map.end())
			{
				// if there are components of that type, look for an exact one (prevent duplicate)
				while (l_range.first != l_range.second)
				{
					l_hasType = true;
					if (((*(l_range.first)).second) == toBaseHandle(p_component))
					{
						return false; // no changes made
					}
					++l_range.first;
				}
			}

			// wasn't already there, so add it
			map.emplace(l_type, componentHandle<T>(p_component));
			currentComponentTypeSet.setType(l_type);
			return !l_hasType; // did topography change
		}

		return false;  // no changes made
	}

	/*!*******************************************************************************
	\brief  Rebuilds the current component type set member as it cannot handle removals
	*********************************************************************************/
	void buildCurrentComponentTypeSet();

	gameObject * parentGameObject;
	icomponentRepository* getRepository();

	GameObjectFiltering::componentTypeSet currentComponentTypeSet; /*!< set representing the *
																	* current configuration  *
																	* of components. */
	std::unordered_multimap<componentType, baseComponentHandle> map; /*!< Map of component type to   *
															  * component references,        *
															  * duplicates allowed. */

};

