/*!***************************************************************************************
\file       Behavior.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is the abstract class from which all behaviors derive from.
*****************************************************************************************/
#pragma once

#include "Nodes/Node.hpp"
#include "../BehaviorSystem/BehaviorEnums.hpp"
#include "../BehaviorSystem/BehaviorTask.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../BehaviorSystem/Blackboard.hpp"

#include <rapidjson.h>
#include "document.h"

//#define DEBUGGING_NODES

#ifdef DEBUGGING_NODES
#include "BehaviorLogger.hpp"

enum class DEBUG_MESSAGE_TYPE{INIT, UPDATE, EXIT};
#endif

class Squad;
class Behavior;
class BehaviorTree;
class FormationsManager;
class waypointGraphPathfinder;

typedef std::shared_ptr<Squad> SquadPtr;
typedef std::shared_ptr<Behavior> BehaviorPtr;
typedef std::shared_ptr<BehaviorTask> BehaviorTaskPtr;

class Behavior : public Node
{

#ifdef DEBUGGING_NODES
    // printing functions
    void printDebug(DEBUG_MESSAGE_TYPE type);
#endif

protected:

    void InitDebug();
    void UpdateDebug();
    void ExitDebug();


    // what node owns us
    BehaviorPtr parent;

    // behavior tree that owns this particular behavior
    BehaviorTree* behaviorTreeOwner;

    // can be overriden
    virtual void Init() = 0;
    // MUST be overriden
    virtual void Update(float dt) = 0;
    // by default is just hands task to parent (it calls GiveToParent)
    virtual void Exit();

    // shorthand to get task, it
    // is essentially a helper function to get 
    // task from parent tree
    BehaviorTaskPtr getTask();

    // this function gets the actor/npc that is
    // currently working with this node
    std::shared_ptr<gameObject> getActor();

    // game container of game objects
    gameObjectGatherer* getObjectGatherer();

    space* getCurrentSpace();

    // get reference to local knowledge base
    Blackboard& getBlackboard();

    // get formations manager if behavior needs it
    FormationsManager* getFormationsManager();

    // retrieve squad that unit is in
    std::weak_ptr<Squad> getSquad();

    // get waypoint path finder
    waypointGraphPathfinder* getPathFinder();

    GameObjectPtr getClosestEnemy();

public:
    // These 3 functions are meant to pass around a task within the 
    // tree. The behaviors or the manager should call TakeTask, GiveToChild, and GiveToParent
    void TakeTask(BehaviorTaskPtr);
    void GiveToChild(BehaviorTaskPtr);
    void GiveToParent(BehaviorTaskPtr);

    typeRT toTypeRT() const override;

    virtual BehaviorType GetType() = 0;

    Behavior(); // default constructor

    // get currently active child of said node
    virtual BehaviorPtr getCurrentChild() = 0;

    // get all child nodes
    virtual std::vector<BehaviorPtr> GetChildren() = 0;

    void setParentTree(BehaviorTree*);

    // sets parent of behavior
    void setParent(BehaviorPtr);

    // retrieve parent node, there should only be 1 parent
    BehaviorPtr getParent();

    // based on what phase we are in, this will call Init, Update, or Exit.
    // A behavior should never call tick, only the agent does
    void tick(float dt);

    // not sure what to do with this one ask ryan
    virtual std::pair<bool, std::string> validate() override { return {}; }

    // Update node's data based on info given, by default does nothing
    // NOTE: function not used by behaviors
    virtual void updateFromTypeRT(typeRT & p_data) override ;

    // update Behavior's special data based on info from json
    // (does nothing by default)
    virtual void updateFromFile(const rapidjson::Value &);

    // decorator has 1 chid, composite have multiple, leaves have none
    virtual void addChild(BehaviorPtr) = 0;

};
