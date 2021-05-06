/*!***************************************************************************************
\file       LifetimeScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "LifetimeScript.hpp"

lifetimeScript::lifetimeScript(typeRT p_data): scriptCPP(p_data)
{
}

void lifetimeScript::onStart()
{
}

void lifetimeScript::destroySelf()
{
	getMySpace()->destroyGameObject(getGameObject().lock());
}

void lifetimeScript::setLifetime(float p_lifetime)
{
	m_Lifetime = p_lifetime;
}

float lifetimeScript::getLifetime()
{
	return m_Lifetime;
}

void lifetimeScript::onUpdate()
{
	if(m_Timer <= m_Lifetime)
	{
		m_Timer += dt();
	}
	else
	{
		destroySelf();
	}
}
