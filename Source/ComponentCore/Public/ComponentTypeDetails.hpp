/*!***************************************************************************************
\file       ComponentTypeDetails.hpp
\author     Aaron Damyen
\date       1/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  The definition of the component type details metadata structure
*****************************************************************************************/
#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include <string>
#include "ComponentType.hpp"
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/*!***************************************************************************************
\par struct: componentTypeDetails
\brief   Structure describing details of a component type
*****************************************************************************************/
struct componentTypeDetails
{
	/*!***************************************************************************************
	\brief Detailed constructor for creating a new instance of the structure
	\param p_name - the internal name for the component type
	\param p_friendlyName - the human readable name of the component type
	\param p_allowEditorAdd - true if the editor can add a component of this type
	\param p_allowEditorRemove - true if the editor can remove a component of this type
	\param p_allowMultipleInstances - true if multiple instances of this component can be added to a game object
	*****************************************************************************************/
	componentTypeDetails(componentType p_type, std::string p_name, std::string p_friendlyName, bool p_allowEditorAdd, bool p_allowEditorRemove, bool p_allowMultipleInstances, std::string p_subType = "")
		: type(p_type), name{p_name}, subType(p_subType), friendlyName{ p_friendlyName }, allowEditorAdd{ p_allowEditorAdd }, allowEditorRemove{ p_allowEditorRemove }, allowMultipleInstances{ p_allowMultipleInstances }
	{ }

	componentType type;
	std::string name; //!< The internal name of the component type
	std::string subType;
	std::string friendlyName; //!< The human-readable name of the component type
	bool allowEditorAdd; //!< true if the editor can add a component of this type
	bool allowEditorRemove; //!< true if the editor can remove a component of this type
	bool allowMultipleInstances; //!< true if multiple instance of this type are allowed on an object
};

