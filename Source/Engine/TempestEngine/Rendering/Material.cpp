/*!***************************************************************************************
\file       Material.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Holds iformation about the objects color and how light interacts with it.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Material.hpp"
//========1st Party Includes============================================================//
#include "../Utility/AssetManager.hpp"
#include "../Utility/GeneralizedFileLoader.hpp"
//========3rd Party Includes============================================================//
#include <Logger.hpp>
#include <string_view>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

		//==============================================================================//
		//        Getters & Setters                                                     //
		//==============================================================================//
material& material::setMaterialName(std::string const& p_name)
{
	m_materialName = p_name;
	return *this;
}

material& material::setAmbientColor(color const& p_color)
{
	m_ambientColor = p_color;
	return *this;
}

material& material::setDiffuseColor(color const& p_color)
{
	m_diffuseColor = p_color;
	return *this;
}

material& material::setSpecularColor(color const& p_color)
{
	m_specularColor = p_color;
	return *this;
}

material& material::setEmissiveColor(color const& p_color)
{
	m_emissiveColor = p_color;
	return *this;
}

material& material::setSpecularExponent(float p_pow)
{
	m_specularExponent = p_pow;
	return *this;
}

material& material::setIlluminationModel(int p_index)
{
	m_illumModel = p_index;
	return *this;
}
