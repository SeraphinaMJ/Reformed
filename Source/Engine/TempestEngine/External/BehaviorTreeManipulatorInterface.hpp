/*!***************************************************************************************
\file       BehaviorTreeManipulatorInterface.hpp
\author     Charles Boudousquie
\date       10/11/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Interface of the manipulator that talks to the BehaviorTreeManager
*****************************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <memory>

class gameObject;
typedef std::weak_ptr<gameObject> GameObjectPtr;

class behaviorTreeManipulatorInterface
{

public:
    virtual int getActiveNodeID(GameObjectPtr agent) const = 0;
    virtual std::vector<std::shared_ptr<gameObject>> getObjectsWithAgents() = 0;
    virtual void markTreeChanged(const std::string&) = 0;
};
