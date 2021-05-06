/*!***************************************************************************************
\file       AllBehaviorsInclude.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      File for including every single behavior that is used in game.
*****************************************************************************************/
#pragma once

//# leaves
#include "Nodes/Behaviors/Leaf.hpp"
#include "Nodes/Behaviors/AttackClosestEnemy.hpp"
#include "Nodes/Behaviors/DefaultLeaf.hpp"
#include "Nodes/Behaviors/GoToEnemy.hpp"
#include "Nodes/Behaviors/JoinSquad.hpp"
#include "Nodes/Behaviors/JumpUp.hpp"
#include "Nodes/Behaviors/Repeater.hpp"
#include "Nodes/Behaviors/ReturnToLeader.hpp"
#include "Nodes/Behaviors/PickEnemyToFight.hpp"

//#Decorators
#include "Nodes/Behaviors/Decorator.hpp"
#include "Nodes/Behaviors/EnemyWithinRange.hpp"
#include "Nodes/Behaviors/EnemySpotted.hpp"
#include "Nodes/Behaviors/HasSquad.hpp"
#include "Nodes/Behaviors/IsSquadEngaged.hpp"

#include "Nodes/Behaviors/Inverter.hpp"
#include "Nodes/Behaviors/IsDead.hpp"
#include "Nodes/Behaviors/IsLowHealth.hpp"
#include "Nodes/Behaviors/Timer.hpp"
#include "Nodes/Behaviors/ReturnTrue.hpp"
#include "Nodes/Behaviors/RepeatUntilN.hpp"
#include "Nodes/Behaviors/RepeatUntilFailure.hpp"
#include "Nodes/Behaviors/RepeatUntilSuccess.hpp"

//#Composites
#include "Nodes/Behaviors/Composite.hpp"
#include "Nodes/Behaviors/Selector.hpp"
#include "Nodes/Behaviors/Sequencer.hpp"
