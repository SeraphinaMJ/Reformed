/*!***************************************************************************************
\file       BehaviorTreeManipulator.cpp
\author     Charles Boudousquie
\date       10/11/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manipulator to talk to the BehaviorTreeManager
*****************************************************************************************/
#include "Agent.hpp"
#include "BehaviorTreeManipulator.hpp"
#include "../BehaviorSystem/BehaviorTreeManager.hpp"
#include "SystemManager.hpp"
#include "Allegiance.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "GameObjectGatherer.hpp"

BehaviorTreeManipulator::BehaviorTreeManipulator(systemManagerInterface * manager_) : m_system_manager(manager_)
{}

int BehaviorTreeManipulator::getActiveNodeID(GameObjectPtr agent) const
{
    if (!agent.expired())
    {
        // get behavior tree manager system
        auto BTManager = m_system_manager->getSystem<BehaviorTreeManager>();

        return BTManager->getCurrentNodeID(agent);
    }

    return -1;
}

std::vector<std::shared_ptr<gameObject>> BehaviorTreeManipulator::getObjectsWithAgents()
{
    // get bt manager
    auto btManager = m_system_manager->getSystem<BehaviorTreeManager>();
    return btManager->getAgentGameObjects();
}

void BehaviorTreeManipulator::markTreeChanged(const std::string& treeName)
{
    auto btManager = m_system_manager->getSystem<BehaviorTreeManager>();
    btManager->MarkTreeAsChanged(treeName);
}

