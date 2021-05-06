/*!***************************************************************************************
\file       FormationsManager.cpp
\author     Charles Boudousquie
\date       11/19/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Contains all squads within the game.
*****************************************************************************************/
#include "FormationsManager.hpp"
#include "GameObject.hpp"
#include "Allegiance.hpp"
#include "Factions.hpp"
#include "Logger.hpp"

#include "EnemySquad.hpp"
#include "PlayerSquad.hpp"

#include "MinionAll.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

#include "BehaviorSystem/BehaviorHelpers.hpp"

#include "GameObjectGatherer.hpp"

void FormationsManager::disperseDeadSquads()
{
    // eliminate dead squads, except player's
    for (auto it = squads.begin(); it != squads.end();)
    {
        if (/*(*it)->isDead()*/(*it)->empty())
        {

            // remove squad from list
            it = squads.erase(it);
        }
        else
        {
            it++;
        }
    }

}

//void FormationsManager::CreateSquad(objID leaderUnit)
//{
//    
//
//    // get faction of unit
//    auto leader = this->getGameObjectByID(leaderUnit);
//    
//    // get space
//    auto space = leader->getSpace();
//
//    if (isMinion(leader) || isPlayer(leader))
//    {
//        auto l_player = this->GetPlayer();
//        // if player is alive
//        if (l_player != nullptr)
//        {
//            squads[leaderUnit] = std::make_shared<PlayerSquad>(space, this);
//        }
//        else
//        {
//            // otherwise failure
//            return;
//        }
//        
//    }
//    else
//    {
//        squads[leaderUnit] = std::make_shared<EnemySquad>(space, this);
//    }
//
//    squads[leaderUnit]->joinSquad(leaderUnit);
//    
//}

std::vector<std::shared_ptr<gameObject>> FormationsManager::GetMinions()
{
    // get list of enemies and check if they are within detection radius
    GameObjectFiltering::componentTypeSet l_pattern;
    l_pattern.setType(minionAll::getType());

    return this->getSystemManager()->getGameObjectGatherer()->getList(l_pattern);
}

std::vector<std::shared_ptr<gameObject>> FormationsManager::GetEnemies()
{
    // get list of enemies and check if they are within detection radius
    GameObjectFiltering::componentTypeSet l_pattern;
    l_pattern.setType(enemy::getType());

    return this->getSystemManager()->getGameObjectGatherer()->getList(l_pattern);
}

GameObjectPtr FormationsManager::GetPlayer()
{
    // get list of enemies and check if they are within detection radius
    GameObjectFiltering::componentTypeSet l_pattern;
    l_pattern.setType(player::getType());

    auto l_list = this->getSystemManager()->getGameObjectGatherer()->getList(l_pattern);

    if (l_list.empty())
    {
        return std::weak_ptr<gameObject>();
    }
    else
    {
        return l_list[0];
    }
}

const std::string & FormationsManager::name() const
{
    return getName();
}

void FormationsManager::onUpdate()
{
    disperseDeadSquads();
    
    // update all squads
    for (auto squad : squads)
    {
        squad->Update(this->getDt());
    }
}

//void FormationsManager::joinSquad(objID unit)
//{
//    
//    // get faction of unit
//    auto actor = this->getGameObjectByID(unit);
//    
//    // get faction
//    auto faction = getFaction(actor);
//    
//    bool squadFound = false;
//    
//    // find an allied squad
//    for(auto squad : squads)
//    {
//        // if allied with squad
//        if(faction == squad.second->getFaction())
//        {
//            // if squad has room
//            if(!squad.second->full())
//            {
//                squadFound = true;
//                squad.second->joinSquad(unit);
//                break;
//            }
//                
//        }
//    }
//    
//    // if squad was not found then we create our own
//    if(!squadFound)
//    {
//        // create and append squad to back
//        CreateSquad(unit);
//    }
//}
//
//void FormationsManager::leaveSquad(objID unit)
//{
//    // get faction of unit
//    auto actor = this->getGameObjectByID(unit);
//    
//    // get faction
//    auto faction = getFaction(actor);
//    
//    bool squadFound = false;
//    
//    for(auto squad : squads)
//    {
//        if(faction == squad.second->getFaction())
//        {
//            
//            if(squad.second->hasUnit(unit))
//            {
//                squadFound = true;
//                squad.second->leaveSquad(unit);
//                break;
//            }
//                
//        }
//    }
//    
//    // if we tried to leave a squad that we were not a part of then report this error
//    if(!squadFound)
//    {
//        logger log("Formations Manager");
//        
//        auto log_error = log.error();
//        log_error << " Actor: " << actor->getName() << ", With faction: " << FactionToName(faction) << std::endl;
//        log_error << " tried to leave his squad, but there was no squad containing him." << std::endl;
//    }
//}

SquadPtr FormationsManager::createSquad(Faction faction)
{
    SquadPtr newSquad = nullptr;
    auto player = this->GetPlayer().lock();
    
    if (player)
    {
        auto ourspace = this->GetPlayer().lock()->getSpace();

        if (faction == Faction::PLAYER)
        {
            newSquad = std::make_shared<PlayerSquad>(ourspace, this);
        }
        else
        {
            newSquad = std::make_shared<EnemySquad>(ourspace, this);
        }

        squads.push_back(newSquad);
    }

    return newSquad;
}

std::weak_ptr<Squad> FormationsManager::getSquad(objID id)
{
    for (auto squad : squads)
    {
        if (squad->hasUnit(id))
        {
            return squad;
        }
    }

    return std::weak_ptr<Squad>(); // nullptr;
}

void FormationsManager::onLevelLoad(const levelLoadEvent &)
{
    this->squads.clear();
}

void FormationsManager::onShutdown()
{
    this->squads.clear();
}

//void FormationsManager::PrintAllSquads() const
//{
//    logger log("FormationsManager");
//
//    for (auto squad : squads)
//    {
//        auto units = squad->getAllUnits();
//
//        auto proxy = log.info();
//
//        for (auto unit : units)
//        {
//            proxy << unit << " ";
//        }
//        proxy << std::endl;
//    }
//}

















