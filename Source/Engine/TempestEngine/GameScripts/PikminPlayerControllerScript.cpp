/*!***************************************************************************************
\file       PikminPlayerControllerScript.cpp
\author		Aaron Damyen
\date       11/20/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the pikmin player controller script.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "PikminPlayerControllerScript.hpp"
//======== 1st Party Includes ==========================================================//
#include "InputComponent.hpp"
#include "Transform.hpp"
#include "InputFramework.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Vector3.hpp>
#include "RigidBody.hpp"
#include "Logger.hpp"
#include "TargetingCircleScript.hpp"
#include "ThirdPersonCameraController.hpp"
#include "Health.hpp"
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
pikminPlayerControllerScript::pikminPlayerControllerScript(typeRT & p_data)
	: scriptCPP(p_data)
{
	updateFromTypeRT(p_data);
}

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//
typeRT pikminPlayerControllerScript::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	std::vector<typeRT> members;

	typeRT cameraNameRT("camera name", m_camera_name);
	result.insertMember(cameraNameRT);
	
	typeRT movementForceRT("movement force", m_movement_force);
	result.insertMember(movementForceRT);

	return result;
}

void pikminPlayerControllerScript::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if(l_members.find("camera name") != l_members.end())
	{
		m_camera_name = p_type.member("camera name").getString();
	}
	if(l_members.find("movement force") != l_members.end())
	{
		m_movement_force = p_type.member("movement force").getFloat();
	}
}

void pikminPlayerControllerScript::onStart()
{
	auto l_input_comp = getComponent<inputComponent>();

	l_input_comp->addButtonMapping("forward", 'w');
	l_input_comp->addButtonMapping("backward", 's');
	l_input_comp->addButtonMapping("left", 'a');
	l_input_comp->addButtonMapping("right", 'd');
	l_input_comp->addButtonMapping("Mode Switch", ' ');
	//l_input_comp->addButtonMapping("Mouse Look", MouseButton::Right);
	l_input_comp->addCursorMapping("Mouse Pos", Cursor::Default_Mouse);
}

void pikminPlayerControllerScript::enableController()
{
	m_enabled = true;
}

void pikminPlayerControllerScript::disableController()
{
	m_enabled = false;
}

//======== Functionality ===============================================================//
void pikminPlayerControllerScript::onUpdate()
{
	if (m_enabled == false)
		return;

	componentHandle<Health> myHealth = getComponent<Health>();
	bool myHealthFound = (myHealth);
	if (!myHealthFound)
		return;

	if (myHealth->IsDead())
		return;

	auto l_input_comp = getComponent<inputComponent>();
	if(!l_input_comp)
	{
		logger("PikminPlayerControllerScript").error() << "inputComponent required on the same object as PikminPlayerControllerScript";
		return;
	}

	// check for mode switch
	if(l_input_comp->getButtonState("Mode Switch") && !m_reticle_mode)
	{
		// cooldown check
		m_reticle_mode = true;
		// notify targeting circle script
		auto l_objs = findObjectsByComponent<targetingCircleScript>();
		for(auto l_obj : l_objs)
		{
			l_obj->getComponent<targetingCircleScript>()->setEnable(m_reticle_mode);
		}
	}
	else if (l_input_comp->getButtonState("Mode Switch") == false && m_reticle_mode)
	{
		m_reticle_mode = false;
		// notify targeting circle script
		auto l_objs = findObjectsByComponent<targetingCircleScript>();
		for (auto l_obj : l_objs)
		{
			l_obj->getComponent<targetingCircleScript>()->setEnable(m_reticle_mode);
		}
	}

	auto l_camera = findObject(m_camera_name);
	
	// update mouse look mode flag, capturing or release mouse appropriately
	bool l_mouse_look = !l_input_comp->getButtonState("Mode Switch");
	if (l_mouse_look != m_mouse_look)
	{
		m_mouse_look = l_mouse_look;
		l_input_comp->setMouseCapture(m_mouse_look);

		// reset old position
		m_old_pos = l_input_comp->getCursorState("Mouse Pos");
	}
	
	// if in mouse look mode, adjust camera
	if(m_mouse_look)
	{
		m_cur_pos = l_input_comp->getCursorState("Mouse Pos");
		// tell the third person camera controller script which state to be in
		if (l_camera != nullptr)
		{
			auto l_camera_controller = l_camera->getComponent<thirdPersonCameraController>();
			if (l_camera_controller)
			{
				float diff_x = m_cur_pos.first - m_old_pos.first;
				float diff_y = m_cur_pos.second - m_old_pos.second;
				l_camera_controller->adjustCamera(diff_x, diff_y);
			}
		}
		m_old_pos = m_cur_pos;
	}
	
	// check for movement
	if (!m_reticle_mode)
	{
		vector3 l_direction = vector3::Zero;

		auto l_transform = getComponent<transform>();

		// choose which forward and right vectors to use
		vector3 l_forward = vector3::Zero;
		vector3 l_right = vector3::Zero;


		// always move directionally based on camera view
		if (l_camera != nullptr)
		{
			l_forward = l_camera->getComponent<transform>()->getForward();
			l_forward.setY(0);
			l_forward.normalized();
			l_right = l_camera->getComponent<transform>()->getRight();
			l_right.setY(0);
			l_right.normalized();
		}
		/*if (!m_mouse_look)
		{
			// when not mouse looking, use vectors of the camera object to move player
			l_forward = l_transform->getForward();
			l_right = l_transform->getRight();
		}
		else
		{
			if (l_camera != nullptr)
			{
				// when mouse looking, use vectors of the player object to keep it moving
				l_forward = l_camera->getComponent<transform>()->getForward();
				l_right = l_camera->getComponent<transform>()->getRight();
			}
		}*/

		if (l_input_comp->getButtonState("forward"))
		{
			l_direction -= l_forward;
		}
		if (l_input_comp->getButtonState("backward"))
		{
			l_direction += l_forward;
		}
		if (l_input_comp->getButtonState("left"))
		{
			l_direction -= l_right;
		}
		if (l_input_comp->getButtonState("right"))
		{
			l_direction += l_right;
		}

		// apply a force to this object to move in the appropriate direction
		if (l_direction != vector3::Zero)
		{
			l_direction = l_direction.normalize() * m_movement_force;
			getComponent<rigidBody>()->applyForce(l_direction);
		}
	}
}

void pikminPlayerControllerScript::onShutdown()
{
	inputFramework::disableMouseCapture();
}


//======== Helper Functions ============================================================//

