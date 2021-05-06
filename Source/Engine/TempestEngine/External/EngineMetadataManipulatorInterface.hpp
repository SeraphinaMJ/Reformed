/*!***************************************************************************************
\file       EngineMetadataManipulatorInterface.hpp
\author     Aaron Damyen
\date       1/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  The definition of the engine metadata manipulator public interface
*****************************************************************************************/
#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include <vector>
#include <ComponentTypeDetails.hpp>

//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/*!***************************************************************************************
\par class: engineMetaManipulatorInterface
\brief   Provides external access to engine metadata.  Mostly read-only.
*****************************************************************************************/
class engineMetadataManipulatorInterface
{
public:
	virtual const std::vector<componentTypeDetails> & getComponentTypes() = 0;
	virtual const componentTypeDetails & getComponentType(const std::string & p_typeName) = 0;
	virtual bool hasComponentType(const std::string & p_typeName) = 0;
};

