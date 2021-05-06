/*!***************************************************************************************
\file       PlayerControllerScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "PlayerControllerScript.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/InputComponent.hpp"
#include "../Physics/PhysicsSystem.hpp"
#include "AE_Events.hpp"
#include "../Audio/AudioSystem.h"
#include "EnemyAIScript.h"
#include <Logger.hpp>
#include <MathFunctions.hpp>
#include <GamepadControlDefines.hpp>

int playerControllerScript::s_totalPlayerNumber = 0;

playerControllerScript::playerControllerScript()
{
	
}

playerControllerScript::~playerControllerScript()
{
	--s_totalPlayerNumber;
}

playerControllerScript::playerControllerScript(typeRT p_data) :scriptCPP(p_data)
{
  updateFromTypeRT(p_data);
}

typeRT playerControllerScript::toTypeRT() const
{
	//Generate the base script data
	typeRT l_result = scriptCPP::toTypeRT();
	//typeRT l_scriptData = l_result.member();

  typeRT l_playerNumber("playerNumber", m_playerNumber + 1);
  l_result.insertMember(l_playerNumber);

	//add data to the l_scriptData object here.
	//l_scriptData.insertMember(typeRT("jumpStrength", m_JumpStrength));

	//re-insert the data back into the base data.
	//l_result.insertMember(l_scriptData);
	return l_result;
}

void playerControllerScript::updateFromTypeRT(typeRT& p_type)
{
	//This is attempting to put variables in the editor. Currently broken!
	auto l_members = p_type.members();

	if (l_members.find("jumpStrength") != l_members.end())
	{
		m_JumpStrength = p_type.member("jumpStrength").getFloat();
	}
  if (l_members.find("playerNumber") != l_members.end())
  {
    m_playerNumber = p_type.member("playerNumber").getInt() - 1;
  }
}

void playerControllerScript::onStart()
{
	// "Respawn position"
	//m_startPos = getGameObject().lock()->getComponent<transform>()->getPosition();
#undef max
  int l_max = std::max(s_totalPlayerNumber - 1, m_playerNumber);
  if(l_max > s_totalPlayerNumber - 1)
  {
    s_totalPlayerNumber = m_playerNumber + 1;
  }

	//Load sounds
	getSystem<AudioSystem>()->sendAudioEvent(new Audio::LoadEvent("SFX_Jump.node"));

	//Map buttons on input component
	auto l_input_component2 = getComponent<inputComponent>();

	l_input_component2->addButtonMapping("jump", ' ');
	l_input_component2->addGamepadButtonMapping("forward", gpb_dpadUp, m_playerNumber);
	l_input_component2->addGamepadButtonMapping("backward", gpb_dpadDown, m_playerNumber);
	l_input_component2->addGamepadButtonMapping("left", gpb_dpadLeft, m_playerNumber);
	l_input_component2->addGamepadButtonMapping("right", gpb_dpadRight, m_playerNumber);
	l_input_component2->addGamepadButtonMapping("jump", gpb_A, m_playerNumber);
	l_input_component2->addGamepadButtonMapping("dash", gpb_X, m_playerNumber);
	l_input_component2->addGamepadButtonMapping("pound", gpb_B, m_playerNumber);
	l_input_component2->addGamepadButtonMapping("grab", gpb_Y, m_playerNumber);
	l_input_component2->addGamepadButtonMapping("throw", gpb_rightShoulder, m_playerNumber);
	l_input_component2->addGamepadButtonMapping("lariat", gpb_leftShoulder, m_playerNumber);
	l_input_component2->addGamepadAxisMapping("moveX", gpa_thumbLX, m_playerNumber);
	l_input_component2->addGamepadAxisMapping("moveY", gpa_thumbLY, m_playerNumber);
	l_input_component2->addGamepadAxisMapping("trig", gpa_rightTrigger, m_playerNumber);
	l_input_component2->addButtonMapping("lmb", MouseButton::Left);
	l_input_component2->addButtonMapping("rmb", MouseButton::Right);
	l_input_component2->addCursorMapping("mouse", Cursor::Default_Mouse);
	l_input_component2->addControllerStateMapping("gamepadState", 0);
}

void playerControllerScript::onUpdate()
{
	//Handles cooldowns.
	updateTimers();

	//Normal update loop.
	updateDefault();

	// If the player "falls off the stage" (falls below a certain y value), then put them back TODO: needs fixing doesn't work
	/*if (getGameObject().lock()->getComponent<transform>()->getPosition().getY() < -6.f)
	{
		getGameObject().lock()->getComponent<rigidBody>()->setVelocity(vector3(0, 0, 0));
		getGameObject().lock()->getComponent<transform>()->setPosition(m_startPos);
		getGameObject().lock()->getComponent<rigidBody>()->updateBulletTransform();
	}*/
}

void playerControllerScript::onShutdown()
{
	logger("PlayerControllerScript").debug("onshutdown called");

}

void playerControllerScript::updateTimers()
{
	if(m_DashCooldownTimer > 0)
	{
		m_DashCooldownTimer -= dt();
	}

	if (m_JumpAgainTimer > 0)
	{
		m_JumpAgainTimer -= dt();
	}
	if (m_JumpUpwardsMomentumTimer > 0)
	{
		m_JumpUpwardsMomentumTimer -= dt();
	}
}

void playerControllerScript::updateDefault()
{
	//Handle left stick input and movement
	updateMovement();

	//Jumping logic
	if(checkJump())
	{
		m_UpwardsMomentum = true;
		m_JumpUpwardsMomentumTimer = m_JumpUpwardsMomentumTime;

		getSystem<AudioSystem>()->sendAudioEvent(new Audio::PlayEvent(123456, "SFX_Jump.node")); getInput();

		// notify the appropriate timer
		m_JumpAgainTimer = m_JumpAgainWait;
		getGameObject().lock()->getComponent<rigidBody>()->applyForce(vector3(0, m_JumpStrength * m_scaleFactor, 0));		
	}

	//Dashing logic
	if(checkDash())
	{
		vector3 forward = getGameObject().lock()->getComponent<transform>()->getForward();
		getGameObject().lock()->getComponent<rigidBody>()->applyForce(forward * m_DashSpeed * m_scaleFactor);
	}

	if(checkLariat())
	{
		std::vector<std::shared_ptr<gameObject> > l_objects;
		l_objects = getSystemManager()->getSystem<physicsSystem>()->getObjectsWithinRadius(getGameObject().lock()->getComponent<transform>()->getPosition(), m_lariatRadius);

		for (auto i_object : l_objects)
		{
			// Push object away from player
			vector3 direction(0, 0, 0);
			direction = i_object->getComponent<transform>()->getPosition() - getGameObject().lock()->getComponent<transform>()->getPosition();
			direction = direction * m_lariatStrength;
			i_object->getComponent<rigidBody>()->applyForce(direction * m_scaleFactor);

			// If the object is an enemy, damage the enemy
			if(i_object->getComponent<enemyAIScript>())
			{
				//damage
				i_object->getComponent<enemyAIScript>()->setActivationState(false);
			}
		}
	}

	//Grabbing logic
	if(checkGrab())
	{
			auto tform = getGameObject().lock()->getComponent<transform>();
			objID l_grabbed_objID = getSystemManager()->getSystem<physicsSystem>()->getObjectRayCast(tform->getForward() * m_grabDistance, m_grabableLayers, getGameObject().lock());
			if (l_grabbed_objID != 0)
			{
				m_GrabbedObj = findObject(l_grabbed_objID);
				m_grabbing = true;
				// If the object is an enemy, damage the enemy
				if (m_GrabbedObj->getComponent<enemyAIScript>())
				{
					//damage
					m_GrabbedObj->getComponent<enemyAIScript>()->setActivationState(false);
				}
				m_GrabbedObj = findObject(l_grabbed_objID);
				//vector3 myPosition = getGameObject().lock()->getComponent<transform>()->getPosition();
				getGameObject().lock()->addChild(m_GrabbedObj);
				m_GrabbedObj->getComponent<transform>()->setPosition(vector3(0,4,0));
				m_GrabbedObj->getComponent<rigidBody>()->setLocalGravity(vector3());
				m_GrabbedObj->getComponent<rigidBody>()->useGlobalGravity(false);
				//m_GrabbedObj->getComponent<rigidBody>()->setActivationState(0);
			}
	}
	//Throwing logic
	if(checkThrow())
	{
		m_grabbing = false;
		//m_GrabbedObj->getComponent<rigidBody>()->setActivationState(1);
		getGameObject().lock()->removeChild(m_GrabbedObj);
		m_GrabbedObj->getComponent<rigidBody>()->updateBulletTransform();
		m_GrabbedObj->getComponent<rigidBody>()->applyForce(getGameObject().lock()->getComponent<transform>()->getForward() * m_ThrowSpeed * m_scaleFactor);
		m_GrabbedObj = nullptr;
	}
}

//This is to fix a bug where first few frames of input are wrong because the controller hasn't been detected yet.
static float wait_timer = 2.0f;

//Gets left stick input
vector3 playerControllerScript::getInput()
{

	auto ic = getGameObject().lock()->getComponent<inputComponent>();

		if(ic->getAxisState("moveX") == 0 && ic->getAxisState("moveY") == 0)
		{
			return vector3(0, 0, 0);
		}
	
	float xasxis = ic->getAxisState("moveX") - 0.5f;
	float yasxis = -(ic->getAxisState("moveY")- 0.5f);
	float epsilon = 0.1f;
	if(abs(xasxis) < epsilon)
	{
		xasxis = 0;
	}

	if (abs(yasxis) < epsilon)
	{
		yasxis = 0;
	}

	if(ic->getButtonState("jump"))
	{
		checkJump();
	}

	//This is to fix a bug where first few frames of input are wrong because the controller hasn't been detected yet.
	if(wait_timer >= 0)
	{
		wait_timer -= dt();
		return vector3(0, 0, 0);
	}
	return vector3(xasxis, 0, yasxis);
}

void playerControllerScript::updateMovement()
{
	// First, update movement based on input
	m_CurrentLeftJoy = getInput();
	getGameObject().lock()->getComponent<rigidBody>()->applyForce(vector3(m_CurrentLeftJoy.x * m_scaleFactor, 0, m_CurrentLeftJoy.z * m_scaleFactor));

	float l_x = m_CurrentLeftJoy.x;
	float l_y = -m_CurrentLeftJoy.z;

	float angle;
	//find angle between two vectors
	
	angle = RadToDeg(atan2f(0, 1) - atan2f(-l_x,-l_y));


	getComponent<transform>()->setRotation(toQuaternion(vector3(0,angle, 0)));


	//Turn to face the direction we are moving
	updateFacing();
}

void playerControllerScript::updateFacing()
{

}

int playerControllerScript::getTotalPlayerNumber()
{
	return s_totalPlayerNumber;
}

bool playerControllerScript::checkJump()
{
	auto ic = getGameObject().lock()->getComponent<inputComponent>();
	auto rb = getGameObject().lock()->getComponent<rigidBody>();

	return (ic->getButtonState("jump") && rb->isGrounded());
}

bool playerControllerScript::checkDash()
{
	auto ic = getGameObject().lock()->getComponent<inputComponent>();
	if(ic->getButtonState("dash") && m_DashCooldownTimer <= 0)
	{
		m_DashCooldownTimer = m_DashCooldown;
		return true;
	}
	return false;
}

bool playerControllerScript::checkGrab()
{
	auto ic = getGameObject().lock()->getComponent<inputComponent>();
	if (ic->getButtonState("grab") && m_grabbing == false)
	{
		return true;
	}
	return false;
}

bool playerControllerScript::checkLariat()
{
	auto ic = getGameObject().lock()->getComponent<inputComponent>();
	if (ic->getButtonState("lariat") && m_grabbing == false)
	{
		return true;
	}
	return false;
}

bool playerControllerScript::checkThrow()
{
	auto ic = getGameObject().lock()->getComponent<inputComponent>();
	if (ic->getButtonState("throw") && m_grabbing == true)
	{
		return true;
	}
	return false;
}
