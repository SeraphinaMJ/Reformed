/*!***************************************************************************************
\file       ToolbarRenderer.cpp
\author     Aaron Damyen
\date       1/26/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the toolbar renderer class members
*****************************************************************************************/

//========Self Include==================================================================//
#include "ToolbarRenderer.hpp"
//========1st Party Includes============================================================//
#include "EditorState.hpp"
//========3rd Party Includes============================================================//
#include <imgui.h>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

void Editor::toolbarRenderer::render(editorState* p_current_state)
{
	//if (ImGui::BeginChild("toolbar"))
	{
		// only allow key presses to effect when mouse is over the window
		if (ImGui::IsMouseHoveringWindow())
		{
			if (ImGui::IsKeyPressed('W')) p_current_state->m_guizmo_operation = guizmoOperation::Translate;
			if (ImGui::IsKeyPressed('E')) p_current_state->m_guizmo_operation = guizmoOperation::Rotate;
			if (ImGui::IsKeyPressed('R')) p_current_state->m_guizmo_operation = guizmoOperation::Scale;
			//if (ImGui::IsKeyPressed('T')) p_current_state->m_guizmo_mode = guizmoMode::Local;
			//if (ImGui::IsKeyPressed('Y')) p_current_state->m_guizmo_mode = guizmoMode::Global;
		}
		ImGui::SameLine();  ImGui::Text("      ");
		ImGui::SameLine();	ImGui::RadioButton("Translate",
			reinterpret_cast<int *>(&p_current_state->m_guizmo_operation), static_cast<int>(guizmoOperation::Translate));
		ImGui::SameLine();	ImGui::RadioButton("Rotation",
			reinterpret_cast<int *>(&p_current_state->m_guizmo_operation), static_cast<int>(guizmoOperation::Rotate));
		ImGui::SameLine();	ImGui::RadioButton("Scale",
			reinterpret_cast<int *>(&p_current_state->m_guizmo_operation), static_cast<int>(guizmoOperation::Scale));
		//ImGui::SameLine();  ImGui::Text("      ");
		//ImGui::SameLine();	ImGui::RadioButton("Local",
		//	reinterpret_cast<int *>(&p_current_state->m_guizmo_mode), static_cast<int>(guizmoMode::Local));
		//ImGui::SameLine();	ImGui::RadioButton("Global",
		//	reinterpret_cast<int *>(&p_current_state->m_guizmo_mode), static_cast<int>(guizmoMode::Global));
	}
	//ImGui::End();
}
