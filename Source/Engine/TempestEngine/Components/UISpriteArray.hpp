/*!***************************************************************************************
\file       UISpriteArray.hpp
\author     Aaron Damyen
\date       12/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the UI Sprite Array component.
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Component.hpp"

//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: uiSpriteArray
\brief   This is a component that displays a image in UI space, but can be quickly changed
          to another image by index
*****************************************************************************************/
class uiSpriteArray : public componentCRTP<uiSpriteArray>
{
public:
	/*!***************************************************************************************
	\brief Default constructor. Initalizes private data.
	*****************************************************************************************/
	uiSpriteArray();
	/*!***************************************************************************************
	\brief Conversion constructor from typeRT.
	\param p_type typeRT that holds the UI sprite array data.
	*****************************************************************************************/
	uiSpriteArray(typeRT & p_type);

	componentType const type() const override { return getType(); }
	static componentType const getType() { static componentType const type = componentType::forConcrete<uiSpriteArray>(); return type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "UI Sprite Array", true, true, true); }

	/*!***************************************************************************************
	\brief Converts the private data into a typeRT to serialize.
	\return Constructed typeRT.
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief Changes any private data inside the UI component that is stored in the typeRT.
	\param p_type typeRT to update private values from.
	*****************************************************************************************/
	void updateFromTypeRT(typeRT& p_type) override;

	void setSelectionIndex(size_t p_index);

private:
	std::vector<std::string> m_texture_names;
	size_t m_selection = 0;
};

