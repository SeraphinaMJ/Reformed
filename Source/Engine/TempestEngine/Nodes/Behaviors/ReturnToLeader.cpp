///*!***************************************************************************************
//\file       ReturnToLeader.cpp
//\author     Charles Boudousquie
//\date       10/30/19
//\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
//\par        Project: Boomerang
//\brief      Leaf node that sends actor back to squad leader.
//*****************************************************************************************/
//#include "ReturnToLeader.hpp"
//#include "Transform.hpp"
//#include "RigidBody.hpp"
//#include "Allegiance.hpp"
//#include "GameObjectGatherer.hpp"
//
//void ReturnToLeader::Init()
//{
//    GameObjectFiltering::componentTypeSet node_pattern;
//    node_pattern.setType(Allegiance::getType());
//
//    // find player object
//    std::vector<GameObjectPtr> actors = this->getObjectGatherer()->getList(node_pattern);
//
//    bool foundLeader = false;
//
//    Blackboard& blackboard = this->getBlackboard();
//    for (auto otherActor : actors)
//    {
//        // get other actor faction
//        auto theirFaction = otherActor->getComponent<Allegiance>()->getFaction();
//
//        if (theirFaction == Faction::PLAYER)
//        {
//            foundLeader = true;
//            blackboard.setValue("Leader", otherActor);
//            break;
//        }
//    }
//
//    // if we fonud the player aka our leader
//    if (foundLeader)
//    {
//        Leaf::Init();
//    }
//    else
//    {
//        getTask()->SetPhase(BehaviorPhase::DONE);
//        getTask()->SetResult(BehaviorResult::FAILURE);
//    }
//}
//
//void ReturnToLeader::Update(float dt)
//{
//    Blackboard& blackboard = this->getBlackboard();
//    auto actor = getActor();
//    auto leader = blackboard.getValue<GameObjectPtr>("Leader");
//    auto leaderPos = leader->getComponent<transform>()->getPosition();
//
//    auto pos = actor->getComponent<transform>()->getPosition();
//
//    auto directionToLeader = leaderPos - pos;
//
//    float distance = directionToLeader.distance();
//
//    if (distance < 10.0f)
//    {
//        getTask()->SetPhase(BehaviorPhase::DONE);
//        getTask()->SetResult(BehaviorResult::SUCCESS);
//    }
//    else
//    {
//        auto rigid_Body = actor->getComponent<rigidBody>();
//        // if no rigidbody then something went horribly wrong
//        if (rigid_Body.isNullptr())
//        {
//            std::string message = "Error object: " + actor->getName() + " HAS NO rigidbody";
//            throw std::runtime_error(message);
//        }
//
//        //normalize vector between us and leader
//        directionToLeader = directionToLeader.normalize();
//        // move to enemy
//        rigid_Body->applyImpulse(directionToLeader);
//    }
//
//}
//
