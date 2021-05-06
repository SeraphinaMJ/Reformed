/*!***************************************************************************************
\file       SceneWindow.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the sceneWindow class
*****************************************************************************************/

#include "SceneWindow.hpp"

#include <imgui.h>
#include "EngineController.hpp"
#include <ImGuizmo.h>
#include "EditorWindow.hpp"
#include "Renderers/ToggleButton.hpp"
#include <GLFW/glfw3.h>
#include <Logger.hpp>
#include <EngineRunner.hpp>
#include <Engine.hpp>

#include <Aspect.hpp>


Editor::sceneWindow::sceneWindow(editorWindow * p_parent_editor, glfwEditorAdapter * p_editor_adapter)
	: windowBase(p_parent_editor),
	m_editor_adapter{p_editor_adapter},
	m_scene_image_size{1024, 768}, m_scene_texture_id{nullptr},
  m_play_button(p_parent_editor, "PlayToggle",
		[this](const toggleButton &) {getEngineController().start(); },
		[this](const toggleButton &) {getEngineController().stop(); }),
    m_editor_object_manager(getEngineController(), getLogger()), m_allow_selection(true)
{
	// set size and color for the play toggle button
	m_play_button.setWidthRatio(5.55f);
	m_play_button.setOnColorHighlight(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_play_button.setOnColor(ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
}
void Editor::sceneWindow::renderGame()
{
	const auto& viewport = getEngineController().getEngineRunner()->getEngine()->getGraphicsManipulator().lock()->getViewport();

	ImVec2 l_guizmo_size( viewport.width,viewport.height );
	ImVec2 l_texture_size{ 1024, 768 }; //TODO: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	const ImGuiWindowFlags l_flags = ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoSavedSettings;

	ImGui::BeginChild("gamePlay", l_texture_size, false, l_flags);
	
	ImVec2 winpos = ImGui::ImageAndGetWindowPos(m_scene_texture_id,
		l_texture_size, ImVec2(0, 1), ImVec2(1, 0));

	auto l_guizmo_position = [viewport, &winpos]() -> ImVec2
	{
		return { winpos.x + viewport.x, winpos.y + viewport.y };

	}();
	//ImVec2(ImGui::GetWindowPos().x - ImGui::GetCursorPos().x,
		//ImGui::GetWindowPos().y - ImGui::GetCursorPos().y);

	// update adapter with position of window
	m_editor_adapter->setOffset(winpos.x, winpos.y);

	if (!m_osm.isCameraAttached())
	{
		m_osm.setRayCastCamera(getEngineController().getEngineRunner()->getEngine()->getCameraManipulator(), getLogger());
	}
	//m_osm.isSelected(winpos);

	if (getSelectionKeeper().isGameObjectSelected())
	{
		gizmoRenderer & l_ptr = getTopWindow()->getGizmoRenderer();
		l_ptr.setDimensions(l_guizmo_position.x, l_guizmo_position.y, l_guizmo_size.x, l_guizmo_size.y);
		l_ptr.setOperation(getEditorState().m_guizmo_operation);
		l_ptr.setMode(getEditorState().m_guizmo_mode);
		l_ptr.renderInWindow();
	}

	// only allow camera manipulation when the mouse is over the window
	if (ImGui::IsMouseHoveringWindow())
	{
		// pan editor camera
		if (ImGui::IsMouseClicked(2)) // middle mouse button pressed
		{
			m_last_drag_delta_2 = ImGui::GetMouseDragDelta(2);
		}
		if (ImGui::IsMouseDragging(2)) // middle mouse button down and dragging
		{
			ImVec2 l_cur_delta = ImGui::GetMouseDragDelta(2);
			float l_xDiff = l_cur_delta.x - m_last_drag_delta_2.x;
			float l_yDiff = l_cur_delta.y - m_last_drag_delta_2.y;
			logger("panning").info() << l_xDiff << ", " << l_yDiff;
			const float l_xScale = -0.01f; // flip direction
			const float l_yScale = 0.01f;

			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->panCameraPosition(l_xDiff * l_xScale, l_yDiff * l_yScale);
			m_last_drag_delta_2 = l_cur_delta;
		}

		// rotate editor camera
		if (ImGui::IsMouseClicked(1)) // right mouse button
		{
			//ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			m_last_drag_delta_1 = ImGui::GetMouseDragDelta(1);
			//m_camera_rotation = getEngineController()->getEngineRunner()->getCameraManipulator().lock()->getCameraDirection();
		}
		if (ImGui::IsMouseDragging(1)) // right mouse button
		{
			ImVec2 l_cur_pos = ImGui::GetMouseDragDelta(1);
			float l_xDiff = l_cur_pos.x - m_last_drag_delta_1.x;
			float l_yDiff = l_cur_pos.y - m_last_drag_delta_1.y;
			logger("panning").info() << l_xDiff << ", " << l_yDiff;
			const float l_xScale = 0.1f;
			const float l_yScale = 0.1f;

			//vector3 l_new_camera_rotation = m_camera_rotation + l_cur_pos * m_rotate_augment_amount;
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->rotateCamera2DCoords(-l_xDiff * l_xScale, -l_yDiff * l_yScale);
			//logger("SceneWindow").debug() << "rotate offset (" << l_offset.x << ", " << l_offset.y << ")";
			m_last_drag_delta_1 = l_cur_pos;
		}

	}

	//move camera position with wasd
	const float l_keyboardCamera_amount = (0.25f);
	const float l_keyboardCamera_shift_amount = 0.75f;
	//if (ImGui::Button("Right"))
	if (ImGui::IsMouseHoveringWindow())
	{
		if (ImGui::IsKeyDown(GLFW_KEY_D) && (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(l_keyboardCamera_shift_amount, 0);
		}
		else if (ImGui::IsKeyDown(GLFW_KEY_D))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(l_keyboardCamera_amount, 0);
		}

		if (ImGui::IsKeyDown(GLFW_KEY_S) && (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(0, l_keyboardCamera_shift_amount);
		}
		else if (ImGui::IsKeyDown(GLFW_KEY_S))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(0, l_keyboardCamera_amount);
		}

		if (ImGui::IsKeyDown(GLFW_KEY_W) && (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(0, -l_keyboardCamera_shift_amount);
		}
		else if (ImGui::IsKeyDown(GLFW_KEY_W))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(0, -l_keyboardCamera_amount);
		}

		if (ImGui::IsKeyDown(GLFW_KEY_A) && (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(-l_keyboardCamera_shift_amount, 0);
		}
		else if (ImGui::IsKeyDown(GLFW_KEY_A))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(-l_keyboardCamera_amount, 0);
		}

		if (ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT) && (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(-l_keyboardCamera_shift_amount, 0);
		}
		else if (ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->panCameraPosition(0, -l_keyboardCamera_amount);
		}

		if (ImGui::IsKeyDown(GLFW_KEY_SPACE) && (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->transCameraXZcoords(-l_keyboardCamera_shift_amount, 0);
		}
		else if (ImGui::IsKeyDown(GLFW_KEY_SPACE))
		{
			getEngineController().getEngineRunner()->getEngine()->getCameraManipulator().lock()->panCameraPosition(0, l_keyboardCamera_amount);
		}

    if (m_allow_selection)
    {
      if (ImGui::IsMouseClicked(0))
      {
        auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();

        float l_mouse_position_x = ImGui::GetMousePos().x - ImGui::GetCursorScreenPos().x;
        float l_mouse_position_y = 1.0f * (m_scene_image_size.y + ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y);
        objID l_object_id = 0;

        l_object_id = l_scene_manipulator->getObjectFromClick(l_mouse_position_x, l_mouse_position_y);

        if (l_object_id != 0)
        {
          getSelectionKeeper().setSelected(l_object_id, getEngineController());
        }
      }
    }
	}
	ImGui::EndChild();
	

	//ImVec2 l_guizmo_size = { 1024,768 };
	//ImVec2(ImGui::GetWindowSize().x - l_guizmo_position.x,
	//	ImGui::GetWindowSize().y - l_guizmo_position.y);

  
  //if (ImGui::IsMouseHoveringWindow())
  //{
  //  
  //  
  //}
}

void Editor::sceneWindow::onRender()
{
	// render the play button and state text
	m_play_button.render();
	ImGui::SameLine();
	if(m_play_button.isEnabled())
	{
		ImGui::Text("Playing");
	}
	else
	{
		ImGui::Text("Stopped");
	}

	toolbarRenderer l_toolbar_renderer;
	l_toolbar_renderer.render(&getEditorState());

	renderGame();
	
}

void Editor::sceneWindow::setImageSize(float p_width, float p_height)
{
	m_scene_image_size.x = p_width;
	m_scene_image_size.y = p_height;
}

Editor::EditorObjectManager& Editor::sceneWindow::getEditorObjectManager()
{
  return m_editor_object_manager;
}

void Editor::sceneWindow::setTextureID(void* p_texture_id)
{
	m_scene_texture_id = p_texture_id;
}







