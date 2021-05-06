/*!***************************************************************************************
\file       SpawnerScript.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "SpawnerScript.h"
#include "EnemyAIScript.h"
#include "../Components/Renderer.hpp"
#include "../Physics/PhysicsSystem.hpp"
#include "GameObjectSerialization.hpp"
#include "Agent.hpp"
#include "Allegiance.hpp"
#include "Factions.hpp"
#include "Enemy.hpp"
#include "PathFollowScript.hpp"
#include "PathingStateScript.hpp"
#include "Health.hpp"
#include "KillFieldScript.hpp"

#include "Formations/FormationsManager.hpp"

spawnerScript::spawnerScript(typeRT p_data) : scriptCPP(p_data)
{
}

void spawnerScript::onStart()
{

	//auto l_enemy = createGameObjectOnMySpace("spawnedEnemy");
	//l_enemy->getComponent<transform>()->setScale(vector3(0.75f, 0.75f, 0.75f));
	////l_enemy->addComponent<enemyAIScript>();
	//l_enemy->addComponent<Agent>()->setTreeName("enemy");
	//l_enemy->addComponent<Allegiance>()->setFaction(Allegiance::Faction::ENEMY);
	//l_enemy->addComponent<enemy>();
	//l_enemy->addComponent<pathFollowScript>();
	//l_enemy->addComponent<pathingStateScript>();

	//l_enemy->addComponent<rigidBody>();
	//l_enemy->getComponent<rigidBody>()->setCollisionLayer(collisionLayer::enm_enemy, collisionLayer::enm_player | collisionLayer::enm_world | collisionLayer::enm_enemy);
	//l_enemy->getComponent<rigidBody>()->setMass(20.0f);
	
	//auto l_renderer_comp = l_enemy->addComponent<renderer>();
	//l_renderer_comp->setPrimitiveType(CUBE_FILE);
	//l_renderer_comp->setProgramType(programType::enm_forward);
	//l_enemy->getComponent<renderer>()->setMaterialName("solidred.mtl");

	/*m_Prefab = ::toTypeRT(l_enemy);
	getMySpace()->destroyGameObject(l_enemy);*/
}

void spawnerScript::onUpdate()
{
	//get all enemy objects
	GameObjectFiltering::componentTypeSet l_pattern2;
	l_pattern2.setType(enemy::getType());
	auto l_enemys = getSystemManager()->getGameObjectGatherer()->getList(l_pattern2);
         
	if (l_enemys.size() < m_maxSpawn) 
    {
	    if (m_spawnTimer <= 0)
	    {
		    m_spawnTimer = m_spawnRate;
		    spawnEnemy(m_spawnCount);
	    }
	    else
	    {
		    m_spawnTimer -= dt();
	    }
    }

}
void spawnerScript::spawnEnemy(int p_count)
{

    // create a squad
    auto formManager = this->getSystemManager()->getSystem<FormationsManager>();
    auto enemy_squad = formManager->createSquad(Faction::ENEMY);
    if (!enemy_squad) { return; }

	for (int i = 0; i < p_count; ++i)
	{
		auto l_enemy = getMySpace()->instantiateGameObject("spawnedEnemy");

		//////////////////////////////////////////////////////////////
		l_enemy->getComponent<transform>()->setScale(vector3(0.75f, 0.75f, 0.75f));
		//l_enemy->addComponent<enemyAIScript>();
		l_enemy->addComponent<Agent>()->setTreeName("enemy");
		l_enemy->addComponent<Health>()->SetHealth(100.0f);
		l_enemy->addComponent<enemy>();
		l_enemy->addComponent<pathFollowScript>();
		l_enemy->addComponent<pathingStateScript>();
		l_enemy->addComponent<killFieldScript>();

		l_enemy->addComponent<rigidBody>();
		l_enemy->getComponent<rigidBody>()->setCollisionLayer(collisionLayer::enm_enemy, collisionLayer::enm_player | collisionLayer::enm_world | collisionLayer::enm_enemy);
		l_enemy->getComponent<rigidBody>()->setMass(20.0f);

        // charles
        l_enemy->getComponent<rigidBody>()->setDamping(0.2);
        //l_enemy->getComponent<rigidBody>()->setLocalGravity({ 0,-30,0 });
        //l_enemy->getComponent<rigidBody>()->useGlobalGravity(false);
        //l_enemy->getComponent<rigidBody>()->setCollisionShape(collisionShape::enm_square);
        //l_enemy->getComponent<rigidBody>()->setVelocity({ 0,0,0 });
        l_enemy->getComponent<rigidBody>()->setFriction(0.5);
        



		auto l_renderer_comp = l_enemy->addComponent<renderer>();
		l_renderer_comp->setPrimitiveType(CUBE_FILE);
		l_renderer_comp->setProgramType(programType::enm_forward);
		l_enemy->getComponent<renderer>()->setMaterialName("solidred.mtl");

		
		//////////////////////////////////////////////////////////

		vector3 l_myPosition = getComponent<transform>()->getPosition();
		l_enemy->getComponent<transform>()->setPosition(l_myPosition);
		getSystem<physicsSystem>()->addPhysicsObjectToWorld(l_enemy);

        if (!enemy_squad->joinSquad(l_enemy->getID()))
        {
            std::cout << "HOW";
        }

	}
}


void spawnerScript::spawnEnemyAt(int p_count, vector3 p_position)
{
	for (int i = 0; i < p_count; ++i)
	{
		auto l_enemy = getMySpace()->instantiateGameObject(m_Prefab);

		l_enemy->getComponent<transform>()->setPosition(p_position);
		
		getSystem<physicsSystem>()->addPhysicsObjectToWorld(l_enemy);
	}
}
