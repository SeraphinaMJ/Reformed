/*!***************************************************************************************
\file       GroundPoundScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "GroundPoundScript.h"
#include "../Components/RigidBody.hpp"
#include "../Components/InputComponent.hpp"
#include "../Scripting/ScriptingSystem.hpp"
#include "../GameObjectGatherer.hpp"
#include "../Components/ParticleEmitter.hpp"
#include "../Physics/PhysicsSystem.hpp"
#include "EnemyAIScript.h"

groundPoundScript::groundPoundScript(typeRT p_data) : scriptCPP(p_data)
{
}

void groundPoundScript::onStart()
{
}

void groundPoundScript::onUpdate()
{
	if(checkGroundPound() && m_pounding == false)
	{
		m_pounding = true;
		getGameObject().lock()->getComponent<rigidBody>()->applyForce(vector3(0, -(m_poundStrength * m_scaleFactor), 0));
	}
	if(m_pounding == true && getComponent<rigidBody>()->isGrounded())
	{
		m_pounding = false;

		//For testing particles. Toggles the emitter on and off on ground pound.
		if(getGameObject().lock()->getComponent<particleEmitter>())
			getGameObject().lock()->getComponent<particleEmitter>()->setActiveFlag(!getGameObject().lock()->getComponent<particleEmitter>()->getActiveFlag());

    std::vector<std::shared_ptr<gameObject> > l_objects;
    l_objects = getSystemManager()->getSystem<physicsSystem>()->getObjectsWithinRadius(getGameObject().lock()->getComponent<transform>()->getPosition(), m_radius);

    vector3 my_position = getGameObject().lock()->getComponent<transform>()->getPosition();

    for (auto i_object : l_objects)
    {
      vector3 position = i_object->getComponent<transform>()->getPosition();
	  // If the object is an enemy, damage the enemy
	  if (i_object->getComponent<enemyAIScript>())
	  {
		  //damage
		  i_object->getComponent<enemyAIScript>()->setActivationState(false);
	  }
      const vector3 direction = (position - my_position).normalize();
      const vector3 force = direction * m_poundStrength;
      i_object->getComponent<rigidBody>()->applyForce(direction * m_scaleFactor);
    }

	}
}

bool groundPoundScript::checkGroundPound()
{
	return getGameObject().lock()->getComponent<inputComponent>()->getButtonState("pound") && !getGameObject().lock()->getComponent<rigidBody>()->isGrounded();
}
