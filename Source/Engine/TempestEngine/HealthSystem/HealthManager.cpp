/*!***************************************************************************************
\file       HealthManager.cpp
\author     Charles Boudousquie
\date       12/2/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manager that updates health component.
*****************************************************************************************/
#include "HealthManager.hpp"
#include "Health.hpp"
#include "GameObjectGatherer.hpp"
#include "GameObjectFilter.hpp"

#include "SystemManagerInterface.hpp"

void HealthManager::onUpdate()
{
    
    GameObjectFiltering::componentTypeSet node_pattern;
    node_pattern.setType(Health::getType());

    // get all NPCs with health
    std::vector<std::shared_ptr<gameObject>> actors = getSystemManager()->getGameObjectGatherer()->getList(node_pattern);
    

    // for every single actor
    for (auto actor : actors)
    {
        actor->getComponent<Health>()->Update(this->getDt());
    }
}

const std::string & HealthManager::name() const
{
    return this->getName();
}
