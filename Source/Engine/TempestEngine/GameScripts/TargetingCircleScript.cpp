#include "TargetingCircleScript.hpp"
#include "InputComponent.hpp"
#include "Logger.hpp"
#include "Transform.hpp"
#include "PhysicsSystem.hpp"
#include "PlatformWindowing.hpp"
#include "../Components/Renderer.hpp"
#include "GraphicsSystem.hpp"
#include "EnemyAIScript.h"
#include <algorithm>
#include "GameScripts/PathFollowScript.hpp"
#include "GameScripts/PathingStateScript.hpp"
#include "Enemy.hpp"

#include "EnemyLeader.hpp"

#include "Player.hpp"
#include "KillFieldScript.hpp"

#include "MinionGroup_1.hpp"
#include "MinionGroup_2.hpp"
#include "MinionGroup_3.hpp"
#include "MinionGroup_4.hpp"
#include "MinionGroup_5.hpp"
#include "MinionGroup_6.hpp"
#include "MinionGroup_7.hpp"
#include "MinionGroup_8.hpp"
#include "MinionGroup_9.hpp"
#include "MinionGroup_10.hpp"

#include "MinionType_1.hpp"
#include "MinionType_2.hpp"
#include "MinionType_3.hpp"

#include "MinionAll.hpp"
#include "MinionAllExceptType_1.hpp"

#include "MinionLeader.hpp"

#include "Health.hpp"
#include "Agent.hpp"
#include "Allegiance.hpp"
#include "Debug.hpp"
#include "ParticleEmitter.hpp"

#include "Formations/FormationsManager.hpp"
#include <Vector2.hpp>
#include "UISpriteArray.hpp"
#include "Selected.hpp"


targetingCircleScript::targetingCircleScript(typeRT p_data)
{

}

void targetingCircleScript::onStart()
{

	auto l_input_component2 = getComponent<inputComponent>();
	l_input_component2->addCursorMapping("default_mouse", Cursor::Default_Mouse);
	l_input_component2->addButtonMapping("mmb", MouseButton::Middle);
	l_input_component2->addButtonMapping("rmb", MouseButton::Right);
	l_input_component2->addButtonMapping("lmb", MouseButton::Left);

	// Group buttons
	l_input_component2->addButtonMapping("1", Key::Digit_1);
	l_input_component2->addButtonMapping("2", Key::Digit_2);
	l_input_component2->addButtonMapping("3", Key::Digit_3);
	l_input_component2->addButtonMapping("4", Key::Digit_4);
	l_input_component2->addButtonMapping("5", Key::Digit_5);
	l_input_component2->addButtonMapping("6", Key::Digit_6);
	l_input_component2->addButtonMapping("7", Key::Digit_7);
	l_input_component2->addButtonMapping("8", Key::Digit_8);
	l_input_component2->addButtonMapping("9", Key::Digit_9);
	l_input_component2->addButtonMapping("0", Key::Digit_0);

	// Type buttons
	l_input_component2->addButtonMapping("q", Key::Q);
	l_input_component2->addButtonMapping("e", Key::E);
	l_input_component2->addButtonMapping("f", Key::F);

	l_input_component2->addButtonMapping("z", Key::Z);
}



int spawnCount = 0;
int spawnTotal = 1;
bool clickedOnce = false;

int numFollowersOfPath = 1;
float timer = 0.0f;

void targetingCircleScript::setMinionType(componentHandle<inputComponent> p_input)
{
	size_t l_index = 0;
	bool updateUI = false;
	
	// check state(refactor)
	if (p_input->getButtonState("q")) {
		currentActiveType = spawnPrefabType::type_1;
		m_typeName = "Defender";
		l_index = 0;
		updateUI = true;
	}

	else if (p_input->getButtonState("e")) {
		currentActiveType = spawnPrefabType::type_2;
		m_typeName = "Attacker";
		l_index = 1;
		updateUI = true;
	}
	/*
	else if (p_input->getButtonState("f")) {
		currentActiveType = spawnPrefabType::type_3;
		m_typeName = "Minion_3";
		l_index = 2;
		updateUI = true;
	}
	*/

	// push the change to the UI display
	if (updateUI)
	{
		auto l_spriteObj = findObject("MinionSelector");
		if (l_spriteObj)
		{
			auto l_sprite_comp = l_spriteObj->getComponent<uiSpriteArray>();
			if (l_sprite_comp)
			{
				l_sprite_comp->setSelectionIndex(l_index);
			}
		}
	}
	
}

void targetingCircleScript::SetGroupAsSelected() 
{
	
	std::vector<std::shared_ptr<gameObject>> SelectedObjects = getCurrentSelectedGroup();

	for (auto object : SelectedObjects){
		object->getComponent<renderer>()->setMaterialName("pinkglow.mtl");
		
	}
}


void targetingCircleScript::setMinionGroup(componentHandle<inputComponent> p_input)
{
	// check state(refactor)
	if (p_input->getButtonState("1")) {
		
		currentActiveGroup = spawnPrefabGroup::group_1;
		m_groupName = "Group_1";
	
		   SetGroupAsSelected();


	}

	else if (p_input->getButtonState("2")) {
		currentActiveGroup = spawnPrefabGroup::group_2;
		m_groupName = "Group_2";

		    SetGroupAsSelected();


	}

	else if (p_input->getButtonState("3")) {
		currentActiveGroup = spawnPrefabGroup::group_3;
		m_groupName = "Group_3";
	
			SetGroupAsSelected();

	
	}

	else if (p_input->getButtonState("4")) {
		currentActiveGroup = spawnPrefabGroup::group_4;
		m_groupName = "Group_4";
	
			SetGroupAsSelected();

	
	}

	else if (p_input->getButtonState("5")) {
		currentActiveGroup = spawnPrefabGroup::group_5;
		m_groupName = "Group_5";
		
			SetGroupAsSelected();

	
	}

	else if (p_input->getButtonState("6")) {
		currentActiveGroup = spawnPrefabGroup::group_6;
		m_groupName = "Group_6";
		
			SetGroupAsSelected();

		
	}

	else if (p_input->getButtonState("7")) {
		currentActiveGroup = spawnPrefabGroup::group_7;
		m_groupName = "Group_7";
	
			SetGroupAsSelected();

	
	}

	else if (p_input->getButtonState("8")) {
		currentActiveGroup = spawnPrefabGroup::group_8;
		m_groupName = "Group_8";

			SetGroupAsSelected();


	}

	else if (p_input->getButtonState("9")) {
		currentActiveGroup = spawnPrefabGroup::group_9;
		m_groupName = "Group_9";
	
			SetGroupAsSelected();

	
	}

	else if (p_input->getButtonState("0")) {
		currentActiveGroup = spawnPrefabGroup::group_10;
		m_groupName = "Group_10";

			SetGroupAsSelected();

		
	}




		

	
}

void targetingCircleScript::buildMinionType(std::shared_ptr<gameObject> p_minion)
{


	if (currentActiveType == spawnPrefabType::type_1)
	{
		p_minion->addComponent<minionType_1>();

		if(p_minion->getComponent<transform>())
			p_minion->getComponent<transform>()->setScale(vector3(0.75f, 0.75f, 0.75f));
		if(p_minion->getComponent<renderer>())
			p_minion->getComponent<renderer>()->setPrimitiveType(CHICKEN_FILE);
		if(p_minion->getComponent<renderer>())
			p_minion->getComponent<renderer>()->setMaterialName("solidblue.mtl");
	}
	else if (currentActiveType == spawnPrefabType::type_2)
	{
		p_minion->addComponent<minionType_2>();
		p_minion->addComponent<minionAllExceptType_1>();

		if (p_minion->getComponent<pathFollowScript>())
			p_minion->getComponent<pathFollowScript>()->setIsPatrolUnit(true);
		if (p_minion->getComponent<transform>())
			p_minion->getComponent<transform>()->setScale(vector3(0.75f, 0.75f, 0.75f));
		if(p_minion->getComponent<renderer>())
			p_minion->getComponent<renderer>()->setPrimitiveType(DUCK_FILE);
		if(p_minion->getComponent<renderer>())
			p_minion->getComponent<renderer>()->setMaterialName("solidred.mtl");
	}
	/*
	else if (currentActiveType == spawnPrefabType::type_3)
	{
		p_minion->addComponent<minionType_3>();
		p_minion->addComponent<minionAllExceptType_1>();
		p_minion->getComponent<transform>()->setScale(vector3(0.75f, 0.75f, 0.75f));
		l_renderer_comp->setPrimitiveType(CUBE_FILE);
		p_minion->getComponent<renderer>()->setMaterialName("solidgreen.mtl");
	}
	*/
}

void targetingCircleScript::addMinionGroupComponent(std::shared_ptr<gameObject> p_minion)
{
	if (currentActiveGroup == spawnPrefabGroup::group_1) {
		if(!p_minion->getComponent<minionGroup_1>())
			p_minion->addComponent<minionGroup_1>();
	}
	else if (currentActiveGroup == spawnPrefabGroup::group_2){
		if (!p_minion->getComponent<minionGroup_1>())
			p_minion->addComponent<minionGroup_2>();
	}
	else if (currentActiveGroup == spawnPrefabGroup::group_3) {
		if (!p_minion->getComponent<minionGroup_1>())
			p_minion->addComponent<minionGroup_3>();
	}
	else if (currentActiveGroup == spawnPrefabGroup::group_4) {
		if (!p_minion->getComponent<minionGroup_1>())
			p_minion->addComponent<minionGroup_4>();
	}
	else if (currentActiveGroup == spawnPrefabGroup::group_5) {
		if (!p_minion->getComponent<minionGroup_1>())
			p_minion->addComponent<minionGroup_5>();
	}
	else if (currentActiveGroup == spawnPrefabGroup::group_6) {
		if (!p_minion->getComponent<minionGroup_1>())
		p_minion->addComponent<minionGroup_6>();
	}
	else if (currentActiveGroup == spawnPrefabGroup::group_7) {
		if (!p_minion->getComponent<minionGroup_1>())
			p_minion->addComponent<minionGroup_7>();
	}
	else if (currentActiveGroup == spawnPrefabGroup::group_8) {
		if (!p_minion->getComponent<minionGroup_1>())
			p_minion->addComponent<minionGroup_8>();
	}
	else if (currentActiveGroup == spawnPrefabGroup::group_9) {
		if (!p_minion->getComponent<minionGroup_1>())
			p_minion->addComponent<minionGroup_9>();
	}
	else if (currentActiveGroup == spawnPrefabGroup::group_10) {
		if (!p_minion->getComponent<minionGroup_1>())
			p_minion->addComponent<minionGroup_10>();
	}

}

group targetingCircleScript::getCurrentSelectedGroup()
{
	GameObjectFiltering::componentTypeSet l_pattern;
	l_pattern.setType(minionAll::getType());
	//l_pattern.setType(minionLeader::getType());

	auto minions = getSystemManager()->getGameObjectGatherer()->getList(l_pattern);
	group l_type, l_group, l_leaders;

	for (auto minion : minions)
	{
		if (currentActiveType == spawnPrefabType::type_1)
		{
			if (minion->getComponent<minionType_1>())
			{
				l_type.push_back(minion);
			}
		}
		else if (currentActiveType == spawnPrefabType::type_2)
		{
			if (minion->getComponent<minionType_2>())
			{
				l_type.push_back(minion);
			}
		}
		else if (currentActiveType == spawnPrefabType::type_3)
		{
			if (minion->getComponent<minionType_3>())
			{
				l_type.push_back(minion);
			}
		}
	}

	for (auto type : l_type)
	{
		if (currentActiveGroup == spawnPrefabGroup::group_1)
		{
			if (type->getComponent<minionGroup_1>())
				l_group.push_back(type);
		}
		else if (currentActiveGroup == spawnPrefabGroup::group_2) {
			if (type->getComponent<minionGroup_2>())
				l_group.push_back(type);
		}
		else if (currentActiveGroup == spawnPrefabGroup::group_3) {
			if (type->getComponent<minionGroup_3>())
				l_group.push_back(type);
		}
		else if (currentActiveGroup == spawnPrefabGroup::group_4) {
			if (type->getComponent<minionGroup_4>())
				l_group.push_back(type);
		}
		else if (currentActiveGroup == spawnPrefabGroup::group_5) {
			if (type->getComponent<minionGroup_5>())
				l_group.push_back(type);
		}
		else if (currentActiveGroup == spawnPrefabGroup::group_6) {
			if (type->getComponent<minionGroup_6>())
				l_group.push_back(type);
		}
		else if (currentActiveGroup == spawnPrefabGroup::group_7) {
			if (type->getComponent<minionGroup_7>())
				l_group.push_back(type);
		}
		else if (currentActiveGroup == spawnPrefabGroup::group_8) {
			if (type->getComponent<minionGroup_8>())
				l_group.push_back(type);
		}
		else if (currentActiveGroup == spawnPrefabGroup::group_9) {
			if (type->getComponent<minionGroup_9>())
				l_group.push_back(type);
		}
		else if (currentActiveGroup == spawnPrefabGroup::group_10) {
			if (type->getComponent<minionGroup_10>())
				l_group.push_back(type);
		}
	}

	return l_group;

}


//group targetingCircleScript::getCurrentSelectedGroup()
//{
//	//get all enemy objects
//	GameObjectFiltering::componentTypeSet l_pattern;
//	l_pattern.setType(minionAll::getType());
//	//l_pattern.setType(minionLeader::getType());
//
//	if (currentActiveType == spawnPrefabType::type_1)
//	{
//		l_pattern.setType(minionType_1::getType());
//	}
//	else if (currentActiveType == spawnPrefabType::type_2)
//	{
//		l_pattern.setType(minionType_2::getType());
//	}
//	/*
//	else if (currentActiveType == spawnPrefabType::type_3)
//	{
//		l_pattern.setType(minionType_3::getType());
//	}
//	*/
//	if (currentActiveGroup == spawnPrefabGroup::group_1) {
//		l_pattern.setType(minionGroup_1::getType());
//	}
//	else if (currentActiveGroup == spawnPrefabGroup::group_2) {
//		l_pattern.setType(minionGroup_2::getType());
//	}
//	else if (currentActiveGroup == spawnPrefabGroup::group_3) {
//		l_pattern.setType(minionGroup_3::getType());
//	}
//	else if (currentActiveGroup == spawnPrefabGroup::group_4) {
//		l_pattern.setType(minionGroup_4::getType());
//	}
//	else if (currentActiveGroup == spawnPrefabGroup::group_5) {
//		l_pattern.setType(minionGroup_5::getType());
//	}
//	else if (currentActiveGroup == spawnPrefabGroup::group_6) {
//		l_pattern.setType(minionGroup_6::getType());
//	}
//	else if (currentActiveGroup == spawnPrefabGroup::group_7) {
//		l_pattern.setType(minionGroup_7::getType());
//	}
//	else if (currentActiveGroup == spawnPrefabGroup::group_8) {
//		l_pattern.setType(minionGroup_8::getType());
//	}
//	else if (currentActiveGroup == spawnPrefabGroup::group_9) {
//		l_pattern.setType(minionGroup_9::getType());
//	}
//	else if (currentActiveGroup == spawnPrefabGroup::group_10) {
//		l_pattern.setType(minionGroup_10::getType());
//	}
//
//	return getSystemManager()->getGameObjectGatherer()->getList(l_pattern);
//}


void targetingCircleScript::onUpdate()
{
	// get emitter
	auto emitter = findObject("reticle_emitter");

	if (!emitter)
		return;

	componentHandle<inputComponent> l_input = getGameObject().lock()->getComponent<inputComponent>();

	setMinionType(l_input);
	setMinionGroup(l_input);

	if (m_enabled && !m_pauseShown)
	{
		emitter->getComponent<particleEmitter>()->setActiveFlag(true);

		//if (l_input->getButtonState("rmb"))
		
			const auto l_mousePos = [&l_input]() -> vector2
			{
				const auto [mouseX, mouseY] = l_input->getCursorState("default_mouse");
				return { float(mouseX), float(mouseY) };
			}();
				
			if (!getSystem<graphicsSystem>()) return;
			const auto [l_viewOff, l_viewDim] = getSystem<graphicsSystem>()->getViewport().asVectors();
				
			
			const auto l_adjustedPos = l_mousePos - l_viewOff;

			if (!getComponent<transform>()) return;
			vector3 camPos = getComponent<transform>()->getGlobalPosition();

			vector3 impactNormal;

			objID l_object_id = 0;
			getSystem<physicsSystem>()->castRay(l_viewDim.x, l_viewDim.y, l_adjustedPos.x, l_adjustedPos.y, enm_world, currentClickPoint, impactNormal, l_object_id);
			logger("picker").debug() << currentClickPoint.x << "," << currentClickPoint.y << "," << currentClickPoint.z << "\n";


			if (emitter) {
				// set emitters pos
				emitter->getComponent<transform>()->setPosition(currentClickPoint);

			}
			else
			{
				// make debug output
			}
			
			/*
			if (l_input->getButtonState("z"))
			{
				spawnMeteor(currentClickPoint);
			}
			*/
			
            
			// Spawning objects
			if (l_input->getButtonState("lmb"))
			{
				if (spawnCount < spawnTotal)
				{
					spawnMinionAt(/*1*/3, currentClickPoint + vector3(0, 3.0f, 0));
					spawnCount++;
				}
			}
			else
			{
				spawnCount = 0;
			}

			// Setting markers 
			if (l_input->getButtonState("rmb"))
			{
				if (!clickedOnce)
				{
					pushNewPathToObjects(!(l_input->getButtonState("f")));
					clickedOnce = true;
					m_lastClickArray[static_cast<int>(currentActiveType)][static_cast<int>(currentActiveGroup)] = currentClickPoint;
				}
			}
			else
			{
				clickedOnce = false;
			}
	}
	else {
		if (!emitter->getComponent<particleEmitter>()) return;
			emitter->getComponent<particleEmitter>()->setActiveFlag(false);
	}

	if (timer <= 0)
	{
		setEnemysPathToPlayer();
		timer = 2.5f;
	}

	timer -= dt();

	debug::displayText(m_typeName, "Neuton-Regular", 425, 290, .5f, color::red);
	debug::displayText(m_groupName, "Neuton-Regular", -500, 290, .5f, color::red);
}


void targetingCircleScript::spawnMeteor(vector3 spawnPoint)
{
	std::shared_ptr<gameObject> newMeteor = createGameObjectOnMySpace("Meteor");
	//newMeteor->addComponent<
}


void targetingCircleScript::onShutdown()
{

}



///////////////////////////////// Node Path ///////////////////////////////////////////////////
void targetingCircleScript::pushNewPathToObjects(bool resetPath)
{
	
	
	group l_group = getCurrentSelectedGroup();
			

	for(auto object : l_group)
	{

		if (!object->getComponent<pathFollowScript>()) return;
			auto pathScript = object->getComponent<pathFollowScript>();
		if (!object->getComponent<pathingStateScript>()) return;
			auto stateScript = object->getComponent<pathingStateScript>();

		if (resetPath)
			pathScript->dumpPaths();

		auto l_state = stateScript->getState();


		if (l_state == pathingStateScript::PathState::PSHome)
		{
			vector3 pos = object->getComponent<transform>()->getPosition();

			std::shared_ptr<graphPath> homePath = this->getSystemManager()->getSystem<waypointGraphPathfinder>()->getPath(pos, currentClickPoint, true);
			//auto markerPos = maker_1->getComponent<transform>()->getPosition();
			pathScript->addPathToObject(homePath);
		}
		else if(l_state == pathingStateScript::PathState::PSPathing)// add path from one maker to the next marker
		{
			std::shared_ptr<graphPath> commonPath = this->getSystemManager()->getSystem<waypointGraphPathfinder>()->getPath(m_lastClickArray[static_cast<int>(currentActiveType)][static_cast<int>(currentActiveGroup)],
				currentClickPoint, true, (currentActiveType == spawnPrefabType::type_2)); //Patrol units need both start and end visuals
			pathScript->addPathToObject(commonPath);
		}
	}
}


void targetingCircleScript::setEnemysPathToPlayer() {

	//create time counter (if time)

	//get player objects position
	GameObjectFiltering::componentTypeSet l_pattern1;
	l_pattern1.setType(player::getType());
	auto l_player = getSystemManager()->getGameObjectGatherer()->getList(l_pattern1);

	vector3 l_playerPos;

	if (l_player.size()) {
		l_playerPos = l_player[0]->getComponent<transform>()->getPosition();
	}

	//get all enemy objects
	GameObjectFiltering::componentTypeSet l_pattern2;
	l_pattern2.setType(enemyLeader::getType());
	auto l_enemys = getSystemManager()->getGameObjectGatherer()->getList(l_pattern2);

	for (auto& l_enemy : l_enemys) 
	{
		//grab enemys position
		auto l_enemyPos = l_enemy->getComponent<transform>()->getPosition();

		//grab enemys pathscript
		auto l_pathScript = l_enemy->getComponent<pathFollowScript>();


		//clear old path
		l_pathScript->dumpPaths();

		std::shared_ptr<graphPath> homePath = this->getSystemManager()->getSystem<waypointGraphPathfinder>()->getPath(l_enemyPos, l_playerPos);

		// set new path for enemy from enemy to player
		l_pathScript->addPathToObject(homePath);


	}
		

}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////// EnemySawningScript  //////////////////////////////////////////////

void targetingCircleScript::spawnMinionAt(int p_count, vector3 p_position)
{
    auto formMan = this->getSystemManager()->getSystem<FormationsManager>();
    auto l_squad = formMan->createSquad(Faction::PLAYER);

	for (int i = 0; i < p_count; ++i)
	{
		auto l_minion = createGameObjectOnMySpace("playerMinion");

		auto l_renderer_comp = l_minion->addComponent<renderer>();
		l_renderer_comp->setProgramType(programType::enm_forward);

		l_minion->addComponent<pathFollowScript>();
		l_minion->addComponent<pathingStateScript>();
		l_minion->addComponent<minionAll>();
		l_minion->addComponent<killFieldScript>();

		l_minion->addComponent<Agent>()->setTreeName("player_minion");
		//l_minion->addComponent<Allegiance>()->setFaction(Faction::PLAYER);
		l_minion->addComponent<Health>()->SetHealth(100.0f);
		//l_enemy->getComponent<pathFollowScript>()->setHomePosition(p_position)

		buildMinionType(l_minion);
		addMinionGroupComponent(l_minion);
		
		l_minion->addComponent<rigidBody>();
		l_minion->getComponent<rigidBody>()->setCollisionLayer(collisionLayer::enm_enemy, collisionLayer::enm_player | collisionLayer::enm_world | collisionLayer::enm_enemy);
		l_minion->getComponent<rigidBody>()->setMass(20.0f);
		l_minion->getComponent<rigidBody>()->setFriction(0.8f);

		l_minion->getComponent<transform>()->setPosition(p_position);
		getSystem<physicsSystem>()->addPhysicsObjectToWorld(l_minion);

        l_squad->joinSquad(l_minion->getID());

		l_minion->addComponent<selected>();

	}

	
}

//std::vector<std::shared_ptr<gameObject>> targetingCircleScript::findClosestObjectsToPoint(unsigned p_ammount, vector3 const & origin)
//{
//
//	std::vector< std::shared_ptr<gameObject>> l_closestObjects;
//
//	if (m_groups[currentActiveType].size()) {
//		std::sort(m_groups[currentActiveType].begin(), m_groups[currentActiveType].end(),
//			[origin](std::shared_ptr<gameObject>& a, std::shared_ptr<gameObject>& b) -> bool
//		{
//
//			vector3 position_a = a->getComponent<transform>()->getGlobalPosition();
//			vector3 position_b = b->getComponent<transform>()->getGlobalPosition();
//
//			float distOriginToA = distance(origin, position_a);
//			float distOriginToB = distance(origin, position_b);
//			
//
//			return distOriginToA < distOriginToB;
//		});
//
//		// if less then size are being asked for give size
//		if (p_ammount > m_groups[currentActiveType].size()) {
//			p_ammount = m_groups[currentActiveType].size();
//		}
//
//		
//		l_closestObjects.reserve(p_ammount);
//		l_closestObjects.assign(m_groups[currentActiveType].begin(), m_groups[currentActiveType].begin() + p_ammount);
//
//	}
//
//	// grab amount of objects
//
//
//	return l_closestObjects;
//}
