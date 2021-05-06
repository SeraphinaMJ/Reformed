/*!***************************************************************************************
\file       main.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include <Engine.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "EngineController.hpp"
#include "EditorWindow.hpp"
#include "DataSelect.hpp"
#include <filesystem>

#include "FileOpener.hpp"

#include <fstream>

#include "EditorLogger.hpp"
#include <GlfwWindowData.hpp>
#include <GLFWEditorAdapter.hpp>

#pragma comment(lib, "shlwapi.lib")

// TODO: How do I expose public utility headers to Editor and Bootstrap??? Patrick
#include "../Utility/Public/Exports.hpp"

extern "C"
{
	// Enable dedicated graphics for NVIDIA Optimius
	TEMPEST_EXPORT unsigned long NvOptimusEnablement = 0x00000001;
	// Enable dedicated graphics for AMD Radeon :
	TEMPEST_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}


const std::string g_windowTitle("ToyBox Editor");

bool graphics_OnInitialize(GLFWwindow*& p_window, int& p_result)
{
	// Initialize glfw library
	if (!glfwInit())
	{
		p_result = -1;
		return true;
	}

	// create a windowed mode window and the opengl context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	p_window = [](std::string const& title) -> GLFWwindow*
	{
		const auto createWindow = [&title](const int width, const int height) -> GLFWwindow*
		{
			return glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		};

		if (auto* monitor = glfwGetPrimaryMonitor(); monitor != nullptr)
		{
			if (auto const* mode = glfwGetVideoMode(monitor); mode != nullptr)
			{
				return createWindow(mode->width, mode->height);
			}
		}

		return createWindow( 1920, 1080 );
	}(g_windowTitle);

	if (p_window == nullptr)
	{
		glfwTerminate();
		p_result = -1;
		return true;
	}

	// make the context current
	glfwMakeContextCurrent(p_window);

	// initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		p_result = 1;
		return true;
	}
	return false;
}

void imgui_OnInitialize(GLFWwindow* window)
{
	// setup imgui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
	ImGui::StyleColorsDark();
}

void editor_OnStartFrame()
{
	// start imgui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void editor_OnEndFrame()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_OnShutdown()
{
	// shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void graphics_OnShutdown(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}




//Forward declare
bool openProjectFile(const std::string & p_project_path, std::string & p_project_file_name, std::string & p_project_file_path, bool & p_dark_mode);
bool createProjectFile(const std::string & p_project_path, std::string & p_project_file_name, std::string & p_project_file_path, bool p_dark_mode);

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
int main(int, char **)
#endif
{
	GLFWwindow* l_window;
	int resultValue;

	// initialize everthing
	if (graphics_OnInitialize(l_window, resultValue))
		return resultValue;
	imgui_OnInitialize(l_window);

	// make the context current
	glfwMakeContextCurrent(l_window);

	// get the current directory path
	//std::string l_base_path;
	//HMODULE l_module_handle = GetModuleHandle(NULL);	
	//if(l_module_handle != NULL)
	//{
	//	char l_exe_path[MAX_PATH] = { 0 };
	//	GetCurrentDirectory((sizeof(l_exe_path)), l_exe_path);
	//	//PathRemoveFileSpec(l_exe_path);
	//	l_base_path = l_exe_path;
	//}

	glfwWindowData l_window_data;
	l_window_data.m_window = l_window;
	glfwEditorAdapter l_adapter(&l_window_data);

	bool l_show_welcome_window = true;
	bool l_show_project_options = true;
	bool l_showWelcomeOnce = true;
	bool l_quit_selected = false;
	bool l_dark_mode = false;

	std::string l_project_file_name;
	std::string l_project_file_path;

	//Must go into iumgui.ini and change the position of [Window][Welcome]
	// Pos = 1200, 60
	Editor::EditorLogger l_logger;
  Editor::FileOpener l_project_selector("../../", Editor::FileOpenerType::enm_file);
  Editor::FileOpener l_project_creator("../../", Editor::FileOpenerType::enm_folder);

	while (l_show_project_options)
	{
		// process events
		glfwPollEvents();

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		editor_OnStartFrame();

		l_logger.Draw("Welcome");

		if (l_show_welcome_window)
		{
			if (l_showWelcomeOnce)
			{
				ImGui::OpenPopup("Please choose a project option.");
				l_showWelcomeOnce = false;
			}

			if (ImGui::BeginPopupModal("Please choose a project option.", NULL))
			{
				if (ImGui::Button("Open Existing Project.", ImVec2(250.0f, 20.0f)))
				{
          l_project_selector.setVisible(true);
					l_show_welcome_window = false;
				}

				if (ImGui::Button("Create New Project.", ImVec2(250.0f, 20.0f)))
				{
          l_project_creator.setVisible(true);
					l_show_welcome_window = false;
				}

				if (ImGui::Button("Quit", ImVec2(250.0f, 20.0f)))
				{
					l_show_welcome_window = false;
					l_show_project_options = false;
					l_quit_selected = true;
				}

				ImGui::EndPopup();
			}
		}

    //Project Selector
		if (l_project_selector.render())
		{
      std::filesystem::path l_path = l_project_selector.getPath();
      std::string l_project = l_path.string();
      std::string l_extension = l_path.extension().string();

      if (l_extension == ".prj")
      {
        if (openProjectFile(l_project, l_project_file_name, l_project_file_path, l_dark_mode))
        {
          l_project_selector.setVisible(false);
          l_show_project_options = false;
        }
        else
        {
          l_logger.AddLog("[ERROR] Unable to open Project file!\n");
        }
      }
      else
      {
        l_logger.AddLog("[WARNING] %s is not a valid extension.\n Please select a .prj file.\n", l_extension.c_str());
      }
    }
    else
    {
      if(l_project_selector.getClosed())
      {
        l_showWelcomeOnce = true;
        l_show_welcome_window = true;
      }
    }

    //Project creator
    if (l_project_creator.render())
    {
      std::filesystem::path l_path = l_project_selector.getPath();
      std::string l_project = l_path.string();

      if (createProjectFile(l_project, l_project_file_name, l_project_file_path, l_dark_mode))
      {
        l_show_project_options = false;
        l_project_creator.setVisible(false);
      }
      else
      {
        l_logger.AddLog("[ERROR] Unable to create project file in that directory!\n");
      }
    }
    else
    {
      if (l_project_creator.getClosed())
      {
        l_show_welcome_window = true;
        l_showWelcomeOnce = true;
      }
    }

		editor_OnEndFrame();

		glfwSwapBuffers(l_window);
	}

	if (!l_quit_selected)
	{
		Editor::editorWindow l_editor_window(&l_adapter, l_project_file_path, l_project_file_name, l_dark_mode);

		ImVec2 p_sceneview_size(1024, 768);

		// setup a FBO for background render-to-texture
		GLuint l_frameBufferID = 0;
		glGenFramebuffers(1, &l_frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, l_frameBufferID);
		// create the texture
		GLuint l_background_texture;
		glGenTextures(1, &l_background_texture);
		glBindTexture(GL_TEXTURE_2D, l_background_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		GLuint l_depth_render_buffer;
		glGenRenderbuffers(1, &l_depth_render_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, l_depth_render_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, l_depth_render_buffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, l_background_texture, 0);
		GLenum l_draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, l_draw_buffers);

		// loop until window closes
		while (l_editor_window.isRunning())
		{
			if (glfwWindowShouldClose(l_window))
			{
				glfwSetWindowShouldClose(l_window, 0);
				l_editor_window.requestShutdown();
			}
			// render engine to offscreeen
			glBindFramebuffer(GL_FRAMEBUFFER, l_frameBufferID);
			glBindTexture(GL_TEXTURE_2D, l_background_texture);
			l_adapter.resize(static_cast<int>(p_sceneview_size.x), static_cast<int>(p_sceneview_size.y));

			l_editor_window.getEngineController().processEvents();

			// switch back to the window context
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// glViewport(0, 0, 1024, 768);

			// process events
			glfwPollEvents();

			// render
			glClear(GL_COLOR_BUFFER_BIT); // is this complete?


			editor_OnStartFrame();
			l_editor_window.getSceneWindow().setImageSize(p_sceneview_size.x, p_sceneview_size.y);
			l_editor_window.getSceneWindow().setTextureID(reinterpret_cast<void*>(l_background_texture));
			l_editor_window.render();

			editor_OnEndFrame();
			glfwSwapBuffers(l_window);
		}

		l_editor_window.shutdown();
	}

	imgui_OnShutdown();
	graphics_OnShutdown(l_window);

	return 0;
}

bool openProjectFile(const std::string & p_project_path, std::string & p_project_file_name, std::string & p_project_file_path, bool & p_dark_mode)
{
	std::ifstream l_project(p_project_path);

	if (l_project.good())
	{
		//m_logger.AddLog("[EDITOR] Opening project file:\n %s\n", p_project_path.c_str());

		char l_result[4096];
		ImGuiFs::PathGetFileName(p_project_path.c_str(), l_result);
		char l_resultPath[4096];
		ImGuiFs::PathGetDirectoryName(p_project_path.c_str(), l_resultPath);

		p_project_file_name = l_result;
		p_project_file_path = l_resultPath;

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

		p_dark_mode = l_found;

		l_project.close();
	}
	else
	{
		//m_logger.AddLog("[ERROR] Failed to open project file:\n %s\n", p_project_path.c_str());
		return false;
	}

	return true;
}

bool createProjectFile(const std::string & p_project_path, std::string & p_project_file_name, std::string & p_project_file_path, bool p_dark_mode)
{
	//Get the end folder
	char l_return[4096];
	ImGuiFs::PathGetFileName(p_project_path.c_str(), l_return);
	std::string l_fileName(l_return);
	//l_fileName.insert(0, 1, '/');
	l_fileName.append(".prj");

	std::string l_fullPath = p_project_path + '/' + l_fileName;

	std::ifstream l_exisitingFile(l_fullPath);
	if (l_exisitingFile.good())
	{
		//m_logger.AddLog("[ERROR] Project File already exists here! Please choose an empty project folder.");
		return false;
	}

	std::ofstream l_file(l_fullPath);

	if (l_file.is_open())
	{
		l_file << "Project Path: " << l_fullPath << std::endl;
		//Save style
		if (p_dark_mode)
		{
			l_file << "Style: " << "dark" << std::endl;
		}
		else
		{
			l_file << "Style: " << "light" << std::endl;
		}

		l_file.close();

		p_project_file_path = p_project_path;
		p_project_file_name = l_fileName;

		//m_logger.AddLog("[EDITOR] Creating project file:\n %s", l_fullPath.c_str());
	}
	else
	{
		//m_logger.AddLog("[ERROR] Can not create project file in directory: %s.\n Please choose a writable directory.\n", p_project_path.c_str());
		return false;
	}

	//Project file creation success.
	return true;
}
