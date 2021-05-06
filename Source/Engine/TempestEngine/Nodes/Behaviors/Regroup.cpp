///*!***************************************************************************************
//\file       Regroup.cpp
//\author     Charles Boudousquie
//\date       11/19/2019
//\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
//\par
//\brief      This behavior tells npc to regroup with some squad.
//*****************************************************************************************/
//#include "Regroup.hpp"
//#include "Formations/FormationsManager.hpp"
//
//#include "BehaviorSystem/BehaviorHelpers.hpp"
//
//#include "PathFollowScript.hpp"
//
//
//
//void Regroup::Init()
//{
//    auto formManager = this->getFormationsManager();
//    auto actor = getActor();
//
//    // SEE if we can join a squad or make a new one
//    formManager->joinSquad(actor->getID());
//
//    Leaf::Init();
//}
//
//void Regroup::Update(float)
//{
//    auto actor = getActor();
//
//    // get position of squad
//    auto formManager = this->getFormationsManager();
//    auto squad = formManager->getSquad(actor->getID());
//
//    // if unit is in appropriate position
//    if (squad->InPosition(actor->getID()))
//    {
//        getTask()->SetPhase(BehaviorPhase::DONE);
//        getTask()->SetResult(BehaviorResult::SUCCESS);
//    }
//
//    // else assume squad object is already pushing object to its correct position
//}
//
