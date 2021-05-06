#pragma once
/*!***************************************************************************************
\file       DebugLines.hpp
\author     Patrick Cook
\date       9/24/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  component for storing and displaying multiple line segments
*****************************************************************************************/

#include "Component.hpp"
#include "QueryableInterface.hpp"
#include "LineBuffer.hpp"

#include <ComponentTypeDetails.hpp>
#include <IComponentRepository.hpp>
#include <string>
#include <vector>
#include <optional>

// forward decl
class graphicsSystem;
class gameObject;

/*!***************************************************************************************
\par class: debugLines
\brief   This component allows other systems to write debug information 
to be drawn as line segments.
Notice: line segments are stored in world space
*****************************************************************************************/
class debugLines final : public componentCRTP<debugLines>, public queryableInterface
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
	friend class graphicsSystem;

	/*!***************************************************************************************
	\brief  conversion/default ctor.
	\param tag - used for differntiating between multiple instances of this component
	*****************************************************************************************/
	static componentHandle<debugLines> getLinesByTag(std::shared_ptr<gameObject> obj, std::string const& p_tag);

	/*!***************************************************************************************
	\brief  conversion/default ctor.
	\param tag - used for differntiating between multiple instances of this component
	*****************************************************************************************/
	debugLines(std::string&& tag = "UNTAGGED");

	/*!***************************************************************************************
	\brief  Converts type to transform.
	\param p_type - type to convert
	*****************************************************************************************/
	debugLines(typeRT& p_type);

  bool isTransient() override { return true; };

	//////==============================================================================//////
	//////    Non-Static                                                                //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  returns segment buffer to be edited. Marks as dirty
	\return mutable line segment buffer
	*****************************************************************************************/
	std::vector<lineSegment>& getLineBuffer() noexcept;

	/*!***************************************************************************************
	\brief  returns segment buffer to be viewed.
	\return immutable line segment buffer
	*****************************************************************************************/
	std::vector<lineSegment> const& getLineBuffer() const noexcept;


	/*!***************************************************************************************
	\brief  returns constructor-defined tag. Used for differentiating buffers
	\return immutable tag
	*****************************************************************************************/
	std::string const& getTag() const noexcept;

	void setTag(std::string const p_tag);

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
	\brief  Frees any resources allocated by the component.
	*****************************************************************************************/
	virtual ~debugLines() = default;

	void updateFromTypeRT(typeRT & p_type) override;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Retrieves the type of component this instance is. SceneSystem requirement.
	\return componentType - The type of this component.
	*****************************************************************************************/
	virtual componentType const type() const override { return debugLines::getType(); }
	/*!***************************************************************************************
	\brief  Retrieves the type of component this class is.  SceneSystem requirement.
	\return componentType - The type of component this class represents.
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forConcrete<debugLines>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "DebugLines", false, false, true); }

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
	//! The names of properties relevant to the queryableInterface, and prepped for telemetry tracking.
	std::vector<std::string> m_property_names = { "Tag" };

	//! line segment CPU and GPU buffer information
	lineBuffer m_buffer;

	//! distinguishes this instance from another on the same game object
	std::string m_tag;
};
