/*!***************************************************************************************
\file       SectionHeaders.hpp
\author     Patrick Cook
\date       9/16/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a work in progress example sheet for section headers.
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Component.hpp"
#include "QueryableInterface.hpp"
#include "LineBuffer.hpp"

//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
#if 0
class lineMesh final : public component, public queryableInterface
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	//////==============================================================================//////
	//////    Non-Static                                                                //////
	//////==============================================================================//////
	lineBuffer& getBuffer() noexcept;
	lineBuffer const& getBuffer() const noexcept;

	/////================================================================================/////
	/////     Functions                                                                  /////
	/////================================================================================/////

	////==================================================================================////
	////      Overridden                                                                  ////
	////==================================================================================////

	//======================================================================================//
	//        Helper                                                                        //
	//======================================================================================//

	/*!***************************************************************************************
	\brief  Converts this into a type
	\return Type equivalent.
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Constructs the component and initializes it to defaults
	*****************************************************************************************/
	lineMesh();

	/*!***************************************************************************************
	\brief  Converts type to transform.
	\param p_type - type to convert
	*****************************************************************************************/
	lineMesh(typeRT& p_type);

	/*!***************************************************************************************
	\brief  Frees any resources allocated by the component.
	*****************************************************************************************/
	virtual ~lineMesh() = default;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Retrieves the type of component this instance is. SceneSystem requirement.
	\return componentType - The type of this component.
	*****************************************************************************************/
	virtual componentType const type() const override { return lineMesh::getType(); }
	/*!***************************************************************************************
	\brief  Retrieves the type of component this class is.  SceneSystem requirement.
	\return componentType - The type of component this class represents.
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forConcrete<lineMesh>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Line Mesh", false, false, false); }

	/*!***************************************************************************************
	\brief  Retrieves the type this class, for use with the queryable interface.
	\return string - The type of class this represents.
	*****************************************************************************************/
	std::string getQueryableType() override;

	/*!***************************************************************************************
	\brief  Retrieves the ID of this object's parent, given the parent is valid
	\return string - The ID of the object.
	*****************************************************************************************/
	std::string getQueryableID() override;

	/*!***************************************************************************************
	\brief  Retrieves a property of the object by name
	\param p_name the name of the property(data)
	\return string - Returns the desired data
	*****************************************************************************************/
	std::string getQueryableProperty(std::string p_name) override;

	/*!***************************************************************************************
	\brief  Retrieves a property of the object by name
	\param p_name the name of the property(data)
	\return string - Returns the desired data
	*****************************************************************************************/
	std::vector<std::string> getQueryablePropertyNames() override;

private:
	lineBuffer m_cpuSegmentData;
	//! The names of properties relevant to the queryableInterface, and prepped for telemetry tracking.
	std::vector<std::string> property_names = {};



	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	/*!***************************************************************************************
	\brief  Updates the matrices and global data if any of the local values have changed.
	*****************************************************************************************/
};

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

// namespace <namespace name>

#endif