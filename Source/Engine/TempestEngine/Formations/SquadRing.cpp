/*!***************************************************************************************
\file       SquadRing.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Each squad may have 1 or more ring formations around it. This class
            represents that formation.
*****************************************************************************************/
#include "SquadRing.hpp"
#include "Transform.hpp"
#include "RigidBody.hpp"
#include "Space.hpp"

#include "Squad.hpp"

#include "PathFollowScript.hpp"

#include "BehaviorSystem/BehaviorHelpers.hpp"


#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_ENEMY_SQUAD_SIZE 8

bool SquadRing::hasUnit(objID id) const
{
    return m_slots.find(id) != m_slots.end();
}

bool SquadRing::full() const
{
    return m_slots.size() == (size_t)max_capacity;
}

bool SquadRing::empty() const
{
    return m_slots.empty();
}

int SquadRing::spaceLeft() const
{
    return max_capacity - m_slots.size();
}

int SquadRing::unitCount() const
{
    return m_slots.size();
}

objID SquadRing::AvailableUnit() const
{
    return m_slots.begin()->first;
}

void SquadRing::Update(float dt)
{
    float epsilon = 0.1f;

    //// remove any dead units
    //RemoveDeadUnits();

    calculatePositions();

    // for each unit
    for (auto slot : m_slots)
    {
        // get unit object
        auto actor = BH::getGameObj(slot.first, m_space).lock();
        vector3 actorPos = actor->getComponent<transform>()->getPosition();


        // find distance between unit and 
        auto slotPos = slot.second;
        vector3 vecToslot = slotPos - actorPos; // check this

        float dist = vecToslot.distance();

        // if distance from designated slot is too much
        if (dist > 1.5f)
        {

            // get rigidbody
            auto rb = actor->getComponent<rigidBody>();

            auto l_pathscript = actor->getComponent<pathFollowScript>();

            // if pathfollowscript exists then add it
            if (l_pathscript)
            {
                l_pathscript->dumpPaths();

                // normalize vector
                vecToslot = vecToslot.normalize();

                vecToslot.setY(0.0f);

                // set speed and direction
                if (dist < 3.0f)
                {
                    rb->setVelocity(vecToslot * 0.25f);
                }
                else
                {
                    rb->setVelocity(vecToslot * 8.5f);
                }

            }

        }
    }
}

void SquadRing::insert(objID id)
{
    assert(m_slots.find(id) == m_slots.end());
    assert(m_slots.size() < (size_t)max_capacity);

    int currentsize = m_slots.size();

    auto newPos = m_radius * vector3{ sin(currentsize * 2.0f * (float)M_PI / max_capacity), 0,
                   cos(currentsize * 2.0f * (float)M_PI / max_capacity) };

    m_slots[id] = newPos;
}

void SquadRing::remove(objID id)
{
    m_slots.erase(id);
}

void SquadRing::takeUnits(SquadRing& rhs)
{

    // while we aint full
    while (!this->full() && !rhs.empty())
    {
        objID unit = rhs.AvailableUnit();
        this->insert(unit);
        rhs.remove(unit);
    }
}

void SquadRing::calculatePositions()
{

    if (m_slots.size() == 0) { return; }

    // get leader position
    vector3 leaderPos = m_parent_squad->getCenter();

    int index = 0;

    // for each slot, update its position
    for (auto& slot : m_slots)
    {
        // recalculate positions based on unit count
        vector3 newPos = leaderPos;
        newPos += m_radius * vector3{
            sin(index * 2 * (float)M_PI / max_capacity),    0,
            cos(index * 2 * (float)M_PI / max_capacity) };

        slot.second = newPos;
        index++;
    }



}

vector3 SquadRing::getSlotPosition(objID id) const
{
    return m_slots.find(id)->second;
}

void SquadRing::RemoveDeadUnits()
{
    // for each slot
    for (auto it = m_slots.begin(); it != m_slots.end();)
    {
        // if object no longer exists
        if (BH::getGameObj(it->first, m_space).lock() == nullptr)
        {
            it = m_slots.erase(it);
        }
        else
        {
            it++;
        }

    }
}

