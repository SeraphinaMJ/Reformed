/*!***************************************************************************************
\file       BehaviorHelpers.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Functions that various behaviors can use during gameplay, includes
            finding nearby enemies etc.
*****************************************************************************************/
#include "BehaviorHelpers.hpp"
#include "GameObjectGatherer.hpp"
#include "Allegiance.hpp"
#include "Transform.hpp"
#include "RigidBody.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "MinionAll.hpp"

#include "../Formations/Squad.hpp"

#include <vector>
#include <set>

namespace BH
{

    bool NPC_Is_Alive(objID npc, space* sp)
    {
        return sp->getScene()->findGameObjectByID(npc) != nullptr;
    }

    GameObjectPtr getGameObj(objID id, space *sp)
    {
        return sp->getScene()->findGameObjectByID(id);
    }

    vector3 vecBetween(GameObjectPtr A, GameObjectPtr B)
    {
        if(!A.expired() && !B.expired())
        {
            auto trans_A = A.lock()->getComponent<transform>();
            auto trans_B = B.lock()->getComponent<transform>();

            return (trans_B->getPosition() - trans_A->getPosition());
        }

        return vector3();
    }

    float GetDistance(GameObjectPtr actor, GameObjectPtr other)
    {
        if (!actor.expired() && !other.expired())
        {
            auto actorPos = actor.lock()->getComponent<transform>()->getPosition();
            auto otherPos = other.lock()->getComponent<transform>()->getPosition();

            return (actorPos - otherPos).distance();
        }

        return 0.0f;
    }

    float GetDistance(GameObjectPtr actor, const vector3 & otherPos)
    {
        if(!actor.expired())
        {
            auto actorPos = actor.lock()->getComponent<transform>()->getPosition();

            return (actorPos - otherPos).distance();
        }

        return 0.0f;
    }

    float GetDistanceSquared(GameObjectPtr actor, GameObjectPtr other)
    {
        if (!actor.expired() && !other.expired())
        {
            auto actorPos = actor.lock()->getComponent<transform>()->getPosition();
            auto otherPos = other.lock()->getComponent<transform>()->getPosition();

            return (actorPos - otherPos).distanceSquared();
        }

        return 0.0f;
    }

    //GameObjectPtr GetClosestEnemy(GameObjectPtr actor, Squad* squad /*, gameObjectGatherer *GO_gatherer*/)
    //{
    //    //GameObjectFiltering::componentTypeSet node_pattern;
    //    //node_pattern.setType(Allegiance::getType());
    //    //
    //    //std::vector<std::shared_ptr<gameObject>> actors = GO_gatherer->getList(node_pattern);
    //    //Faction myFaction = actor->getComponent<Allegiance>()->getFaction();
    //
    //    GameObjectPtr closestEnemy = nullptr;
    //
    //    // ask squad about who is closest
    //    return squad->getClosestEnemy(actor->getID());
    //
    //    //vector3 myPos = actor->getComponent<transform>()->getPosition();
    //
    //    //float closestDistance = 1000000;
    //    //for (auto otherActor : actors)
    //    //{
    //    //    Faction theirFaction = otherActor->getComponent<Allegiance>()->getFaction();
    //    //    
    //    //    // if confirmed to be enemy
    //    //    if (myFaction != theirFaction &&   /*!IsAlly(myFaction, theirFaction)*/ theirFaction != Faction::NONE)
    //    //    {
    //    //        // get their position
    //    //        auto enemyPos = otherActor->getComponent<transform>()->getPosition();
    //    //        auto distance = (myPos - enemyPos).distance();
    //    //        if (distance < closestDistance)
    //    //        {
    //    //            closestDistance = distance;
    //    //            closestEnemy = otherActor;
    //    //        }
    //    //    }
    //    //}
    //
    //    return closestEnemy;
    //}

    GameObjectPtr GetPlayer(gameObjectGatherer *GO_gatherer)
    {
        GameObjectFiltering::componentTypeSet node_pattern;
        node_pattern.setType(player::getType());

        GameObjectPtr player_obj = *(GO_gatherer->getList(node_pattern).begin());

        return player_obj;
    }

    vector3 getPosition(GameObjectPtr A)
    {
        if(!A.expired())
        {
            return A.lock()->getComponent<transform>()->getPosition();
        }

        return vector3();
    }


    bool IsPlayer(GameObjectPtr actor)
    {
        if(!actor.expired())
        {
            return actor.lock()->getComponent<player>();
        }
        return false;
    }

    bool IsEnemy(GameObjectPtr actor)
    {
        if (!actor.expired())
        {
            return actor.lock()->getComponent<enemy>();
        }
        return false;
    }

    bool IsMinion(GameObjectPtr actor)
    {
        if (!actor.expired())
        {
            return actor.lock()->getComponent<minionAll>();
        }
        return false;
    }
}
