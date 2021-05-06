/*!***************************************************************************************
\file       GameObjectSerialization.cpp
\author     Aaron Damyen
\date       10/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the serialization functions for GameObjects.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "GameObjectSerialization.hpp"
//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <ComponentFactory.hpp>
#include <set>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//

typeRT toTypeRT(const std::shared_ptr<gameObject> & p_go)
{
	typeRT result;
	result.setTypeName("gameObject");
	result.setVariableName(p_go->m_name);

	// save children
	std::vector<typeRT> l_child_as_type_run_time;
	for (const auto& i_children : p_go->children)
	{
		typeRT current_gameObjectRT("id", i_children->getID());
		l_child_as_type_run_time.push_back(current_gameObjectRT);
	}
	typeRT childrenRT(l_child_as_type_run_time);
	childrenRT.setTypeName("childList");
	childrenRT.setVariableName("childList");

		//Might be 2 objid on file?
	// save components
	std::vector<typeRT> l_components_as_type_run_time;
	for (auto& i_component : p_go->getAllComponents())
	{
		// only write non-transient components
		if(!((i_component)->isTransient()))
			l_components_as_type_run_time.push_back((i_component)->toTypeRT());
	}
	typeRT componentsRT(l_components_as_type_run_time);
	componentsRT.setTypeName("components");
	componentsRT.setVariableName("components");

	result.insertMember(typeRT("id", p_go->getID()));
	result.insertMember(childrenRT);
	result.insertMember(componentsRT);
	return result;
}

void synchronizeGameObject(std::shared_ptr<gameObject> & p_go, typeRT & p_type)
{
	// remove all components
	//   Only way to remove components that shouldn't be part of the object.
	p_go->RemoveAllComponents();
	
	// get components from the new typert
	auto l_changed_components = p_type.member("components").array();

	for(auto l_iter = l_changed_components.begin(); l_iter != l_changed_components.end(); ++l_iter)
	{
		//Find the component by componentType->has a string in it.
		auto l_component = p_go->component_list.getComponentByString(l_iter->getTypeName());
		//If it is attached to this gameObject, change it
		if(l_component)
		{
			(l_component)->updateFromTypeRT(*l_iter);
		}
		else
		{
			componentFactory::addComponentByType(p_go, l_iter->getTypeName());
            l_component = p_go->component_list.getComponentByString(l_iter->getTypeName());
			(l_component)->updateFromTypeRT(*l_iter);
		}
	}
}
