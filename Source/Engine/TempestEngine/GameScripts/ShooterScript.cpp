/*!***************************************************************************************
\file       ShooterScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "ShooterScript.h"
#include "../Components/Renderer.hpp"
#include "../Components/RigidBody.hpp"
#include "../Physics/PhysicsSystem.hpp"
#include "LifetimeScript.hpp"
#include "../SceneManagement/Transform.hpp"
#include "GameObjectSerialization.hpp"

shooterScript::shooterScript(typeRT p_data): scriptCPP(p_data)
{
}

void shooterScript::onStart()
{
	auto l_ObjectToSpawn = getMySpace()->instantiateGameObject("spawnTemplate");
	l_ObjectToSpawn->addComponent<rigidBody>();
	l_ObjectToSpawn->getComponent<rigidBody>()->setCollisionLayer(collisionLayer::enm_enemy, collisionLayer::enm_player | collisionLayer::enm_world | collisionLayer::enm_enemy);
	l_ObjectToSpawn->getComponent<rigidBody>()->setMass(10.0f);
	l_ObjectToSpawn->getComponent<rigidBody>()->setCollisionShape(collisionShape::enm_square);

	auto l_renderer_comp = l_ObjectToSpawn->addComponent<renderer>();
	l_renderer_comp->setPrimitiveType(CUBE_FILE);
	l_renderer_comp->setProgramType(programType::enm_forward);
	l_ObjectToSpawn->getComponent<renderer>()->setMaterialName("basic.mtl");

	l_ObjectToSpawn->addComponent<lifetimeScript>();


	m_ObjectToShoot = ::toTypeRT(l_ObjectToSpawn);
	getMySpace()->destroyGameObject(l_ObjectToSpawn);
}

void shooterScript::onUpdate()
{
	if(m_Timer <= 0)
	{
		m_Timer = m_ShootTime;
		shoot(m_ObjectToShoot);
	}
	else
	{
		m_Timer -= dt();
	}
}

void shooterScript::shoot(typeRT p_objectData)
{
	auto l_ObjectToSpawn = getMySpace()->instantiateGameObject(m_ObjectToShoot);
	getSystem<physicsSystem>()->addPhysicsObjectToWorld(l_ObjectToSpawn);
	l_ObjectToSpawn->getComponent<transform>()->setPosition(getComponent<transform>()->getPosition());
	l_ObjectToSpawn->getComponent<rigidBody>()->applyImpulse(vector3(0,1,0) * 500);
}
