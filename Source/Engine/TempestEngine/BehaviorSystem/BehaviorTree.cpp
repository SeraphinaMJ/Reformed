/*!***************************************************************************************
\file       BehaviorTree.cpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the object contains Behavior Nodes.
*****************************************************************************************/
#include "BehaviorTree.hpp"
#include "Nodes/Behaviors/Behavior.hpp"
#include "Agent.hpp"

void BehaviorTree::SetUpTree(BehaviorPtr root_)
{
    root = root_;
    RecursiveSetParentTree(root.get());
}

void BehaviorTree::SetManager(BehaviorTreeManager *manager_)
{
    manager = manager_;
}

BehaviorTreeManager * BehaviorTree::GetManager()
{
    return manager;
}

BehaviorPtr BehaviorTree::GetRoot()
{
    return root;
}

BehaviorTree::BehaviorTree(){}

void BehaviorTree::SetName(const std::string & newName)
{
    name = newName;
}

std::string BehaviorTree::GetName()
{
    return this->name;
}

void BehaviorTree::RecursiveSetParentTree(Behavior* behavior)
{

    behavior->setParentTree(this);

    auto children = behavior->GetChildren();

    for (auto child : children)
    {
        RecursiveSetParentTree(child.get());
    }
}
