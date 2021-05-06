/*!***************************************************************************************
\file       UISpriteArray.cpp
\author     Aaron Damyen
\date       12/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the UI Sprite Array component
*****************************************************************************************/

//======== Self Include ================================================================//
#include "UISpriteArray.hpp"
//======== 1st Party Includes ==========================================================//
#include "GameObject.hpp"
#include "UISprite.hpp"
#include "Logger.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//


uiSpriteArray::uiSpriteArray()
{
}

uiSpriteArray::uiSpriteArray(typeRT& p_type)
{
	updateFromTypeRT(p_type);
}

typeRT uiSpriteArray::toTypeRT() const
{
	typeRT result;

	result.setTypeName(getType().name());
	result.setVariableName(getType().name());

	typeRT textureCountRT("count", static_cast<unsigned int>(m_texture_names.size()));
	result.insertMember(textureCountRT);

	for(size_t index = 0; index < m_texture_names.size(); index++)
	{
		typeRT textureNameRT("textureName" + std::to_string(index), m_texture_names[index]);
		result.insertMember(textureNameRT);
	}

	typeRT selectionIndexRT("selectionIndex", static_cast<unsigned int>(m_selection));
	result.insertMember(selectionIndexRT);

	return result;
}

void uiSpriteArray::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	m_texture_names.clear();
	
	if(l_members.find("count") != l_members.end())
	{
		m_texture_names.resize(static_cast<size_t>(p_type.member("count").getUInt()));
	}
	for(size_t index = 0; index < m_texture_names.size(); index++)
	{
		if(l_members.find("textureName" + std::to_string(index)) != l_members.end())
		{
			m_texture_names[index] = p_type.member("textureName" + std::to_string(index)).getString();
		}
	}

	if(l_members.find("selectionIndex") != l_members.end())
	{
		m_selection = static_cast<size_t>(p_type.member("selectionIndex").getUInt());
	}
}

void uiSpriteArray::setSelectionIndex(size_t p_index)
{
	logger("uiSpriteArray").debug() << "size = " << m_texture_names.size();
	if(p_index >= 0 && p_index < m_texture_names.size())
	{
		m_selection = p_index;

		// find sibling uiSprite component
		auto l_uiSprite = getGameObject().lock()->getComponent<uiSprite>();
		if(l_uiSprite)
		{
			l_uiSprite->setTextureName(m_texture_names[m_selection]);
		}
	}
}
