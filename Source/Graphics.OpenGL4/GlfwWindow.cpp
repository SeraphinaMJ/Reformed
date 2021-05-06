/*!***************************************************************************************
\file       GlfwWindow.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "GlfwWindow.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlfwWindowData.hpp"

#include <tuple>

glfwWindow::glfwWindow(int p_version_major, int p_version_minor, int p_width, int p_height, std::string p_title, bool fullscreen)
	: m_data{new glfwWindowData()}
{
	// initialize glfw library
	if (!glfwInit())
	{
		return;
	}

	// create a windowed mode window and the OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p_version_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

	std::tie(m_data->m_window, m_data->m_fullscreen) = [&p_width, &p_height, &p_title] () -> std::pair<GLFWwindow*, bool>
	{
		GLFWmonitor * monitor = [](const bool fullscreen) -> GLFWmonitor *
		{
#ifndef _DEBUG
			if (fullscreen)
			{
				return glfwGetPrimaryMonitor();
			}
#endif

			return nullptr;
		}(false); //TODO: We can't start the window in fullscreen, or there be consequences >:(


		if (monitor)
		{
			if (auto const* mode = glfwGetVideoMode(monitor); mode)
			{
				return { glfwCreateWindow(mode->width, mode->height, p_title.c_str(), monitor, nullptr), true };
			}
		}

		return { glfwCreateWindow(p_width, p_height, p_title.c_str(), nullptr, nullptr), false };
	}();


	if (!m_data->m_window)
	{
		glfwTerminate();
		return;
	}

	// make the context current
	glfwMakeContextCurrent(m_data->m_window);

	// initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		return;
	}

}

glfwWindow::~glfwWindow()
{
	glfwDestroyWindow(m_data->m_window);
	glfwTerminate();
	delete m_data;
}

