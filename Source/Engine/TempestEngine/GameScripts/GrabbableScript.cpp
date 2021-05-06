/*!***************************************************************************************
\file       GrabbableScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "GrabbableScript.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../Components/RigidBody.hpp"

grabbableScript::grabbableScript(typeRT p_data): scriptCPP(p_data)
{
}

void grabbableScript::onStart()
{
	m_ThePlayer = findObject("Player");
	m_OFriction = m_rigidBody->getFriction();
	m_OBounce = m_rigidBody->getRestitution();
}

void grabbableScript::onUpdate()
{
	m_PreviousVelocity = m_rigidBody->getVelocity();
}

void grabbableScript::launch()
{
	m_BeingThrown = true;
	disableGravity();
}

void grabbableScript::slam()
{
	m_BeingThrown = true;
	m_BeingSlammed = true;
	disableGravity();
}

void grabbableScript::onCollisionEnter()
{
	//Handle Collision Logic;
}

void grabbableScript::disableGravity()
{
	getGameObject().lock()->getComponent<rigidBody>()->setLocalGravity(vector3(0, 0, 0));
	getGameObject().lock()->getComponent<rigidBody>()->useGlobalGravity(false);
}



