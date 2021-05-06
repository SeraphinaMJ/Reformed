/*!***************************************************************************************
\file       ToggleDisplayOnButtonPress.cpp
\author     Alex Kudebeh
\date       11/26/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Toggles display of this UI object based on a button press
*****************************************************************************************/

//======== Self Include ================================================================//
#include "ToggleDisplayOnButtonPress.hpp"
#include "UISprite.hpp"
//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
toggleDisplayOnButtonPress::toggleDisplayOnButtonPress(typeRT & p_data)
{
	updateFromTypeRT(p_data);
}

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//
typeRT toggleDisplayOnButtonPress::toTypeRT() const
{
	typeRT toReturn;
	toReturn.setTypeName(getType().name());
	toReturn.setVariableName(getType().name());
	return toReturn;
}

void toggleDisplayOnButtonPress::onStart()
{
	m_input = getComponent<inputComponent>();
	m_input->addButtonMapping("toggle", Key::H);
	componentHandle<uiSprite> sprite = getComponent<uiSprite>();
	sprite->setVisible(false);
}

void toggleDisplayOnButtonPress::onUpdate()
{
	if (m_input->getButtonState("toggle"))
	{
		if (!pressed)
		{
			if (show)
			{
				componentHandle<uiSprite> sprite = getComponent<uiSprite>();
				sprite->setVisible(false);
				show = false;
			}
			else
			{
				componentHandle<uiSprite> sprite = getComponent<uiSprite>();
				sprite->setVisible(true);
				show = true;
			}
		}
		pressed = true;
	}
	else
	{
		pressed = false;
	}


}

void toggleDisplayOnButtonPress::onShutdown()
{

}

void toggleDisplayOnButtonPress::updateFromTypeRT(typeRT & p_type)
{

}

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//


