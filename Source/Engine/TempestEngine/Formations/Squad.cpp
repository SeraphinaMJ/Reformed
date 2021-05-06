/*!***************************************************************************************
\file       Squad.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents the formation of a squad within the game.
*****************************************************************************************/
#include "Squad.hpp"
#include "Space.hpp"
#include "Vector3.hpp"
#include "Transform.hpp"

#include "FormationsManager.hpp"

#include "BehaviorSystem/BehaviorHelpers.hpp"

#include "DynamicWaypoints/WaypointGraphPathfinder.hpp"

#include "SystemManagerInterface.hpp"

Squad::Squad(space* space_, FormationsManager* fm, Faction faction)
    : 
    m_space(space_), 
    formManager(fm), 
    m_faction(faction), 
    m_engaged(false) 
{}

waypointGraphPathfinder * Squad::getPathFinder()
{
    return this->formManager->getSystemManager()->getSystem<waypointGraphPathfinder>();
}

const std::set<objID>& Squad::getDetectedEnemies() const
{
    return this->m_detected_enemies;
}

bool Squad::hasDetectedHostile(objID hostile) const
{
    return this->m_detected_enemies.find(hostile) != this->m_detected_enemies.end();
}

bool Squad::withinDetectionRadius(objID unit) const
{
    auto hostile = m_space->getScene()->findGameObjectByID(unit);
    float distance = BH::GetDistance(hostile, this->getCenter());

    return distance <= m_detection_radius;
}

void Squad::deleteDeadEnemies()
{
    // clear out list of enemies that are already dead
    for (auto it = m_detected_enemies.begin(); it != m_detected_enemies.end();)
    {
        if (BH::NPC_Is_Alive(*it, m_space))
        {
            it++;
        }
        else
        {
            it = m_detected_enemies.erase(it);
        }
    }
}

void Squad::deleteOutOfRangeEnemies()
{
    // clear out list of enemies that are already dead
    for (auto it = m_detected_enemies.begin(); it != m_detected_enemies.end();)
    {
        // if within detection radius then keep it
        if (withinDetectionRadius(*it))
        {
            it++;
        }
        else
        {
            // forget about enemy
            it = m_detected_enemies.erase(it);
        }
    }
}

bool Squad::leaderAlive() const
{
    if(BH::getGameObj(leaderID, m_space).expired())
    {
        return false;
    }

    return true;
}

bool Squad::isDead() const
{
    // there will always be a new leader for a squad until it is completely destroyed
    if (BH::getGameObj(this->leaderID, m_space).expired())
    {
        return true;
    }

    return false;
}

bool Squad::isEngaged() const
{
    return m_engaged;
}

Faction Squad::getFaction() const
{
    return m_faction;
}

bool Squad::InPosition(objID unit)
{
    float epsilon = 1.0f;

    auto l_game_object = BH::getGameObj(unit, m_space);

    if(!l_game_object.expired())
    {
        auto ourPos = l_game_object.lock()->getComponent<transform>()->getPosition();

        auto ring = this->getRingFromID(unit);

        auto idealPos = ring->getSlotPosition(unit);

        float distance = (ourPos - idealPos).distance();

        // if within acceptable distance
        if (distance <= epsilon)
        {
            return true;
        }
        return false;
    }

    return false;
}

//void Squad::Update(float dt)
//{
//
//    // for each ring check if it is not filled, if so take squad members from 
//    // outside ring.
//    //for(auto& ring = m_rings.begin(); ring != rings.end();)
//    //{
//    //    // if we have room
//    //    if(!(*ring)->Full())
//    //    {
//    //        auto nextRing = std::next(ring);
//    //        // if not at end
//    //        if(nextRing != rings.end())
//    //        {
//    //            // if next ring actually has units we can take
//    //            if(!(*nextRing)->Empty())
//    //            {
//    //                (*ring)->TakeUnits(*(*nextRing));
//    //            }
//    //        }
//    //    }
//    //    
//    //    ring++;
//    //}
//
//    // for each ring push units such that they are in their correct positions.
//    /*for(auto ring : m_rings)
//    {
//        ring->Update(dt);
//    }*/
//
//}

vector3 Squad::getCenter() const
{
    auto l_game_object = BH::getGameObj(this->leaderID, m_space);

    // if object exists
    if(!l_game_object.expired())
    {
        return l_game_object.lock()->getComponent<transform>()->getPosition();
    }
   
    return vector3();
}

objID Squad::getLeaderID() const
{
    return this->leaderID;
}

GameObjectPtr Squad::requestOpponent(objID unit) const
{
    auto actor = BH::getGameObj(unit, m_space);

    float minDist = 1000000.0f;

    GameObjectPtr closestOpponent;

    // search through detected enemies list and find closest
    for (auto opponentID : m_detected_enemies)
    {
        auto opponent = BH::getGameObj(opponentID, m_space);
        if (BH::NPC_Is_Alive(opponentID, m_space))
        {
            float distance = BH::GetDistance(opponent, actor);
            if (distance < minDist)
            {
                closestOpponent = opponent;
                minDist = distance;
            }
        }
        
    }

    return closestOpponent;
}

bool Squad::isLeader(objID unit) const
{
    return this->leaderID == unit;
}

