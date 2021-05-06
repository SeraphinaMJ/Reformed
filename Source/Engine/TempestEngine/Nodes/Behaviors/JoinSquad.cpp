///*!***************************************************************************************
//\file       JoinSquad.cpp
//\author     Charles Boudousquie
//\date       11/22/2019
//\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
//\par        Project: Boomerang
//\brief      Tells unit to join squad if not in one.
//*****************************************************************************************/
//#include "JoinSquad.hpp"
//#include "Formations/FormationsManager.hpp"
//
//void JoinSquad::Update(float)
//{
//    UpdateDebug();
//
//    auto squadManager = getFormationsManager();
//
//    auto task = getTask();
//
//    // join squad (or make new one)
//    if (squadManager)
//    {
//        squadManager->joinSquad(getActor()->getID());
//        task->SetPhase(BehaviorPhase::DONE);
//        task->SetResult(BehaviorResult::SUCCESS);
//    }
//    else
//    {
//        task->SetPhase(BehaviorPhase::DONE);
//        task->SetResult(BehaviorResult::FAILURE);
//    }
//    
//}
//
