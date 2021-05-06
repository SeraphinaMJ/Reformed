/*!***************************************************************************************
\file       EnemySquad.cpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Structure containing enemy units in game.
*****************************************************************************************/
#include "EnemySquad.hpp"
#include "BehaviorSystem/BehaviorHelpers.hpp"
#include "FormationsManager.hpp"
#include "MinionAll.hpp"

#include "RigidBody.hpp"

#include "Enemy.hpp"
#include "EnemyLeader.hpp"

#include "PathFollowScript.hpp"

#include "Renderer.hpp"

#include "Logger.hpp"

//std::string printVec3(const vector3& vec)
//{
//    int x = vec.x;
//    int y = vec.y;
//    int z = vec.z;
//
//    return "{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "}";
//}
//
//void EnemySquad::PrintPositions()
//{
//    logger log("");
//    std::cout << std::endl;
//    auto proxy = log.info();
//    for (auto slots : this->m_ring->getSlots())
//    {
//        proxy << printVec3(slots.second);
//        proxy << ",";
//    }
//    std::cout << std::endl;
//
//}

void EnemySquad::scoutArea()
{
    // update detected enemies list
    Squad::deleteDeadEnemies();
    Squad::deleteOutOfRangeEnemies();

    auto hostiles = this->formManager->GetMinions();
    auto player = formManager->GetPlayer();
    if (!player.expired()) { hostiles.push_back(player.lock()); }

    // reset engaged
    this->m_engaged = false;

    // for all units that are against our squad
    for (auto hostile : hostiles)
    {
        // if they are within detection radius
        if (withinDetectionRadius(hostile->getID()))
        {
            // set to engaged if enemy nearby
            this->m_engaged = true;
            this->m_detected_enemies.insert(hostile->getID());
        }
    }

}

std::vector<objID> EnemySquad::getAllUnits() const
{
    std::vector<objID> allUnits;

    // if leader exists then add him to list
    if (leaderAlive())
    {
        allUnits.push_back(this->leaderID);
    }

    for (auto slot : m_ring->getSlots())
    {
        if (BH::NPC_Is_Alive(slot.first, this->m_space))
        {
            allUnits.push_back(slot.first);
        }
    }

    return allUnits;
}

void EnemySquad::promoteToLeader(objID unit)
{

    this->leaderID = unit;
    // attach pathing script to leader
    auto l_leader = BH::getGameObj(leaderID, m_space).lock();

    if (l_leader)
    {
        // make sure leader does not collide with its own units
        auto rb = l_leader->getComponent<rigidBody>();
        if (rb)
        {
            rb->setCollisionLayer(collisionLayer::enm_enemy, collisionLayer::enm_player | collisionLayer::enm_world);
        }

        l_leader->addComponent<enemyLeader>();
    }

}

EnemySquad::EnemySquad(space* space_, FormationsManager* fm) : Squad(space_, fm, Faction::ENEMY)
{
    this->m_detection_radius = 24.0f;
    int max_capacity = 8;
    float radius = 12.0f;

    m_ring = std::make_shared<SquadRing>(max_capacity, this, radius, space_);
}

SquadRingPtr EnemySquad::getRingFromID(objID) const
{
    return m_ring;
}

//void EnemySquad::kill()
//{
//
//    // gather list of everyone in squad
//    auto squadList = getAllUnits();
//
//    // for each unit in ring
//    for (objID unit : squadList)
//    {
//        // kill unit
//        auto go = BH::getGameObj(unit, this->m_space);
//        if (go.expired())
//        {
//            return;
//        }
//        go.lock()->markForDeletion();
//    }
//}

bool EnemySquad::hasUnit(objID unit) const
{
    // if our ring or our leader has same unit id
    return this->m_ring->hasUnit(unit) || (leaderID == unit);
}

void EnemySquad::Update(float dt)
{

    this->m_ring->RemoveDeadUnits();

    // if player exists find path to him
    auto l_player = formManager->GetPlayer();

    auto leader = BH::getGameObj(this->leaderID, m_space);

    bool promotedsomeone = false;

    // if leader died then we choose a new one
    if (leader.expired())
    {
        // if there is someone left to be promoted
        if (!this->empty())
        {
            promoteToLeader(this->m_ring->AvailableUnit());
            leader = BH::getGameObj(this->leaderID, m_space);
            promotedsomeone = true;
        }
        else
        {
            return;
        }
    }

    auto pathScript = leader.lock()->getComponent<pathFollowScript>();

    // if we are engaged then make sure all paths are cleared
    if (this->m_engaged)
    {
        pathScript->dumpPaths();
        pathScript->StopPathing();
    }

    // if there is a player target and we are not preoccupied
    if (!l_player.expired() && !this->m_engaged)
    {
        pathScript->dumpPaths();

        // if there is no path to follow
        if (pathScript->hasPaths() == false)
        {
            // then make path
            vector3 playerPos = BH::getPosition(l_player);

            auto center = this->getCenter();

            std::shared_ptr<graphPath> pathToPlayer = this->getPathFinder()->getPath(center, playerPos);

            pathScript->addPathToObject(pathToPlayer);
        }

        pathScript->ContinuePathing();

    }

    scoutArea();

    // if not engaged then stay in formation
    if (this->m_engaged == false)
    {
        // update positions remove dead squad mates, etc.
        m_ring->Update(dt);
    }


}

bool EnemySquad::joinSquad(objID unit)
{
    if (this->full())
    {
        return false;
    }

    // if leader is dead then unit gets to be new leader
    if (!this->leaderAlive())
    {
        promoteToLeader(unit);
    }
    else
    {
        m_ring->insert(unit);
    }

    return true;
}

void EnemySquad::leaveSquad(objID unit)
{
    // if unit is leader then promote new unit to leader
    if (isLeader(unit))
    {
        // if there is a unit to promote to leader
        if (!m_ring->empty())
        {
            // get available unit and assign him as new leader
            objID newLeader = this->m_ring->AvailableUnit();
            promoteToLeader(newLeader);
            // remove from ring
            this->m_ring->remove(newLeader);
        }

    }
    else
    {
        // just remove from formation
        m_ring->remove(unit);
    }

}

bool EnemySquad::full() const
{
    return m_ring->full();
}

bool EnemySquad::empty() const
{
    return this->m_ring->empty();
}

