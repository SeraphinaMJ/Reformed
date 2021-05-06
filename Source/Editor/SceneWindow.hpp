/*!***************************************************************************************
\file       SceneWindow.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the sceneWindow class
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Scaffolding/WindowBase.hpp"
#include "Renderers/ToggleButton.hpp"
#include "Renderers/GizmoRenderer.hpp"
#include "ObjectSelector.hpp"
#include "EditorObjectManager.hpp"
//======== 3rd Party Includes ==========================================================//
#include <imgui.h>
#include <tuple>
#include <GLFWEditorAdapter.hpp>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

namespace Editor
{
	//======== Forward Declarations=======================================================//
	class hierarchySelectionKeeper;
	class engineController;
	class editorWindow;


	/*!***************************************************************************************
	\par class: sceneWindow
	\brief   This class is responsible for the content of the sceneWindow in the Editor
	*****************************************************************************************/
	class sceneWindow : public windowBase
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs the object, specifying base parameters
		\param p_parent_editor - reference to the containing editorWindow
		*****************************************************************************************/
		sceneWindow(editorWindow * p_parent_editor, glfwEditorAdapter * p_editor_adapter);

		/*!***************************************************************************************
		\brief  Specifies the size of the image rendered by the engine
		\param p_width - the width of the image
		\param p_height - the height of the image
		*****************************************************************************************/
		void setImageSize(float p_width, float p_height);

		EditorObjectManager & getEditorObjectManager();

		/*!***************************************************************************************
		\brief  Specifies the identification number of the texture of the image rendered by the engine
		\param p_texture_id - the id number of the texture of the rendered image
		\return What is this returning?
		*****************************************************************************************/
		void setTextureID(void * p_texture_id);

    void setAllowSelection(bool p_allow) { m_allow_selection = p_allow; }

	protected:
		/*!***************************************************************************************
		\brief  Handles the render event for drawing the window to the screen
		*****************************************************************************************/
		void onRender() override;
		void renderGame();

	private:
		glfwEditorAdapter * m_editor_adapter;
		
		ImVec2 m_scene_image_size;
		void* m_scene_texture_id;

		ImVec2 m_last_drag_delta;

		toggleButton m_play_button;

		ImVec2 m_last_drag_delta_1;
		ImVec2 m_last_drag_delta_2;

		objectSelector m_osm;

		EditorObjectManager m_editor_object_manager;

    bool m_allow_selection;
	};


} // namespace Editor

