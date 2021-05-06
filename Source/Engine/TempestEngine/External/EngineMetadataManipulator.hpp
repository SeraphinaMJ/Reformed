/*!***************************************************************************************
\file       EngineMetadataManipulator.hpp
\author     Aaron Damyen
\date       1/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  The definition of the engine metadata manipulator class
*****************************************************************************************/
#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//
#include "EngineMetadataManipulatorInterface.hpp"
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/*!***************************************************************************************
\par class: engineMetadataManipulator
\brief   Implements the engineMetadataManipulatorInterface
*****************************************************************************************/
class engineMetadataManipulator : public engineMetadataManipulatorInterface
{
public:
	virtual ~engineMetadataManipulator() = default;

	/*!***************************************************************************************
	\brief  Retrieves the list of all component types known by the engine
	\return vector<componentTypeDetails> - List of the component type detail entries
	*****************************************************************************************/
	const std::vector<componentTypeDetails>& getComponentTypes() override;
	/*!***************************************************************************************
	\brief  Identifies if the engine knows about a component type
	\param p_typeName - the name of the component type to look for.
	\return bool - true if the engine has a detail entry for the component type name, false otherwise
	*****************************************************************************************/
	bool hasComponentType(const std::string& p_typeName) override;
	/*!***************************************************************************************
	\brief  Retrieves the component type detail entry for the specified type.
	\param p_typeName - the name of the component type to retrieve.
	\return componentTypeDetails - the component type detail entry.
	*****************************************************************************************/
	const componentTypeDetails& getComponentType(const std::string& p_typeName) override;
private:

};
