/*!***************************************************************************************
\file       BehaviorTree.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the object contains Behavior Nodes.
*****************************************************************************************/
#pragma once
#include <memory>

class BehaviorTask;
class Behavior;
class BehaviorTreeManager;

typedef std::shared_ptr<BehaviorTask> BehaviorTaskPtr;
typedef std::shared_ptr<Behavior> BehaviorPtr;

class BehaviorTree
{

    // main node of the tree
    BehaviorPtr root;

    // name of tree
    std::string name;

    // for each behavior set its parent tree to this
    void RecursiveSetParentTree(Behavior*);

    // owning manager
    BehaviorTreeManager* manager;
public:
    void SetManager(BehaviorTreeManager*);
    BehaviorTreeManager* GetManager();

    BehaviorPtr GetRoot();

    BehaviorTree();

    void SetName(const std::string& newName);
    std::string GetName();
    
    // give the tree its root node
    void SetUpTree(BehaviorPtr);

};

