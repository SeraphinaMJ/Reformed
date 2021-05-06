/*!***************************************************************************************
\file       JumpUp.cpp
\author     Charles Boudousquie
\date       10/18/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node makes the character jump upwards.
*****************************************************************************************/
#include "JumpUp.hpp"
#include "RigidBody.hpp"

void JumpUp::Update(float)
{
    UpdateDebug();

    auto actor = getActor();
    auto rigid = actor->getComponent<rigidBody>();

    // if no rigidbody then something went horribly wrong
    if (!rigid)
    {
        std::string message = "Error object: " + actor->getName() + " HAS NO rigidbody";
        throw std::runtime_error(message);
    }

    rigid->applyImpulse({ 0,500,0 });
   // rigid->applyForce({ 0,10000,0 });
    //rigid->setVelocity({ 0,600,0 });

    getTask()->SetResult(BehaviorResult::SUCCESS);
    getTask()->SetPhase(BehaviorPhase::DONE);
}
