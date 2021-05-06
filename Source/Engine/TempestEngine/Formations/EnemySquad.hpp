/*!***************************************************************************************
\file       EnemySquad.hpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Structure containing enemy units in game.
*****************************************************************************************/
#pragma once
#include "Squad.hpp"
#include <set>

class EnemySquad final : public Squad
{
    // only 1 ring for enemy squad
    SquadRingPtr m_ring;

    void scoutArea() override;

    // get all units in squad that are alive
    std::vector<objID> getAllUnits() const override;

    void promoteToLeader(objID);

public:    

    ~EnemySquad() {}

    EnemySquad(space* space_, FormationsManager* fm);

    SquadRingPtr getRingFromID(objID) const override;

    //void kill() override;
    bool hasUnit(objID) const override;

    bool full() const override;
    bool empty() const override;

    bool joinSquad(objID) override;
    void leaveSquad(objID) override;

    void Update(float) override;

};


