/*!***************************************************************************************
\file       BehaviorTreeBuilder.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      Delegate (aka helper) object that constructs trees for the BehaviorTree Manager.
*****************************************************************************************/
#pragma once

class BehaviorTreeManager;
class BehaviorTree;
class Behavior;

#include <memory>
#include <unordered_map>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Logger.hpp"

class BehaviorTreeBuilder
{
    typedef std::shared_ptr<Behavior> BehaviorPtr;

public:

    void SetManager(BehaviorTreeManager*);
    
    BehaviorTreeBuilder();

    // construct node from name
    std::shared_ptr<BehaviorTree> CreateTree(const std::string & fileName);

private:

    void PrintTree(BehaviorPtr beh, int depth, logger::loggerProxy & log);

    // links nodes after node maps have been filled
    void LinkNodes(std::map<int, std::vector<int>>& nodeLinks, std::map<int, BehaviorPtr>& behaviors);

    BehaviorTreeManager* manager;
};


