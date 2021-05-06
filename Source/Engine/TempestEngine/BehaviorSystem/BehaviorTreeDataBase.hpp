/*!***************************************************************************************
\file       ActorDatabase.hpp
\author     Charles Boudousquie
\date       10/26/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the class keeps track of which actor has been selected by the 
            Behavior Tree Manager
*****************************************************************************************/
#pragma once
#include <memory>

class gameObject;
typedef std::weak_ptr<gameObject> GameObjectPtr;

class BehaviorTree;
typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;

class BehaviorTreeManager;

class ActorDatabase
{
    ActorDatabase(){}
public:

    static ActorDatabase& GetInstance()
    {
        static ActorDatabase instance;
        return instance;
    }

    // tell data base who current actor is
    void AssignCurrentActor(GameObjectPtr);

    // get actor that is currently selected by BTM
    GameObjectPtr GetCurrentActor();

    // give database a pointer to the behavior manager
    void SetManager(BehaviorTreeManager*);

    BehaviorTreeManager* GetManager();

private:
    // actor that a behavior tree should act upon
    GameObjectPtr currentActor;
    BehaviorTreeManager* btm;
};
