/*!***************************************************************************************
\file       BehaviorTreeManager.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the Manager that controls any behavior tree an agent may use.
*****************************************************************************************/
#pragma once
#include <SystemBase.hpp>
#include <set>
#include <memory>

class BehaviorTree;
class BehaviorTreeBuilder;

//class gameObject;
//typedef std::weak_ptr<gameObject> GameObjectPtr;

class BehaviorTreeManager final : public systemBase
{
    typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;
    typedef std::shared_ptr<BehaviorTreeBuilder> BehaviorTreeBuilderPtr;

    // there should only be 1 tree builder
    BehaviorTreeBuilderPtr builder;

    std::unordered_map<std::string, BehaviorTreePtr> trees;

    // trees that need to be reloaded, because user modified them
    std::set<std::string> treesToReload;

    // gets tree if it exists otherwise returns nullptr
    BehaviorTreePtr GetTree(const std::string&);

    void ReloadTrees();

    friend class Behavior;

public:

    /*!*******************************************************************************
    \brief  Default constructor
    *********************************************************************************/
    BehaviorTreeManager();

    /*!*******************************************************************************
    \brief  Retrieves the type of component this instance is. SceneSystem requirement.
    \return componentType - The type of this component.
    *********************************************************************************/
    const std::string& name() const override;

    /*!*******************************************************************************
    \brief  Returns name of this system.
    *********************************************************************************/
    static const std::string& getName()
    {
        static const std::string n("BehaviorTreeManager"); return n;
    }

    /*!***************************************************************************************
    \brief  Updates all trees.
    *****************************************************************************************/
    void onUpdate() override;

    /*!***************************************************************************************
    \brief  Tells treebuilder to build trees from disk
    *****************************************************************************************/
    void onInitialize()override;
    
    // returns all game objects that have an agent component
    std::vector<std::shared_ptr<gameObject>> getAgentGameObjects();

    gameObjectGatherer* getGO_Gatherer();

    // returns id of node that an agent is working on
    int getCurrentNodeID(std::weak_ptr<gameObject>);

    // if user has modified a graph then we will have to reload
    // when running the game again
    void MarkTreeAsChanged(const std::string&);
};


