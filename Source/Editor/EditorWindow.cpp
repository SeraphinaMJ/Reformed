/*!***************************************************************************************
\file       EditorWindow.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the main editor window class
*****************************************************************************************/

//======== 1st Party Includes ==========================================================//
#include "EditorWindow.hpp"
#include "EngineController.hpp"
#include "UndoRedo.hpp"
#include "EditorInput.hpp"
#include "GraphicsWindow.hpp"

//======== 3rd Party Includes ==========================================================//
#include <imgui.h>
#include <imgui_internal.h>
#include "imguifilesystem.h"
#include <IComponentRepository.hpp>
#include <EngineRunner.hpp>
#include <Engine.hpp>
#include <windef.h> // UINT

#include <iostream>
#include <fstream>

//#define IMGUIFILESYSTEM_USE_ASCII_SHORT_PATHS_ON_WINDOWS
Editor::EditorLogger Editor::editorWindow::m_logger;

Editor::editorWindow::editorWindow(glfwEditorAdapter * p_editor_adapter, const std::string & p_base_path, const std::string & p_project_name, bool p_dark_mode)
	: m_editor_adapter{p_editor_adapter},
	m_engine_controller{new engineController(nullptr, m_logger)},
	m_hierarchy_selection(m_logger),
	m_gizmo_renderer(this),
	m_hierarchy_window{new hierarchyWindow(this)},
	m_scene_window{new sceneWindow(this, m_editor_adapter)},
	m_dataSelect_window{new dataSelect(this)},
    m_inspector_window{new inspectorWindow(this)},
	m_navmesh_window{new navMeshWindow(this)},
	m_nodegraph_window{new nodeGraphWindow(this, m_engine_controller)},
	m_graphics_window{new graphicsWindow{this}},
    m_editor_input(new EditorInput{m_engine_controller}),
	m_dark_mode(false),
	m_show_imgui_debug_window{ false }, 
    m_show_imgui_demo{ false }, 
    m_show_imgui_user_guide{ false }, 
    m_show_quit_window{ false }, 
    m_openPupUpOnce{ false }, 
    m_running{ true },
	m_show_welcome_window(false), 
    m_showWelcomeOnce(false),
	//m_show_file_system{false}, 
    //m_show_folder_system(false), 
    m_project_folder_set(false), 
    m_coming_from_file_save_scene(false),
    m_coming_from_file_open_project(false)
{
	configuration l_config;
	l_config.asset_path = p_base_path + "/" + "Assets/";
	l_config.data_path = p_base_path + "/" + "Data/";
	l_config.no_initial_scene = true;
	l_config.initial_window_show = true;

	engineRunner * l_engine = new engineRunner(m_editor_adapter, m_editor_adapter, m_editor_adapter);
	l_engine->initialize(l_config);

	m_engine_controller->setEngine(l_engine);

	m_project_folder_set = true;
	m_project_file_path = p_base_path;
	m_project_file_name = p_project_name;
	m_dark_mode = p_dark_mode;

  m_file_selector.setType(FileOpenerType::enm_file);
  m_folder_selector.setType(FileOpenerType::enm_folder);

  m_hierarchy_window->createEditorObjects();
}

Editor::editorWindow::~editorWindow()
{
	shutdown();

	delete m_navmesh_window;
	delete m_inspector_window;
	delete m_dataSelect_window;
	delete m_scene_window;
	delete m_hierarchy_window;
	delete m_nodegraph_window;
	delete m_engine_controller;
}

static ImVec4 colorFor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	return ImVec4(red / 256.0f, green / 256.0f, blue / 256.0f, alpha / 256.0f);
}

static ImVec4 colorFor(UINT32 rgba)
{
	return colorFor(
		(rgba >> 24) & 0xFF,
		(rgba >> 16) & 0xFF,
		(rgba >> 8) & 0xFF,
		rgba & 0xFF);
}

// macro to simplify the assignment of the numerous colors, now parameters are closer to what is seen in the Demo Window
#define SetColor(flag_suffix, colorHex) ImGui::GetStyle().Colors[ImGuiCol_##flag_suffix] = colorFor(colorHex)

void Editor::editorWindow::requestShutdown()
{
	if (!m_show_quit_window)
	{
		m_show_quit_window = true;
		m_openPupUpOnce = true;
	}
}

void Editor::editorWindow::shutdown()
{
	// release and delete the engine we created
	engineRunner * l_engine_runner_ptr = m_engine_controller->getEngineRunner();
	if (l_engine_runner_ptr != nullptr)
	{
		engine * l_engine_ptr = l_engine_runner_ptr->getEngine();
		if (l_engine_ptr != nullptr)
		{
			l_engine_ptr->shutdown();
			m_engine_controller->releaseEngine();
			delete l_engine_ptr;
		}
	}
}

void Editor::editorWindow::setupColors()
{
	SetColor(Text, 0x1E1E1EFF);
	SetColor(WindowBg, 0xC2C2C2FF);

	SetColor(MenuBarBg, 0xD2D2D2FF);
	SetColor(PopupBg, 0xFFFFFFFF);

	SetColor(TitleBg, 0xA2A2A2FF);
	SetColor(TitleBgActive, 0xA2A2A2FF);

	SetColor(TabUnfocused, 0xA2A2A2FF);
	SetColor(TabUnfocusedActive, 0xE4E4E4FF);
	SetColor(TabActive, 0xDEDEDEFF);
}

void Editor::editorWindow::newInitialScene()
{
	m_engine_controller->getEngineRunner()->getEngine()->getPersistenceManipulator().lock()->new_scene();
}

bool Editor::editorWindow::createProjectFile(const std::string & p_project_path)
{
	//Get the end folder
	char l_return[4096];
	ImGuiFs::PathGetFileName(p_project_path.c_str(), l_return);
	std::string l_fileName(l_return);
	//l_fileName.insert(0, 1, '/');
	l_fileName.append(".prj");

	std::string l_fullPath = p_project_path + '/' + l_fileName;

	std::ifstream l_exisitingFile(l_fullPath);
	if(l_exisitingFile.good())
	{
		m_logger.AddLog("[ERROR] Project File already exists here! Please choose an empty project folder.\n");
		return false;
	}

	std::ofstream l_file(l_fullPath);

	if(l_file.is_open())
	{
		l_file << "Project Path: " << l_fullPath << std::endl;
		//Save style
		if(m_dark_mode)
		{
			l_file << "Style: " << "dark" << std::endl;
		}
		else
		{
			l_file << "Style: " << "light" << std::endl;
		}

		l_file.close();

		m_project_file_path = p_project_path;
		m_project_file_name = l_fileName;

		m_logger.AddLog("[EDITOR] Creating project file:\n %s", l_fullPath.c_str());
	}
	else
	{
		m_logger.AddLog("[ERROR] Can not create project file in directory: %s.\n Please choose a writable directory.\n", p_project_path.c_str());
		return false;
	}

	//Project file creation success.
	return true;
}

bool Editor::editorWindow::saveProjectFile()
{
  std::string l_fullPath = m_project_file_path + '/' + m_project_file_name;
  std::ifstream l_file(l_fullPath);

  if(l_file.good())
  {
    l_file.close();
    std::ofstream l_outFile;

    //Rewrite data (easy)
    l_outFile.open(l_fullPath, std::ofstream::out | std::ofstream::trunc);
    l_outFile << "Project Path: " << l_fullPath << std::endl;
    if (m_dark_mode)
    {
      l_outFile << "Style: " << "dark" << std::endl;
    }
    else
    {
      l_outFile << "Style: " << "light" << std::endl;
    }

    l_outFile.close();

    m_logger.AddLog("[EDITOR] Saving project file:\n %s \n", l_fullPath.c_str());
    return true;
  }
  else
  {
    m_logger.AddLog("[ERROR] Could not open project file: %s.\n", l_fullPath.c_str());
    return false;
  }
}

bool Editor::editorWindow::openProjectFile(const std::string& p_project_path)
{
  std::ifstream l_project(p_project_path);

  if(l_project.good())
  {
    m_logger.AddLog("[EDITOR] Opening project file:\n %s\n", p_project_path.c_str());

    char l_result[4096];
    ImGuiFs::PathGetFileName(p_project_path.c_str(), l_result);
    char l_resultPath[4096];
    ImGuiFs::PathGetDirectoryName(p_project_path.c_str(), l_resultPath);

    m_project_file_name = l_result;
    m_project_file_path = l_resultPath;

    bool l_found = false;

    std::string l_line = "Style:";
    char search[5] = "dark";
    //auto l_buffer = l_project.rdbuf();
    for (unsigned int l_curLine = 0; std::getline(l_project, l_line); ++l_curLine)
    {
      if (l_line.find(search) != std::string::npos)
      {
        l_found = true;
        break;
      }
    }

    m_dark_mode = l_found;

    l_project.close();
  }
  else
  {
    m_logger.AddLog("[ERROR] Failed to open project file:\n %s\n", p_project_path.c_str());
    return false;
  }

  return true;
}

void Editor::editorWindow::renderImGuiFPS() const
{
  //FPS
  float l_imgui_fps = ImGui::GetIO().Framerate;
  ImGui::SameLine(0.0f, ImGui::GetWindowWidth() - 360.0f);
  ImGui::Text("ImGUI Reported FPS: ");
  ImGui::SameLine();
  ImGui::PushItemWidth(100.0f);
  ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.8f);
  ImGui::InputFloat("", &l_imgui_fps, ImGuiInputTextFlags_ReadOnly);
  ImGui::PopItemWidth();
  ImGui::PopItemFlag();
  ImGui::PopStyleVar();
  //
}

void Editor::editorWindow::render()
{
	// setup color scheme
  if(m_dark_mode)
  {
    ImGui::StyleColorsDark();
  }
  else
  {
    setupColors();
  }

	// make the docking window fullscreen and invisible
	ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	const ImGuiWindowFlags l_flags = ImGuiWindowFlags_NoMove
																| ImGuiWindowFlags_NoBringToFrontOnFocus
																| ImGuiWindowFlags_NoResize
																| ImGuiWindowFlags_NoScrollbar
																| ImGuiWindowFlags_NoSavedSettings
																| ImGuiWindowFlags_NoTitleBar
																| ImGuiWindowFlags_MenuBar;
	const float l_old_window_rounding = ImGui::GetStyle().WindowRounding;
	ImGui::GetStyle().WindowRounding = 0;
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	const bool l_visible = ImGui::Begin("ImGuiDock Window", NULL, ImVec2(0, 0), 1.0f, l_flags);
	ImGui::GetStyle().WindowRounding = l_old_window_rounding;
	ImGui::DockSpace(dockspace_id);

  if (m_show_welcome_window)
  {
    m_showWelcomeOnce = true;
  }

	if (l_visible)
	{
        // check for undo or redo
        m_editor_input->Update();

		// display main menu
		if (ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("New Scene"))
				{
          if (m_engine_controller->isPlaying())
          {
            m_engine_controller->stop();
          }

          m_logger.AddLog("[EDITOR] Creating a new scene.\n");
					newInitialScene();
				}
        if(ImGui::MenuItem("Save Project"))
        {
          //Write to the .prj file
          //Also call save scene here (for the current loaded scene)
          if(!m_project_folder_set)
          {
            m_folder_selector.setVisible(true);
          }
          else
          {
            //Write data to ProjectFile here
            saveProjectFile();
          }

        }
				if(ImGui::MenuItem("Save Scene", "CTRL+S"))
				{
          //If the save folder hasn't been set yet
          if(!m_project_folder_set)
          {
            //Open the folder picker and don't save yet.
            m_logger.AddLog("[ERROR] Project not yet set! Please choose a project folder.\n");

            m_folder_selector.setVisible(true);
            m_coming_from_file_save_scene = true;
          }
          else if(!m_engine_controller->isPlaying())
          {
            if(saveProjectFile())
            {
              m_logger.AddLog("[NOTICE] Stopping the engine.\n");
              m_logger.AddLog("[EDITOR] Saving scene.\n");
              //m_engine_controller->getEngineRunner()->getPersistenceManipulator().lock()->setPersistenceLocation("Data/");
              m_engine_controller->getEngineRunner()->getEngine()->getPersistenceManipulator().lock()->saveCurrentScene();
              //m_nodegraph_window->saveGraph();
            }
          }
				}
        if(ImGui::MenuItem("Open Project ..."))
        {
          m_file_selector.setVisible(true);
          m_file_selector.setPath(m_project_file_path);
          m_coming_from_file_open_project = true;
        }

        if(ImGui::MenuItem("Open Scene ..."))
		{
					m_hierarchy_selection.clearSelection();
					//m_engine_controller->getEngineRunner()->getPersistenceManipulator().lock()->setPersistenceLocation("Data/");
					//m_engine_controller->getEngineRunner()->getPersistenceManipulator().lock()->load("testname");

          if(m_project_folder_set)
          {
            m_file_selector.setVisible(true);
            m_file_selector.setPath(m_project_file_path + "/Data/scenes");
            m_coming_from_file_open_project = false;

            if (m_engine_controller->isPlaying())
            {
              m_logger.AddLog("[NOTICE] Stopping the engine.\n");
              m_engine_controller->stop();
            }
          }
          else
          {
            m_logger.AddLog("[WARNING] Please open or create a project first.\n");
          } 
				}
        if(ImGui::MenuItem("Quit", "ALT+F4"))
				{
          if(!m_show_quit_window)
          {
            m_show_quit_window = true;
            m_openPupUpOnce = true;
          }
				}
				ImGui::EndMenu();
			}
      if(ImGui::BeginMenu("Style"))
      {
        if(ImGui::MenuItem("Light Style"))
        {
          m_dark_mode = false;
        }
        if(ImGui::MenuItem("Dark Style"))
        {
          m_dark_mode = true;
        }
        ImGui::EndMenu();
      }
#ifdef _DEBUG
			if (ImGui::BeginMenu("Window"))
			{
				if(ImGui::MenuItem("Show ImGui Debug Window", 0, m_show_imgui_debug_window))
				{
					m_show_imgui_debug_window = !m_show_imgui_debug_window;
				}
				if(ImGui::MenuItem("Show ImGui Demo", 0, m_show_imgui_demo))
				{
					m_show_imgui_demo = !m_show_imgui_demo;
				}
				if(ImGui::MenuItem("Show ImGui User Guide", 0, m_show_imgui_user_guide))
				{
					m_show_imgui_user_guide = !m_show_imgui_user_guide;
				}
				ImGui::EndMenu();
			}

      //FPS
      renderImGuiFPS();
#endif
			ImGui::EndMainMenuBar();
		}

    if(m_show_quit_window)
    {
      //https://github.com/ocornut/imgui/issues/1422
      if(m_openPupUpOnce)
      {
        ImGui::OpenPopup("Are You Sure You Want To Quit?");
        m_openPupUpOnce = false;
      }

      if(ImGui::BeginPopupModal("Are You Sure You Want To Quit?", &m_show_quit_window))
      {
        if (ImGui::Button("Save And Quit", ImVec2(250.0f, 20.0f)))
        {
          if(saveProjectFile())
          {
            //Save then quit
            //m_engine_controller->getEngineRunner()->getPersistenceManipulator().lock()->setPersistenceLocation("Data/");
            m_engine_controller->getEngineRunner()->getEngine()->getPersistenceManipulator().lock()->saveCurrentScene();
            //m_nodegraph_window->saveGraph();

            m_running = false;
          }
        }
        if (ImGui::Button("Quit Without Saving", ImVec2(250.0f, 20.0f)))
        {
          //Quit
          m_running = false;
        }
        if (ImGui::Button("Cancel", ImVec2(250.0f, 20.0f)))
        {
          m_show_quit_window = false;
          ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
      }

    }

    if(m_file_selector.render())
    {
      // If you want to copy the (valid) returned path somewhere, you can use something like:
      std::filesystem::path l_path = m_file_selector.getPath();
      std::string l_extension = l_path.extension().string();

      if(!m_coming_from_file_open_project)
      {
        //Make sure we are only opening up scene files
        if (l_extension == ".cee")
        {
          std::string l_return = l_path.stem().string();

          m_logger.AddLog("[EDITOR] Loading scene: %s.\n", l_return.c_str());

          m_scene_window->getEditorObjectManager().clearObjectList();

          //m_engine_controller->getEngineRunner()->getPersistenceManipulator().lock()->setPersistenceLocation("Data/");
          m_engine_controller->getEngineRunner()->getEngine()->getPersistenceManipulator().lock()->load(l_return);
          auto l_scene_manip = m_engine_controller->getEngineRunner()->getEngine()->getSceneManipulator().lock();
          l_scene_manip->setPhysicsGravity(vector3(0.0f, 0.0f, 0.0f));
          // contact undo redo system to clear its history
          UndoRedoManager::GetInstance().ClearHistory();

          m_hierarchy_window->createEditorObjects();

          m_file_selector.setVisible(false);
        }
        else
        {
          m_logger.AddLog("[WARNING] %s is not a valid extension.\n Please select a .cee file.\n", l_extension.c_str());
        }
      }
      else
      {
        //Make sure we are only opening up .prj files
        if (l_extension == ".prj")
        {
          std::string l_project = l_path.string();

          if(openProjectFile(l_project))
          {
            m_project_folder_set = true;
          }
          else
          {
            m_project_folder_set = false;
          }

          m_file_selector.setVisible(false);
        }
        else
        {
          m_logger.AddLog("[WARNING] %s is not a valid extension.\n Please select a .prj file.\n", l_extension.c_str());
        }
      }
    }
    else
    {
      if(m_file_selector.getClosed())
      {
        m_coming_from_file_open_project = false;
      }
    }

    if(m_folder_selector.render())
    {
      std::filesystem::path l_path = m_folder_selector.getPath();
      std::string l_project = l_path.string();

      m_folder_selector.setVisible(false);
      //Create .prj file
      if(createProjectFile(l_project))
      {
        m_project_folder_set = true;

        if (m_coming_from_file_save_scene)
        {
          //Save here
          //m_engine_controller->getEngineRunner()->getPersistenceManipulator().lock()->setPersistenceLocation("Data/");
          //m_engine_controller->getEngineRunner()->getPersistenceManipulator().lock()->saveCurrentScene();
        }
      }
      else
      {
        m_project_folder_set = false;
      }

      m_coming_from_file_save_scene = false;
    }

    if(m_show_welcome_window)
    {
      if(m_showWelcomeOnce)
      {
        ImGui::OpenPopup("Please choose a project option.");
        m_showWelcomeOnce = false;
      }

      if (ImGui::BeginPopupModal("Please choose a project option.", NULL))
      {
        if (ImGui::Button("Open Existing Project.", ImVec2(250.0f, 20.0f)))
        {
          m_file_selector.setVisible(true);
          m_coming_from_file_open_project = true;
          m_show_welcome_window = false;
        }

        if (ImGui::Button("Create New Project.", ImVec2(250.0f, 20.0f)))
        {
          m_folder_selector.setVisible(true);
          m_show_welcome_window = false;
        }

        if (ImGui::Button("Quit", ImVec2(250.0f, 20.0f)))
        {
          m_show_welcome_window = false;
          m_running = false;
        }

        ImGui::EndPopup();
      }
    }

		//  display the toolbar
//		m_toolbar_renderer.render(&m_current_state);

		// display tabs
		// hierarchy window must be rendered first to get the selection updates
		if (ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_MenuBar))
		{
			m_hierarchy_window->render();
		}
		ImGui::End();

		// set the properties for the gizmo
		m_gizmo_renderer.pullData();



		// scene window must be rendered after the gizmo has been updated
		if (ImGui::Begin("Scene"))
		{
			m_scene_window->render();
		}
		ImGui::End();

		// inspector window must be rendered after the gizmo has been updates
		if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_MenuBar))
		{
			m_inspector_window->render();
		}
		ImGui::End();

		// still empty as of September 10th, 2019
		if (ImGui::Begin("Project"))
		{
		}
		ImGui::End();

		m_nodegraph_window->render();

		//if(ImGui::Begin("Data Select"))
		//{
		//	m_dataSelect_window->render();
		//}
		//ImGui::End();

		if(ImGui::Begin("Tools"))
		{
			m_navmesh_window->render();
		}
    ImGui::End();

		if (ImGui::Begin("Graphics"))
		{
			m_graphics_window->render();
		}
    ImGui::End();

    bool l_showLog = true;

    m_logger.Draw("Output", &l_showLog);

		// push gizmo properties back into engine if it changed
    if(m_gizmo_renderer.getIsChanged())
		  m_gizmo_renderer.pushData();

    m_hierarchy_selection.updateSelected();
	}


	if (m_show_imgui_debug_window)
	{
		ImGui::ShowDockingDebug();
	}
	if (m_show_imgui_demo)
	{
		ImGui::ShowDemoWindow(NULL);
	}
	if(m_show_imgui_user_guide)
	{
		ImGui::ShowUserGuide();
	}

	ImGui::End();

}
