/*!***************************************************************************************
\file       Factions.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Here is the enum class to represent what faction an npc is a part of.
*****************************************************************************************/
#include "Factions.hpp"
#include "GameObject.hpp"
#include <unordered_map>
#include <set>

#include "ComponentLists.h"

#include "Enemy.hpp"
#include "MinionAll.hpp"
#include "Player.hpp"

static std::map<Faction, const char*> factionNames
{
    {Faction::PLAYER, "PLAYER"},
    {Faction::ENEMY, "ENEMY"}
};

const std::map<Faction, const char*>& GetFactionList()
{
    return factionNames;
}

const char* FactionToName(const Faction& faction)
{
    if(factionNames.find(faction) != factionNames.end())
    {
        return factionNames[faction];
    }
    
    throw std::runtime_error("Tried to acces name of faction that did not exist");
}

bool IsAlly(const Faction&  factionA, const Faction&  factionB)
{
    return factionA == factionB;   
}

bool isMinion(GameObjectPtr unit)
{
    if(!unit.expired())
    {
        return unit.lock()->getComponent<minionAll>();
    }
    return false;
}

bool isEnemy(GameObjectPtr unit)
{
    if (!unit.expired())
    {
        return unit.lock()->getComponent<enemy>();
    }
    return false;
}

bool isPlayer(GameObjectPtr unit)
{
    if (!unit.expired())
    {
        return unit.lock()->getComponent<player>();

    }
    return false;
}

Faction getFaction(GameObjectPtr unit)
{
    if (isEnemy(unit))
    {
        return Faction::ENEMY;
    }
    else if (isPlayer(unit) || isMinion(unit))
    {
        return Faction::PLAYER;
    }
    else
    {
        return Faction::NONE;
    }

}
