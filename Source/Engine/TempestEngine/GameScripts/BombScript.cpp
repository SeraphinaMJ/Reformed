/*!***************************************************************************************
\file       BombScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "BombScript.h"
#include "../Scripting/ScriptingSystem.hpp"
#include "../GameObjectGatherer.hpp"
#include "PlayerControllerScript.hpp"
#include "../SceneManagement/Transform.hpp"


bombScript::bombScript(typeRT p_data): scriptCPP(p_data)
{
}


void bombScript::onStart()
{
	m_detonationTimer = 0;
	m_originalSize = getGameObject().lock()->getComponent<transform>()->getScale();
	m_isActivated = true;
}

void bombScript::onUpdate()
{
	if(m_isActivated)
	{
		m_detonationTimer += dt();
	}

	if ((m_detonationTime - m_detonationTimer) <= 2)
	{
		m_blink *= 0.95f;
	}

	if ((m_detonationTime - m_detonationTimer) <= 0.5)
	{
		detonate();
	}

	if(m_detonationTimer > m_detonationTime)
	{
		explode();
	}
}

void bombScript::detonate()
{
	float local_x = getGameObject().lock()->getComponent<transform>()->getScale().getX();
	if((local_x/m_originalSize.getX())<=2)
	{
		getGameObject().lock()->getComponent<transform>()->setScale(getGameObject().lock()->getComponent<transform>()->getScale() * 1.01f);
	}
}

void bombScript::explode()
{
	//Get code from ground pound
	GameObjectFiltering::componentTypeSet l_input_component_pattern;
	l_input_component_pattern.setType(rigidBody::getType());
	std::vector<std::shared_ptr<gameObject>> l_list = getSystemManager()->getGameObjectGatherer()->getList(l_input_component_pattern);

	vector3 my_position = getGameObject().lock()->getComponent<transform>()->getPosition();
	for (auto i_object : l_list)
	{
		vector3 position = i_object->getComponent<transform>()->getPosition();
		float x_distance = abs(my_position.x - position.x);
		float y_distance = abs(my_position.y - position.y);
		if (x_distance == 0 && y_distance == 0)
		{
			continue;
		}
		if (x_distance <= m_radius && y_distance <= m_radius)
		{
			vector3 direction = (position - my_position).normalize();
			vector3 force = direction * m_strength;
			i_object->getComponent<rigidBody>()->applyForce(force * m_scaleFactor);
		}
	}
}


