/*!***************************************************************************************
\file       FormationsManager.hpp
\author     Charles Boudousquie
\date       11/19/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Contains all squads within the game.
*****************************************************************************************/
#pragma once
#include "Squad.hpp"

#include <SystemBase.hpp>
#include <memory>
#include <vector>
#include <unordered_map>

typedef std::shared_ptr<Squad> SquadPtr;

class FormationsManager final : public systemBase
{
        
    public:
    

        /*!*******************************************************************************
        \brief  Retrieves the type of component this instance is. SceneSystem requirement.
        \return componentType - The type of this component.
        *********************************************************************************/
        const std::string& name() const override;

        /*!*******************************************************************************
        \brief  Returns name of this system.
        *********************************************************************************/
        static const std::string& getName()
        {
            static const std::string n("Formations Manager"); return n;
        }

        /*!***************************************************************************************
        \brief  Updates all squads, Deletes empty squads(except player's)
        *****************************************************************************************/
        void onUpdate() override;

        // create squad 
        SquadPtr createSquad(Faction);

        std::weak_ptr<Squad> getSquad(objID);

        void onLevelLoad(const levelLoadEvent & /*p_event*/) override;

        void onShutdown() override;

        //void PrintAllSquads() const;

    private:

        // delete squads whose leader is dead,
        // if player dies then kill all minions
        void disperseDeadSquads();

        //void CreateSquad(objID);

        // retrive list of all minions
        std::vector<std::shared_ptr<gameObject>> GetMinions();
        // get list of all enemies
        std::vector<std::shared_ptr<gameObject>> GetEnemies();
        // get player
        GameObjectPtr GetPlayer();


        friend class EnemySquad;
        friend class Squad;
        friend class PlayerSquad;

        // map of squad leaders to squads
        std::vector<SquadPtr> squads;
        //std::unordered_map<objID, SquadPtr> squads;
};



