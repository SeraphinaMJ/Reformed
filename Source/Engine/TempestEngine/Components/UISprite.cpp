/*!***************************************************************************************
\file       UISprite.cpp
\author     Aaron Damyen
\date       11/18/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the UI Sprite component
*****************************************************************************************/

//======== Self Include ================================================================//
#include "UISprite.hpp"
//======== 1st Party Includes ==========================================================//
#include "TypeData.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//


uiSprite::uiSprite()
	: m_modelName{FullScreenQuad}
{
}

uiSprite::uiSprite(typeRT & p_type)
{
	updateFromTypeRT(p_type);
}

typeRT uiSprite::toTypeRT() const
{
	typeRT result;

	result.setTypeName(getType().name());
	result.setVariableName(getType().name());

	typeRT texture("texture", m_textureName);
	result.insertMember(texture);
	
	return result;
}

void uiSprite::updateFromTypeRT(typeRT & p_type)
{
	auto l_members = p_type.members();

	if (l_members.find("texture") != l_members.end())
	{
		m_textureName = (p_type.member("texture").getString());
	}
}

void uiSprite::setVisible(bool p_active)
{
	m_isVisible = p_active;
}
