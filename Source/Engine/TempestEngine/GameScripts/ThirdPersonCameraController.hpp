/*!***************************************************************************************
\file       ThirdPersonCameraController.hpp
\author     Aaron Damyen
\date       11/19/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the third person camera controller script
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "ScriptCPP.hpp"

//======== 3rd Party Includes ==========================================================//
#include <Vector3.hpp>

//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
class thirdPersonCameraController final : public scriptCPP
{
public:
	thirdPersonCameraController() = default;
	thirdPersonCameraController(typeRT & p_data);

	typeRT toTypeRT() const override;
	void updateFromTypeRT(typeRT& p_type) override;
	
	componentType const type() const override { return thirdPersonCameraController::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<thirdPersonCameraController, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Third Person Camera Controller Script", true, true, true); }

	void adjustCamera(float p_diff_x, float p_diff_y);

	void onUpdate() override;
	
private:
	float m_y_rotation = 0.0f;
	float m_max_distance;
	std::string m_target_object_name;
	vector3 m_offset = vector3(0,15,50);
	float m_sensitivity = 0.60f;
};

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

// namespace <namespace name>

