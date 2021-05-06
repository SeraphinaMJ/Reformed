/*!***************************************************************************************
\file       ToggleDisplayOnButtonPress.hpp
\author     Alex Kudebeh
\date       11/26/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Toggles display of this UI object based on a button press
*****************************************************************************************/
#pragma once
//======== 1st Party Includes ==========================================================//
#include "../Components/ScriptCPP.hpp"
#include "InputComponent.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

class toggleDisplayOnButtonPress final : public scriptCPP
{
public:
	/*!***************************************************************************************
	\brief  Default constructor
	*****************************************************************************************/
	toggleDisplayOnButtonPress() = default;

	/*!***************************************************************************************
	\brief  Conversion constructor
	\param  p_data - The typeRT data to convert from
	*****************************************************************************************/
	toggleDisplayOnButtonPress(typeRT & p_data);

	/*!***************************************************************************************
	\brief  Type getter
	\return The type of component this is
	*****************************************************************************************/
	componentType const type() const override { return toggleDisplayOnButtonPress::getType(); }

	/*!***************************************************************************************
	\brief  Static type getter
	\return The type of component this is
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forInherited<toggleDisplayOnButtonPress, scriptCPP>(); return s_type; }

	/*!***************************************************************************************
	\brief  Override for type detail getter
	\retrun The component type details
	*****************************************************************************************/
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Toggle Display On Button Press Script", true, true, true); }

	/*!***************************************************************************************
	\brief  Converts the script to a typeRT.
	\return The script as a typeRT
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Updates the script from a typeRT
	\return The script as a typeRT
	*****************************************************************************************/
	void updateFromTypeRT(typeRT & p_type) override;

	/*!***************************************************************************************
	\brief  Overrides onStart. Does nothing
	*****************************************************************************************/
	void onStart() override;

	/*!***************************************************************************************
	\brief  Overrides onUpdate. Causes the agent to follow their path, if enabled and has a path
	*****************************************************************************************/
	void onUpdate() override;

	/*!***************************************************************************************
	\brief  Overrides onShutdown. Clears memory
	*****************************************************************************************/
	void onShutdown() override;

private:
	//!The input component attached to this object
	componentHandle<inputComponent> m_input;
	//!If the controls thing should be shown or not
	bool show = false;
	//!Toggle for detecting a new button press
	bool pressed = false;
};
