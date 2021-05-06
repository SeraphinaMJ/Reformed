/*!***************************************************************************************
\file       PikminPlayerControllerScript.hpp
\author     Aaron Damyen
\date       11/20/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the pikmin player controller script.
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
class pikminPlayerControllerScript final : public scriptCPP
{
public:
	pikminPlayerControllerScript() = default;
	pikminPlayerControllerScript(typeRT & p_data);

	typeRT toTypeRT() const override;
	void updateFromTypeRT(typeRT & p_type) override;
	
	componentType const type() const override { return pikminPlayerControllerScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<pikminPlayerControllerScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Pikmin Player Controller Script", true, true, true); }

	void onStart() override;
	void onUpdate() override;
	void onShutdown() override;

	void enableController();
	void disableController();
	
private:

	std::string m_camera_name;

	bool m_enabled = true;
	
	float m_mode_change_cooldown = 0.0f;
	bool m_reticle_mode = false;
	bool m_mouse_look = false;
	float m_movement_force = 100.0f;

	cursorPosition m_old_pos;
	cursorPosition m_cur_pos;
	
};

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

// namespace <namespace name>

