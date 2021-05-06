/*!***************************************************************************************
\file       ThirdPersonCameraController.cpp
\author     Aaron Damyen
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the third person camera controller script.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "ThirdPersonCameraController.hpp"
//======== 1st Party Includes ==========================================================//
#include "Transform.hpp"
#include "DebugLines.hpp"
#include "Logger.hpp"
#include "CameraBase.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Vector3Serialization.hpp>
#include <Matrix4x4.hpp>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
thirdPersonCameraController::thirdPersonCameraController(typeRT & p_data)
	: scriptCPP(p_data)
{
	updateFromTypeRT(p_data);
}

typeRT thirdPersonCameraController::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	std::vector<typeRT> members;

	typeRT maxDistanceRT("max distance", m_max_distance);
	result.insertMember(maxDistanceRT);
	typeRT targetRT("target object", m_target_object_name);
	result.insertMember(targetRT);
	typeRT offsetRT = ::toTypeRT(m_offset);
	offsetRT.setVariableName("offset");
	result.insertMember(offsetRT);
	typeRT sensitivityRT("sensitivity", m_sensitivity);
	result.insertMember(sensitivityRT);

	return result;
}

void thirdPersonCameraController::updateFromTypeRT(typeRT & p_type)
{
	auto l_members = p_type.members();

	if(l_members.find("max distance") != l_members.end())
	{
		m_max_distance = p_type.member("max distance").getFloat();
	}
	if(l_members.find("target object") != l_members.end())
	{
		m_target_object_name = p_type.member("target object").getString();
	}
	if(l_members.find("offset") != l_members.end())
	{
		m_offset = ::toVector3(p_type.member("offset"));
	}
	if(l_members.find("sensitivity") != l_members.end())
	{
		m_sensitivity = p_type.member("sensitivity").getFloat();
	}
}

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//
void thirdPersonCameraController::onUpdate()
{
	auto target = findObject(m_target_object_name);
	if (target == nullptr)
		return;

	// rotate for the view
	matrix4x4 targetTransform = matrix4x4::rotationAxis(vector3::UnitY, m_y_rotation);

	// offset position by some set amount
	vector3 back_offset = targetTransform.forward().normalize() * m_offset.getZ();
	vector3 up_offset = targetTransform.up().normalize() * m_offset.getY();
	vector3 right_offset = targetTransform.right().normalize() * m_offset.getX();

	vector3 targetPosition = target->getComponent<transform>()->getGlobalPosition() + back_offset + up_offset + right_offset;

	//logger("tPCC").info() << targetPosition.getX() << ", " << targetPosition.getY() << ", " << targetPosition.getZ();


	matrix4x4 kdf;
	kdf.computeModelMatrixFromLookAtVector2(targetPosition, target->getComponent<transform>()->getPosition());
	getComponent<transform>()->fromGlobalMatrix(kdf);

	getComponent<transform>()->setPosition(targetPosition);

	//getComponent<transform>()->lookAt(target->getComponent<transform>()->getGlobalPosition());
}

//======== Functionality ===============================================================//
void thirdPersonCameraController::adjustCamera(float p_diff_x, float p_diff_y)
{
	float l_arc_measure = (m_sensitivity * p_diff_x) / (m_offset.getZ() * 2 * 3.1415927f);

	m_y_rotation -= l_arc_measure;
}


//======== Helper Functions ============================================================//

