/*!***************************************************************************************
\file       GlfwWindowData.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include <glfw3.h>

struct glfwWindowData
{
	bool m_fullscreen;
	GLFWwindow * m_window;
	int m_original_raw_mouse_motion;

	GLFWwindowclosefun m_previousWindowCloseFunc;
	GLFWwindowsizefun m_previousWindowSizeFunc;
	GLFWkeyfun m_previousKeyFunc;
	GLFWscrollfun m_previousMousePosFunc;
	GLFWmousebuttonfun m_previousMouseButtonFunc;
};
