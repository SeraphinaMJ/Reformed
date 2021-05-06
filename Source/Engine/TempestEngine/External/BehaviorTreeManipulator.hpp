/*!***************************************************************************************
\file       BehaviorTreeManipulator.hpp
\author     Charles Boudousquie
\date       10/11/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manipulator to talk to the BehaviorTreeManager
*****************************************************************************************/
#pragma once
#include "BehaviorTreeManipulatorInterface.hpp"

class systemManagerInterface;

class BehaviorTreeManipulator : public behaviorTreeManipulatorInterface
{
    systemManagerInterface* m_system_manager;
public:

    BehaviorTreeManipulator(systemManagerInterface*);

    // get active node in BehaviorManager system
    int getActiveNodeID(GameObjectPtr agent) const override;

    // get all game objects that have an agent component
    std::vector<std::shared_ptr<gameObject>> getObjectsWithAgents() override;

    // call this when user edits the variables of a Behavior Node
    // while using the Node Graph Editor
    void markTreeChanged(const std::string&) override;

    virtual ~BehaviorTreeManipulator() {}

};

