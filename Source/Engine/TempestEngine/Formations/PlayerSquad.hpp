/*!***************************************************************************************
\file       PlayerSquad.hpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Structure containing player's minions in game.
*****************************************************************************************/
#pragma once
#include "Squad.hpp"

class PlayerSquad final : public Squad
{

    std::vector<SquadRingPtr> m_rings;

    void scoutArea() override;

    // get all units in squad that are alive
    std::vector<objID> getAllUnits() const override;

    void insertUnit(objID);

    void promoteToLeader(objID);

public:

    ~PlayerSquad();

    PlayerSquad(space* space_, FormationsManager* fm);

    // get squad ring that contains unit
    SquadRingPtr getRingFromID(objID unit) const override;

    //void kill() override;

    bool hasUnit(objID) const override;

    bool full() const override;
    bool empty() const override;

    bool joinSquad(objID) override;
    void leaveSquad(objID) override;

    void Update(float) override;

};

