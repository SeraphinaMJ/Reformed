///*!***************************************************************************************
//\file       BreakFormation.cpp
//\author     Charles Boudousquie
//\date       11/22/2019
//\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
//\par
//\brief      This behavior tells npc to break away from his squad.
//*****************************************************************************************/
//#include "BreakFormation.hpp"
//#include "Formations/FormationsManager.hpp"
//#include "PathFollowScript.hpp"
//
//void BreakFormation::Init()
//{
//    auto formManager = this->getFormationsManager();
//
//    auto actor = getActor();
//
//    // SEE if we can join a squad or make a new one
//    formManager->leaveSquad(actor->getID());
//
//    // get script component
//    auto pathingScript = actor->getComponent<pathFollowScript>();
//
//    // tell scripts to stop following path
//    pathingScript->StopPathing();
//
//    Leaf::Init();
//}
//
//void BreakFormation::Update(float)
//{
//
//    getTask()->SetPhase(BehaviorPhase::DONE);
//    getTask()->SetResult(BehaviorResult::SUCCESS);
//}
