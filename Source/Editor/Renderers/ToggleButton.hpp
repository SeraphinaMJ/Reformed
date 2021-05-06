/*!***************************************************************************************
\file       ToggleButton.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for a toggle button ImGui component.
        Following function from https://github.com/ocornut/imgui/issues/1537
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "../Scaffolding/RenderableBase.hpp"
//======== 3rd Party Includes ==========================================================//
#include <imgui.h>
#include <imgui_internal.h>
#include <functional>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: toggleButton
\brief   This is the implementation of a simple toggle button in ImGui
*****************************************************************************************/
class toggleButton : public Editor::renderableBase
{
public:
	typedef std::function<void(const toggleButton &)> triggerHandler;
	/*!***************************************************************************************
	\brief  Simple no-operation function for defaults of handlers
	*****************************************************************************************/
	static void NOOP(const toggleButton &) {}

	/*!***************************************************************************************
	\brief  Creates the toggle button with an id and event handler callbacks.
	\param p_id - the id of the button
	\param p_onEnabledTriggerHandler - handler function for when button becomes enabled.
	\param p_onDisabledTriggerHandler - handler function for when button becomes disabled.
	*****************************************************************************************/
	toggleButton(Editor::editorWindow * p_top_window, const char * p_id, triggerHandler p_onEnabledTriggerHandler = NOOP, triggerHandler p_onDisabledTriggerHandler = NOOP);

	/*!***************************************************************************************
	\brief  Identifies if the button is currently enabled
	\return bool - true if button is enabled, false otherwise
	*****************************************************************************************/
	bool isEnabled() const;
	/*!***************************************************************************************
	\brief  Resets the state of the button (to disabled)
	*****************************************************************************************/
	void reset();

	/*!***************************************************************************************
	\brief  Retrieves the width ratio of the button
	\return float - width ratio of button
	*****************************************************************************************/
	float getWidthRatio() const;
	/*!***************************************************************************************
	\brief  Sets the width ratio of the button
	\param p_width_ratio - new width ratio of button
	*****************************************************************************************/
	void setWidthRatio(float p_width_ratio);

	/*!***************************************************************************************
	\brief  Retrieves the color of the button when enabled.
	\return ImVec4 - color of button when enabled
	*****************************************************************************************/
	ImVec4 getOnColor() const;
	/*!***************************************************************************************
	\brief  Sets the color of the button when enabled.
	\param ImVec4 - new color of button when enabled
	*****************************************************************************************/
	void setOnColor(ImVec4 p_color);
	/*!***************************************************************************************
	\brief  Retrieves the color of the button when disabled.
	\return ImVec4 - color of button when disabled
	*****************************************************************************************/
	ImVec4 getOffColor() const;
	/*!***************************************************************************************
	\brief  Sets the color of the button when disabled.
	\param ImVec4 - new color of button when disabled
	*****************************************************************************************/
	void setOffColor(ImVec4 p_color);
	/*!***************************************************************************************
	\brief  Retrieves the highlight color of the button  when enabled
	\return ImVec4 - highlight color of the button when enabled
	*****************************************************************************************/
	ImVec4 getOnColorHighlight() const;
	/*!***************************************************************************************
	\brief  Sets the highlight color of the button when enabled
	\param ImVec4 - new highlight color of button when enabled
	*****************************************************************************************/
	void setOnColorHighlight(ImVec4 p_color);
	/*!***************************************************************************************
	\brief  Retrieves the highlight color fo the button when disabled
	\return ImVec4 - highlight color of the button when disabled
	*****************************************************************************************/
	ImVec4 getOffColorHighlight() const;
	/*!***************************************************************************************
	\brief  Sets the highlight color of the button when disabled
	\param ImVec4 - new highlight color of button when disabled
	*****************************************************************************************/
	void setOffColorHighlight(ImVec4 p_color);

	/*!***************************************************************************************
	\brief  Renders the toggle button to the screen
	*****************************************************************************************/
	void onRender() override;

private:
	const char * m_id;
	bool m_is_enabled;

	triggerHandler m_onEnabledTriggerHandler;
	triggerHandler m_onDisabledTriggerHandler;

	float m_width_ratio;
	ImVec4 m_on_color;
	ImVec4 m_off_color;
	ImVec4 m_on_color_highlight;
	ImVec4 m_off_color_highlight;
};


