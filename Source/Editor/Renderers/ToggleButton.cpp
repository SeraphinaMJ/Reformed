/*!***************************************************************************************
\file       ToggleButton.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for a toggle button ImGui component.
		Following function from https://github.com/ocornut/imgui/issues/1537
*****************************************************************************************/
#include "ToggleButton.hpp"
#include "../EditorObjectManager.hpp"
#include "../EditorWindow.hpp"

toggleButton::toggleButton(Editor::editorWindow * p_top_window, const char* p_id, triggerHandler p_onEnabledTriggerHandler, triggerHandler p_onDisabledTriggerHandler)
	: renderableBase(p_top_window), m_id{ p_id }, m_is_enabled{ false },
m_onEnabledTriggerHandler{p_onEnabledTriggerHandler}, m_onDisabledTriggerHandler{p_onDisabledTriggerHandler},
m_width_ratio{1.55f},
	m_on_color{0.56f, 0.83f, 0.26f, 1.0f}, m_off_color{ 0.85f, 0.85f, 0.85f, 1.0f },
  m_on_color_highlight{ 0.64f, 0.83f, 0.34f, 1.0f }, m_off_color_highlight{ 0.78f, 0.78f, 0.78f, 1.0f }
{
}

bool toggleButton::isEnabled() const
{
	return m_is_enabled;
}

void toggleButton::reset()
{
	m_is_enabled = false;
}

float toggleButton::getWidthRatio() const
{
	return m_width_ratio;
}

void toggleButton::setWidthRatio(float p_width_ratio)
{
	m_width_ratio = p_width_ratio;
}

ImVec4 toggleButton::getOnColor() const
{
	return m_on_color;
}

void toggleButton::setOnColor(ImVec4 p_color)
{
	m_on_color = p_color;
}

ImVec4 toggleButton::getOffColor() const
{
	return m_off_color;
}

void toggleButton::setOffColor(ImVec4 p_color)
{
	m_off_color = p_color;
}

ImVec4 toggleButton::getOnColorHighlight() const
{
	return m_on_color_highlight;
}

void toggleButton::setOnColorHighlight(ImVec4 p_color)
{
	m_on_color_highlight = p_color;
}

ImVec4 toggleButton::getOffColorHighlight() const
{
	return m_off_color_highlight;
}

void toggleButton::setOffColorHighlight(ImVec4 p_color)
{
	m_off_color_highlight = p_color;
}

void toggleButton::onRender()
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = height * m_width_ratio;
	float radius = height * 0.50f;

	ImGui::InvisibleButton(m_id, ImVec2(width, height));
	if (ImGui::IsItemClicked())
	{
		m_is_enabled = !m_is_enabled;
		if(m_is_enabled)
		{
			m_onEnabledTriggerHandler(*this);
		}
		else
		{
			m_onDisabledTriggerHandler(*this);

      //auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();
      Editor::EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
      //Need to reset all? EditorObject data
      l_editor_object_manager.updateAllEditorObjectsFromEngine();
		}
	}

	float t = m_is_enabled ? 1.0f : 0.0f;

	ImGuiContext& g = *GImGui;
	float ANIM_SPEED = 0.08f;
	if (g.LastActiveId == g.CurrentWindow->GetID(m_id))// && g.LastActiveIdTimer < ANIM_SPEED)
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = m_is_enabled ? (t_anim) : (1.0f - t_anim);
	}

	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = ImGui::GetColorU32(ImLerp(m_off_color_highlight, m_on_color_highlight, t));
	else
		col_bg = ImGui::GetColorU32(ImLerp(m_off_color, m_on_color, t));

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}


