/*!***************************************************************************************
\file       DamagePrismScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "DamagePrismScript.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../Components/Renderer.hpp"


damagePrismScript::damagePrismScript(typeRT p_data):scriptCPP(p_data)
{
}

void damagePrismScript::damage(float p_amount)
{
	changeColor(p_amount);
}

void damagePrismScript::changeColor(float p_amount)
{
	//r = x, g = y, b = z, a = w
	color newColor = getGameObject().lock()->getComponent<renderer>()->getColor();

	float l_colorMod;

	if(m_up)
	{
		l_colorMod = p_amount;
	}
	else
	{
		l_colorMod = -p_amount;
	}

	if (m_rgbMod == 1)
	{
		newColor.x += l_colorMod;

		if (newColor.x >= 1 || newColor.x <= 0)
		{
			updateState();
		}
	}
	else if (m_rgbMod == 2)
	{
		newColor.y += l_colorMod;

		if (newColor.y >= 1 || newColor.y <= 0)
		{
			updateState();
		}
	}
	else if (m_rgbMod == 3)
	{
		newColor.z += l_colorMod;

		if (newColor.z >= 1 || newColor.z <= 0)
		{
			updateState();
		}
	}

	getGameObject().lock()->getComponent<renderer>()->setColor(newColor);
}

void damagePrismScript::updateState()
{
	//r = x, g = y, b = z, a = w
	vector4 theColor = getGameObject().lock()->getComponent<renderer>()->getColor();

	++m_rgbMod;

	if(m_rgbMod == 4)
	{
		m_rgbMod = 1;
	}

	if (m_rgbMod == 1)
	{
		if(theColor.x <0.1f)
		{
			m_up = true;
		}
		else
		{
			m_up = false;
		}
	}
	else if(m_rgbMod == 2)
	{
		if (theColor.y < 0.1f)
		{
			m_up = true;
		}
		else
		{
			m_up = false;
		}
	}
	else if (m_rgbMod == 3)
	{
		if (theColor.z < 0.1f)
		{
			m_up = true;
		}
		else
		{
			m_up = false;
		}
	}
}
