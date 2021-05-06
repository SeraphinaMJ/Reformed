/*!***************************************************************************************
\file       EditorWindow.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the main editor window class
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "HierarchyWindow.hpp"
#include "SceneWindow.hpp"
#include "DataSelect.hpp"
#include "InspectorWindow.hpp"
#include "NavMeshWindow.hpp"
#include "EditorState.hpp"
#include "ToolbarRenderer.hpp"
#include "EditorLogger.hpp"
#include "NodeGraph/NodeGraphWindow.hpp"
#include "FileOpener.hpp"
//======== 3rd Party Includes ==========================================================//
#include <GLFWEditorAdapter.hpp>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class engineRunner;
class icomponentRepository;
class externalWindowInterface;
class renderContextInterface;
class inputSourceInterface;

namespace Editor
{
	//======== Forward Declarations=========================================================//
	class EditorInput;
	class styleKeeper;
	class graphicsWindow;

	/*!***************************************************************************************
	\par class: editorWindow
	\brief   This is the interface for the editor's main window
	*****************************************************************************************/
	class editorWindow
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs the editor window around a particular instance of an engine
		\param p_engine - The engine this editor should use
		*****************************************************************************************/
		editorWindow(glfwEditorAdapter * p_editor_adapter, const std::string & p_base_path, const std::string & p_project_name, bool p_dark_mode);
		/*!***************************************************************************************
		\brief  Destroys this editorWindow, freeing resources as necessary
		*****************************************************************************************/
		~editorWindow(); 

		/*!***************************************************************************************
		\brief  Retrieves a reference to the engine controller object
		\return engineController & - The reference to the engine controller
		*****************************************************************************************/
		engineController & getEngineController() const { return *m_engine_controller; }
		/*!***************************************************************************************
		\brief  Retrieves a reference to the scene window within this editor
		\return sceneWindow & - Reference to the scene window
		*****************************************************************************************/
		sceneWindow & getSceneWindow() const { return *m_scene_window; }

		/*!***************************************************************************************
		\brief  Retrieves a reference to the inspector window within this editor
		\return inspectorWindow & - Reference to the inspector window
		*****************************************************************************************/
		inspectorWindow & getInspectorWindow() const { return *m_inspector_window; }

		/*!***************************************************************************************
		\brief  Renders the editor window and subwindows
		*****************************************************************************************/
		void render();

		/*!***************************************************************************************
		\brief  Retrieves a reference to the hierarchy window within this editor
		\return hierarchyWindow * - Reference to the hierarchy window.
		*****************************************************************************************/
		hierarchyWindow * getHierarchyWindow() { return m_hierarchy_window; }
		/*!***************************************************************************************
		\brief  Retrieves a reference to the gizmo renderer object within this editor
		\return gizmoRenderer & - Reference to the gizmo renderer.
		*****************************************************************************************/
		gizmoRenderer & getGizmoRenderer() { return m_gizmo_renderer;  }

		/*!***************************************************************************************
		\brief  Retrieves a reference to the selection keeper object within this editor
		\return hierarchySelectionKeeper & - Reference to the selection keeper
		*****************************************************************************************/
		hierarchySelectionKeeper & getSelectionKeeper() { return m_hierarchy_selection; }

		/*!***************************************************************************************
		\brief  Retrieves a reference to the style keeper object within this editor
		\return styleKeeper & - Reference to the style keeper
		*****************************************************************************************/
		styleKeeper & getStyleKeeper() { return m_styles; }

		/*!***************************************************************************************
		\brief  Retrieves if the editor is currently running
		\return bool - true if the editor is/should be running, false otherwise
		*****************************************************************************************/
		bool isRunning() { return m_running; }

		/*!***************************************************************************************
		\brief  Retrieves the editor state object for this editor
		\return editorState & - Reference to the editor state
		*****************************************************************************************/
		editorState & getEditorState() { return m_current_state; }

		void requestShutdown();

		bool getIsProjectFolderSet() const { return m_project_folder_set; }

		void setIsProjectFolderSet(bool p_set) { m_project_folder_set = p_set; }

		Editor::EditorLogger & getLogger() { return m_logger; }

		bool getDarkMode() const { return m_dark_mode; }

		void setDarkMode(bool p_bool) { m_dark_mode = p_bool; }

		void shutdown();

		const std::string & getProjectFilePath() const { return m_project_file_path; }

		const std::string & getProjectFileName() const { return m_project_file_name; }

	private:
		/*!***************************************************************************************
		\brief  Configures the color scheme for the editor.
		*****************************************************************************************/
		void setupColors();
		/*!***************************************************************************************
		\brief  Setups an initial scene in the engine (as viewed by the editor)
		*****************************************************************************************/
		void newInitialScene();

		bool createProjectFile(const std::string & p_project_path);

		bool saveProjectFile();

		bool openProjectFile(const std::string & p_project_path);

    void renderImGuiFPS() const;

		glfwEditorAdapter * m_editor_adapter;
		engineController * m_engine_controller;
		editorState m_current_state;
		hierarchySelectionKeeper m_hierarchy_selection;
		gizmoRenderer m_gizmo_renderer;
		styleKeeper m_styles;


		toolbarRenderer m_toolbar_renderer;
		hierarchyWindow * m_hierarchy_window;
		sceneWindow * m_scene_window;
		dataSelect * m_dataSelect_window;
		inspectorWindow * m_inspector_window;
		navMeshWindow * m_navmesh_window;
		nodeGraphWindow * m_nodegraph_window;
		graphicsWindow * m_graphics_window;


		EditorInput * m_editor_input;

		static Editor::EditorLogger m_logger;

    FileOpener m_file_selector;
    FileOpener m_folder_selector;

    std::string m_project_file_path;
    std::string m_project_file_name;

		bool m_dark_mode;

		bool m_show_imgui_debug_window;
		bool m_show_imgui_demo;
		bool m_show_imgui_user_guide;

		bool m_show_quit_window;
		bool m_openPupUpOnce;
		bool m_running;

		bool m_show_welcome_window;
		bool m_showWelcomeOnce;

    //bool m_show_file_system;
    //bool m_show_folder_system;
    bool m_project_folder_set;
    bool m_coming_from_file_save_scene;
    bool m_coming_from_file_open_project;
	};
			 	 	
} // namespace Editor

