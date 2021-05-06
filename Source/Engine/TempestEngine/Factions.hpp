/*!***************************************************************************************
\file       Factions.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Here is the enum class to represent what faction an npc is a part of.
*****************************************************************************************/
#pragma once
#include <map>
#include <memory>

class gameObject;
typedef std::weak_ptr<gameObject> GameObjectPtr;

enum class Faction { PLAYER = 0, ENEMY, NONE, COUNT };

const char* FactionToName(const Faction& faction);

bool isMinion(GameObjectPtr unit);

bool isEnemy(GameObjectPtr unit);

bool isPlayer(GameObjectPtr unit);

Faction getFaction(GameObjectPtr unit);

bool IsAlly(const Faction& factionA, const Faction& factionB);

const std::map<Faction, const char*>& GetFactionList();

