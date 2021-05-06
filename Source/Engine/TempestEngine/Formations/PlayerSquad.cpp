/*!***************************************************************************************
\file       PlayerSquad.cpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Structure containing player's minions in game.
*****************************************************************************************/
#include "PlayerSquad.hpp"
#include "Space.hpp"
#include "FormationsManager.hpp"
#include "GameObject.hpp"

#include "BehaviorSystem/BehaviorHelpers.hpp"

#include "RigidBody.hpp"

#include "MinionLeader.hpp"

#include <cassert>

void PlayerSquad::scoutArea()
{
    // update detected enemies list
    Squad::deleteDeadEnemies();
    Squad::deleteOutOfRangeEnemies();

    auto hostiles = this->formManager->GetEnemies();

    // reset engaged
    m_engaged = false;

    // for all units that are against our squad
    for (auto hostile : hostiles)
    {
        // if they are within detection radius
        if (withinDetectionRadius(hostile->getID()))
        {
            // set to engaged if enemy nearby
            m_engaged = true;
            this->m_detected_enemies.insert(hostile->getID());
        }
    }

}

std::vector<objID> PlayerSquad::getAllUnits() const
{
    std::vector<objID> allUnits;

    // if player is alive
    if (leaderAlive())
    {
        allUnits.push_back(this->leaderID);
    }

    for (SquadRingPtr ring : m_rings)
    {
        for (auto slot : ring->getSlots())
        {
            if (BH::NPC_Is_Alive(slot.first, this->m_space))
            {
                allUnits.push_back(slot.first);
            }
        }
    }

    return allUnits;
}

void PlayerSquad::insertUnit(objID unit)
{
    // if no rings created or no room in last ring
    if (m_rings.empty() || m_rings.back()->full())
    {
        int ringCapacity = (m_rings.size() + 1)  * BASE_SQUAD_SIZE;
        float radius = (m_rings.size() + 1) * 10.0f;

        // make a new ring
        SquadRingPtr newSquad = std::make_shared<SquadRing>(ringCapacity, this, radius, m_space);
        m_rings.push_back(newSquad);
    }

    m_rings.back()->insert(unit);

    m_rings.back()->calculatePositions();

}

void PlayerSquad::promoteToLeader(objID newLeader)
{
    this->leaderID = newLeader;

    auto l_leader = BH::getGameObj(leaderID, m_space).lock();

    if (l_leader)
    {
        // make sure leader does not collide with its own units
        auto rb = l_leader->getComponent<rigidBody>();
        if (rb)
        {
            //rb->setCollisionLayer(collisionLayer::enm_player, collisionLayer::enm_enemy | collisionLayer::enm_world);
        }

        l_leader->addComponent<minionLeader>();
    }

}

PlayerSquad::~PlayerSquad()
{
}

PlayerSquad::PlayerSquad(space * space_, FormationsManager * fm)
    : Squad(space_, fm, Faction::PLAYER)
{
    // get player id and set that as leader
    //this->leaderID = fm->GetPlayer()->getID();

    this->m_detection_radius = 24.0f;

}

bool PlayerSquad::joinSquad(objID unit)
{
    // don't do anything if player is dead
    auto player = formManager->GetPlayer();
    if (player.expired()) { return false; }

    // if empty then make this unit our leader
    if (this->empty())
    {
        promoteToLeader(unit);
    }
    else
    {
        // otherwise tell him to get in line
        insertUnit(unit);
    }

    return true;
}

void PlayerSquad::leaveSquad(objID unit)
{
    // player should never leave his own squad
    assert(unit != leaderID);

    auto ring = this->getRingFromID(unit);
    ring->remove(unit);

}

void PlayerSquad::Update(float dt)
{
    for (auto ring : m_rings)
    {
        ring->RemoveDeadUnits();
    }

    // if player is alive
    if (!this->formManager->GetPlayer().expired())
    {

        // scout area for enemies
        scoutArea();

        // if not engaged
        if (this->m_engaged == false)
        {
            // move units into position
            for (auto ring : this->m_rings)
            {
                ring->Update(dt);
            }
        }
    }
    else
    {
        // otherwise deleted all player squads
        this->m_rings.clear();
        this->m_detected_enemies.clear();
    }

}

SquadRingPtr PlayerSquad::getRingFromID(objID unit) const
{
    for (auto ring : m_rings)
    {
        if (ring->hasUnit(unit))
        {
            return ring;
        }
    }

    throw std::runtime_error("No ring has this unit?");
}

//void PlayerSquad::kill()
//{
//
//    // get all units that are alive including player
//    auto allUnits = getAllUnits();
//
//    // for each squad member
//    for (auto unit : allUnits)
//    {
//
//        // kill unit
//        auto go = BH::getGameObj(unit, this->m_space);
//        if (!go.expired())
//        {
//            go.lock()->markForDeletion();
//        }
//
//    }
//
//}

bool PlayerSquad::hasUnit(objID unit) const
{
    if (isLeader(unit))
    {
        return true;
    }

    for (auto ring : m_rings)
    {
        if (ring->hasUnit(unit))
        {
            return true;
        }
    }

    return false;
}

bool PlayerSquad::full() const
{
    return false;
}

bool PlayerSquad::empty() const
{
    if (leaderAlive())
    {
        return false;
    }

    for (auto ring : m_rings)
    {
        // if ring has units
        if (!ring->empty())
        {
            return false;
        }
    }

    return true;
}

