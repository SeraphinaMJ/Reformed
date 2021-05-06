/*!***************************************************************************************
\file       JumpPadScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "JumpPadScript.h"
#include "../Components/RigidBody.hpp"
#include "../Physics/PhysicsSystem.hpp"
#include "../Scripting/ScriptingSystem.hpp"
#include "EnemyAIScript.h"

jumpPadScript::jumpPadScript(typeRT p_data) : scriptCPP(p_data)
{
}

void jumpPadScript::onUpdate()
{
	// Get the objects in its radius
	std::vector<std::shared_ptr<gameObject> > l_objects;
	l_objects = getSystemManager()->getSystem<physicsSystem>()->getObjectsWithinRadius(getGameObject().lock()->getComponent<transform>()->getPosition(), m_radius);

	// Push objects upward
	for (auto i_object : l_objects)
	{
		// If the object is an enemy, damage the enemy
		if (i_object->getComponent<enemyAIScript>())
		{
			//damage
			i_object->getComponent<enemyAIScript>()->setActivationState(false);
		}
		vector3 direction(0, 1, 0);
		direction = direction * m_upwardStrength;
		i_object->getComponent<rigidBody>()->applyForce(direction * m_scaleFactor);
	}
}