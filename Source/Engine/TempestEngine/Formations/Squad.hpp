/*!***************************************************************************************
\file       Squad.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents the formation of a squad within the game.
*****************************************************************************************/
#pragma once
#include <vector>
#include <memory>
#include <set>
#include "SquadRing.hpp"
#include "DataTypes.hpp"
#include "Factions.hpp"

class space;
enum class FormationState { BROKEN, FORMING, COMPLETE };

class FormationsManager;
class waypointGraphPathfinder;

typedef std::shared_ptr<SquadRing> SquadRingPtr;

#define BASE_SQUAD_SIZE 8

// a group of cooperative npcs that stick close together
class Squad
{
protected:

    space* m_space;
    FormationsManager* formManager;

    // container for units that have been spotted
    std::set<objID> m_detected_enemies;

    virtual SquadRingPtr getRingFromID(objID) const = 0;
    
    // checks if unit is actually within detection radius
    bool withinDetectionRadius(objID) const;

    // removes dead enemies from detected enemies list
    void deleteDeadEnemies();

    // remove out of range enemies from the detected enemies list
    void deleteOutOfRangeEnemies();

    // find enemies within detection radius
    virtual void scoutArea() = 0;

    

    // check if leader still exists
    bool leaderAlive() const;

    objID leaderID;
    Faction m_faction;
    bool m_engaged;
    float m_detection_radius;

public:

    // get all units in squad that are alive
    virtual std::vector<objID> getAllUnits() const = 0;

    Squad(space* space_, FormationsManager* fm, Faction);

    waypointGraphPathfinder* getPathFinder();

    const std::set<objID>& getDetectedEnemies() const;

    bool hasDetectedHostile(objID) const;

    Faction getFaction() const;
    vector3 getCenter() const;
    objID getLeaderID() const;
    GameObjectPtr requestOpponent(objID) const;

    bool isLeader(objID) const;

    // is everyone in the squad dead?
    bool isDead() const;
    // is squad currently fighting opponents
    bool isEngaged() const;

    // checks if unit is in correct position within the formation
    bool InPosition(objID);

    // kill all units in squad
    //virtual void kill() = 0;

    // checks if unit is actually in this squad
    virtual bool hasUnit(objID) const = 0;
    // returns false if squad is full
    virtual bool full() const = 0;
    virtual bool empty() const = 0;

    // add new unit to squad
    virtual bool joinSquad(objID) = 0;
    // remove unit from squad
    virtual void leaveSquad(objID) = 0;

    virtual void Update(float dt) = 0;

};






