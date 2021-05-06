/*!***************************************************************************************
\file       BehaviorHelpers.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Functions that various behaviors can use during gameplay, includes
            finding nearby enemies etc.
*****************************************************************************************/
#pragma once
#include <memory>
#include "GameObject.hpp"
#include "Vector3.hpp"
#include "Factions.hpp"

typedef std::weak_ptr<gameObject> GameObjectPtr;

class gameObjectGatherer;
class Squad;

namespace BH
{
    // check if said character still exists in space
    bool NPC_Is_Alive(objID npc, space*);

    GameObjectPtr getGameObj(objID id, space*); //  todo

    // B - A
    vector3 vecBetween(GameObjectPtr A, GameObjectPtr B);

    // get distance between actor and some other npc
    float GetDistance(GameObjectPtr actor, GameObjectPtr other);
    float GetDistance(GameObjectPtr actor, const vector3 & otherPos);

    float GetDistanceSquared(GameObjectPtr actor, GameObjectPtr other);

    GameObjectPtr GetPlayer(gameObjectGatherer *GO_gatherer);

    vector3 getPosition(GameObjectPtr A);

    bool IsPlayer(GameObjectPtr);

    bool IsEnemy(GameObjectPtr);

    bool IsMinion(GameObjectPtr);
}


