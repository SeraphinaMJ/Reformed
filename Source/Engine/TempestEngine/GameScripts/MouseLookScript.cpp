/*!***************************************************************************************
\file       MouseLookScript.cpp
\author     Aaron Damyen
\date       11/21/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the Mouse Look script
*****************************************************************************************/

//======== Self Include ================================================================//
#include "MouseLookScript.hpp"
#include "InputComponent.hpp"
#include "Logger.hpp"
#include "Quaternion.hpp"
#include "Transform.hpp"
//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <MathFunctions.hpp>
#include "Debug.hpp"
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//

mouseLookScript::mouseLookScript(typeRT & p_data)
{
	updateFromTypeRT(p_data);
}

typeRT mouseLookScript::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	std::vector<typeRT> members;

	typeRT targetRT("target object", m_target_object_name);
	result.insertMember(targetRT);

	return result;
}

void mouseLookScript::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if(l_members.find("target object") != l_members.end())
	{
		m_target_object_name = p_type.member("target object").getString();
	}
}

void mouseLookScript::setActive(bool p_active)
{
	// if we are switching to active, then clear out the old position
	if(!m_active && p_active)
	{
		m_old_pos = m_cur_pos;
	}
	m_active = p_active;


	auto l_input_comp = getComponent<inputComponent>();
	if(!l_input_comp)
	{
		logger("MouseLookScript").error() << "inputComponent required on same object as MouseLookScript.";
		return;
	}

	logger("MouseLookScript").debug() << "setMouseCapture(" << m_active << ")";
	l_input_comp->setMouseCapture(m_active);
}

bool mouseLookScript::getActive()
{
	return m_active;
}

void mouseLookScript::onStart()
{
	auto l_input_comp = getComponent<inputComponent>();

	if(!l_input_comp)
	{
		logger("MouseLookScript").error() << "inputComponent required on same object as MouseLookScript.";
		return;
	}

	l_input_comp->addCursorMapping("Mouse", Cursor::Default_Mouse);
}

void mouseLookScript::onUpdate()
{
	auto l_input_comp = getComponent<inputComponent>();
	m_cur_pos = l_input_comp->getCursorState("Mouse");
	if(!l_input_comp)
	{
		logger("MouseLookScript").error() << "inputComponent required on same object as MouseLookScript.";
		return;
	}

	if(m_active)
	{
		const auto&[oldX, oldY] = m_old_pos;
		const auto&[newX, newY] = m_cur_pos;

		float diff_x = newX - oldX;
		float diff_y = newY - oldY;

		float rotationX = diff_x * m_sensitivity_x;
		float rotationY = diff_y * m_sensitivity_y;

		auto l_transform = getComponent<transform>();

		vector3 l_euler = toEulerAngle(l_transform->getRotation());

		l_euler.x -= rotationY;
		l_euler.y -= rotationX;

		l_transform->setRotation(toQuaternion(l_euler));
	}
	else
	{
		auto target = findObject(m_target_object_name);
		if (target != nullptr)
		{

			// look at the target
			vector3 targetPosition = target->getComponent<transform>()->getGlobalPosition();
			vector3 up_offset = target->getComponent<transform>()->getUp().normalize() * 15.0f;

			targetPosition += up_offset;
			getComponent<transform>()->lookAt(targetPosition);

			logger("MLS").debug() << "Target: " << targetPosition.getX() << ", " << targetPosition.getY() << ", " << targetPosition.getZ();
		}
	}

	vector3 position = getComponent<transform>()->getGlobalPosition();
	logger("MLS").debug() << "pos: " << position.getX() << ", " << position.getY() << ", " << position.getZ();

	m_old_pos = m_cur_pos;

}

void mouseLookScript::onShutdown()
{
	if(m_active)
	{
		setActive(false);
	}
}
