/*!***************************************************************************************
\file       EngineMetadataManipulator.cpp
\author     Aaron Damyen
\date       1/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  The definition of the engine metadata manipulator class members
*****************************************************************************************/

//========Self Include==================================================================//
#include "EngineMetadataManipulator.hpp"
//========1st Party Includes============================================================//
#include <ComponentFactory.hpp>
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//


const std::vector<componentTypeDetails>& engineMetadataManipulator::getComponentTypes()
{
	return componentFactory::getComponentTypeDetails();
}

bool engineMetadataManipulator::hasComponentType(const std::string& p_typeName)
{
	auto & l_list = componentFactory::getComponentTypeDetails();
	for(auto & i_iter : l_list)
	{
		if (i_iter.name == p_typeName)
			return true;
	}
	return false;
}

const componentTypeDetails& engineMetadataManipulator::getComponentType(const std::string& p_typeName)
{
	auto & l_list = componentFactory::getComponentTypeDetails();
	for(auto & i_iter : l_list)
	{
		if (i_iter.name == p_typeName)
			return i_iter;
	}
	return *l_list.end();
}
