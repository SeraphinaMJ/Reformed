/*!***************************************************************************************
\file       StyleKeeper.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a manager for rendering styles within the Editor
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

namespace Editor
{
	//======== Types =====================================================================//
	struct style
	{
		std::string component_type;
		std::string variable_name;
		std::string property;
		int value;
	};

	typedef std::unordered_map<std::string /*variable name*/,
		std::unordered_map<std::string /*property*/,
		unsigned int /*value*/>> propertyMap;

	/*!***************************************************************************************
	\par class: styleKeeper
	\brief   This is a style manager for rendering details in the editor
	*****************************************************************************************/
	class styleKeeper
	{
	public:
		/*!***************************************************************************************
		\brief  Retrieves the value of a style triplet for a component type, variable name and property
		\param p_component_type - the type of component of the style
		\param p_variable_name - the variable name of the style
		\param p_property - the property of the style
		\return unsigned int - the value of the style triplet
		*****************************************************************************************/
		unsigned int getStyle(std::string p_component_type, std::string p_variable_name, std::string p_property) const;
		/*!***************************************************************************************
		\brief  Retrieves the property map for a component type
		\param p_component_type - the type of the component of the property map
		\return propertyMap & - map of properties and style values for the specified component type
		*****************************************************************************************/
		const propertyMap & getStyles(std::string p_component_type) const;
		/*!***************************************************************************************
		\brief  Assigns a value of a style triplet for a component type, variable name and property
		\param p_component_type - the type of component of the style
		\param p_variable_name - the variable name of the style
		\param p_property - the property of the style
		\param p_value - the new value of the style
		*****************************************************************************************/
		void setStyle(std::string p_component_type, std::string p_variable_name, std::string p_property, int p_value);
		/*!***************************************************************************************
		\brief  Removes a style triplet from the mapping
		\param p_component_type - the type of component of the style
		\param p_variable_name - the variable name of the style
		\param p_property - the property of the style
		*****************************************************************************************/
		void clearStyle(std::string p_component_type, std::string p_variable_name, std::string p_property);

	private:
		std::tuple<bool /*found*/, unsigned int /*value*/> getStyleSpecific(std::string p_component_type, std::string p_variable_name, std::string p_property) const;
		std::unordered_map<std::string /*component_type*/, propertyMap> m_maps;
	};


} // namespace Editor



