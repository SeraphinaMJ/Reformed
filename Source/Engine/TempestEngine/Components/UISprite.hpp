/*!***************************************************************************************
\file       UISprite.hpp
\author     Aaron Damyen
\date       11/18/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the UI Sprite component.
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "TypeData.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Component.hpp>

//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: uiSprite
\brief   This is a component to display an image in UI space
*****************************************************************************************/
class uiSprite : public componentCRTP<uiSprite>
{
public:
	/*!***************************************************************************************
	\brief Default constructor. Initalizes private data.
	*****************************************************************************************/
	uiSprite();
	/*!***************************************************************************************
	\brief Conversion constructor from typeRT.
	\param p_type typeRT that holds the UI sprite data.
	*****************************************************************************************/
	uiSprite(typeRT & p_type);

	componentType const type() const override { return getType(); }
	static componentType const getType() { static componentType const type = componentType::forConcrete<uiSprite>(); return type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "UI Sprite", true, true, true); }
	
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

	/*!***************************************************************************************
	\brief If a button is active, it is visible.
	\param p_active True or false.
	*****************************************************************************************/
	void setVisible(bool p_active);

	/*!***************************************************************************************
	\brief Whether or not the button is currently visible.
	\return True or false.
	*****************************************************************************************/
	bool isVisible() const { return m_isVisible; }

	/*!***************************************************************************************
	\brief Retrieves the primitive type for rendering. (Usually a quad)
	\return Should always return quad.
	*****************************************************************************************/
	std::string getRenderType() { return m_modelName; }

	/*!***************************************************************************************
	\brief Sets this UI components primitive type for rendering.
	\param p_type The type of primitive.
	*****************************************************************************************/
	void setRenderType(std::string p_type) { m_modelName = p_type; }

	/*!***************************************************************************************
	\brief Retrieves the name of the texture being used for rendering.
	\return A string of the texture name.
	*****************************************************************************************/
	const std::string & getTextureName() { return m_textureName; }

	/*!***************************************************************************************
	\brief Sets the texture to use for rendering the UI component.
	\param p_texture_name String name of the texture.
	*****************************************************************************************/
	void setTextureName(const std::string & p_texture_name) { m_textureName = p_texture_name; }

	void enableHighlight() { m_shaderType = programType::enm_screenSpaceHighlight; }

	void disableHighlight() { m_shaderType = programType::enm_screenSpace; }

	programType getShaderType() const { return m_shaderType; }

private:
	/*!***************************************************************************************
	\brief If the UI component is active (visible).
	*****************************************************************************************/
	bool m_isVisible = true;

	/*!***************************************************************************************
	\brief The render type of the UI component.
	*****************************************************************************************/
	std::string m_modelName;

	/*!***************************************************************************************
	\brief The name of the texture used for rendering.
	*****************************************************************************************/
	std::string m_textureName;

	programType m_shaderType = programType::enm_screenSpace;
};

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

// namespace <namespace name>

