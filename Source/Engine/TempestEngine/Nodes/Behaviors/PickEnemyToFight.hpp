/*!***************************************************************************************
\file       PickEnemyToFight.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This behavior chooses who to fight from a list of enemies detected by the squad.
*****************************************************************************************/
#include "Leaf.hpp"

class PickEnemyToFight : public Leaf
{
    public:

        void Update(float) override;
};

