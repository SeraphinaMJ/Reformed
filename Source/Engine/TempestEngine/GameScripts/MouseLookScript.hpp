/*!***************************************************************************************
\file       MouseLookScript.hpp
\author     Aaron Damyen
\date       11/21/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the Mouse Look script
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "ScriptCPP.hpp"
#include "InputConstants.hpp"

//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
class mouseLookScript final : public scriptCPP
{
public:
	mouseLookScript() = default;
	mouseLookScript(typeRT & p_data);

	typeRT toTypeRT() const override;
	void updateFromTypeRT(typeRT & p_type) override;

	componentType const type() const override { return mouseLookScript::getType(); }
	static componentType const getType() { static componentType s_type = componentType::forInherited<mouseLookScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Mouse Look Script", true, true, true); }

	void setActive(bool p_active);
	bool getActive();

	void onStart() override;
	void onUpdate() override;
	void onShutdown() override;
	
private:
	bool m_active = false;
	float m_sensitivity_x = 0.25f;
	float m_sensitivity_y = 0.25f;

	std::string m_target_object_name;
	
	cursorPosition m_old_pos;
	cursorPosition m_cur_pos;
};

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

// namespace <namespace name>

