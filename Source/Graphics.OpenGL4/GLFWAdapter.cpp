/*!***************************************************************************************
\file       GLFWAdapter.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "GLFWAdapter.hpp"
#include <functional>

#include "GlfwWindowData.hpp"

#include <glfw3.h>

#include <array>

static std::vector<actionType> populateActionMap()
{
	std::array<actionType, GLFW_REPEAT + 1> map;

	map[GLFW_RELEASE] = actionType::Released;
	map[GLFW_PRESS]   = actionType::Pressed;
	map[GLFW_REPEAT]  = actionType::Repeat;

	return { map.cbegin(), map.cend() };
}

glfwAdapter::glfwAdapter(glfwWindowData * p_window)
	: m_parameters_changed{true}
	, m_data{ std::exchange(p_window, nullptr) }
	, m_keyboard_callback{ nullptr }
	, m_mouse_pos_callback{ nullptr }
	, m_mouse_button_callback{ nullptr }
	, m_glfw_action_map{ populateActionMap() }
{
	glfwSetWindowUserPointer(m_data->m_window, this);
	

	m_data->m_previousWindowCloseFunc = glfwSetWindowCloseCallback(m_data->m_window,
		[](GLFWwindow * p_window)
	{
		// extract the window details
		auto l_window = static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window));

		// call the previous handler, if present
		if (l_window->m_data->m_previousWindowCloseFunc != nullptr)
			l_window->m_data->m_previousWindowCloseFunc(p_window);

		// do our own handling
		l_window->m_should_close = true;
	});
	m_data->m_previousWindowSizeFunc = glfwSetWindowSizeCallback(m_data->m_window,
		[](GLFWwindow * p_window, int p_width, int p_height)
	{
		// extract the window details
		auto l_window = static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window));

		// call the previous handler, if present
		if (l_window->m_data->m_previousWindowSizeFunc != nullptr)
			l_window->m_data->m_previousWindowSizeFunc(p_window, p_width, p_height);

		// do our own handling
		// update the parameters
		l_window->m_parameters.width = p_width;
		l_window->m_parameters.height = p_height;
		l_window->m_parameters_changed = true;
	});
	m_data->m_previousKeyFunc = glfwSetKeyCallback(m_data->m_window,
		[](GLFWwindow * p_window, int p_key, int p_scancode, int p_action, int p_mods)
	{
		// extract the window details
		auto l_window = static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window));

		// call the previous handler, if present
		if (l_window->m_data->m_previousKeyFunc != nullptr)
			l_window->m_data->m_previousKeyFunc(p_window, p_key, p_scancode, p_action, p_mods);

		// do our own handling
		l_window->onGLFWKeyCallback(p_key, p_scancode, p_action, p_mods);
	});
	m_data->m_previousMousePosFunc = glfwSetCursorPosCallback(m_data->m_window,
		[](GLFWwindow * p_window, double p_xpos, double p_ypos)
	{
		// extract the window details
		auto l_window = static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window));
		
		// call the previous handler, if present
		if (l_window->m_data->m_previousMousePosFunc != nullptr)
			l_window->m_data->m_previousMousePosFunc(p_window, p_xpos, p_ypos);

		// do our own handling
		l_window->onGLFWMousePosCallback(p_xpos, p_ypos);		
	});
	m_data->m_previousMouseButtonFunc = glfwSetMouseButtonCallback(m_data->m_window,
		[](GLFWwindow * p_window, int p_button, int p_action, int p_mods)
	{
		// extract the window details
		auto l_window = static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window));
		
		// call the previous handler, if present
		if (l_window->m_data->m_previousMouseButtonFunc != nullptr)
			l_window->m_data->m_previousMouseButtonFunc(p_window, p_button, p_action, p_mods);

		// do our own handling
		l_window->onGLFWMouseButtonCallback(p_button, p_action, p_mods);
	});

	// populate the dimensions
	std::tie(m_parameters.width, m_parameters.height) = getWindowDimentions();

	// force fullscreen
#ifndef _DEBUG

	handleFullscreenToggleRequest();

#endif


}

glfwAdapter::~glfwAdapter()
{
	// reset the handlers to the old value
	glfwSetWindowCloseCallback(m_data->m_window, m_data->m_previousWindowCloseFunc);
	glfwSetWindowSizeCallback(m_data->m_window, m_data->m_previousWindowSizeFunc);
	glfwSetKeyCallback(m_data->m_window, m_data->m_previousKeyFunc);
	glfwSetCursorPosCallback(m_data->m_window, m_data->m_previousMousePosFunc);
	glfwSetMouseButtonCallback(m_data->m_window, m_data->m_previousMouseButtonFunc);
}

void glfwAdapter::handleProcessEvents()
{
	glfwPollEvents();
}

void glfwAdapter::handleResizeRequest(int p_width, int p_height)
{
	glfwSetWindowSize(m_data->m_window, p_width, p_height);
}

void glfwAdapter::handleFullscreenToggleRequest()
{
	auto *const primary = glfwGetPrimaryMonitor();
	if (!primary) return;

	auto const*const mode = glfwGetVideoMode(primary);
	if (!mode) return;

	m_data->m_fullscreen = !m_data->m_fullscreen;

	auto const& [width, height, rbits, gbits, bbits, refresh] = *mode;

	if (m_data->m_fullscreen)
	{
		m_last_windowed_pos = getWindowPos();
		glfwSetWindowMonitor(m_data->m_window, primary, 0, 0, width, height, refresh);

		std::tie(m_parameters.width, m_parameters.height) = std::tie(width, height);
		m_parameters_changed = true;
	}
	else
	{
		auto const& [x, y] = m_last_windowed_pos;
		glfwSetWindowMonitor(m_data->m_window, nullptr, x, y, width / 2, height / 2, GLFW_DONT_CARE);

		glfwSetWindowAttrib(m_data->m_window, GLFW_RESIZABLE, GLFW_TRUE);
		glfwSetWindowAttrib(m_data->m_window, GLFW_DECORATED, GLFW_TRUE);

	}
}

void glfwAdapter::handleQuitRequest()
{
	// if engine requests a close, we close
	m_should_close = true;
}

bool glfwAdapter::isClosing() const
{
	return m_should_close;
}

bool glfwAdapter::isFullscreen() const
{
	return m_data->m_fullscreen;
}

void glfwAdapter::onGLFWKeyCallback(int p_key, int p_scancode, int p_action, int p_mods)
{
	if ((p_mods & GLFW_MOD_ALT && p_key == GLFW_KEY_ENTER) || p_key == GLFW_KEY_F11)
	{
		if (m_glfw_action_map[p_action] == actionType::Released)
		{
			handleFullscreenToggleRequest();
		}

		return;
	}

	if (m_keyboard_callback != nullptr)
	{
		scancodeWrapper l_scancode = { 0 };
		l_scancode.scancode = p_scancode;

		// override alt-enter and f11 to toggle fullscreen


		m_keyboard_callback(l_scancode, m_glfw_action_map[p_action], p_mods);
	}
}

void glfwAdapter::onGLFWMousePosCallback(double p_xpos, double p_ypos)
{
	if (m_mouse_pos_callback != nullptr)
	{
		m_mouse_pos_callback(p_xpos, p_ypos);
	}
}

void glfwAdapter::onGLFWMouseButtonCallback(int p_key, int p_action, int p_mods)
{
	if (m_mouse_button_callback != nullptr)
	{
		m_mouse_button_callback(p_key, m_glfw_action_map[p_action], p_mods);
	}
}

std::pair<int, int> glfwAdapter::getWindowDimentions()
{
	std::pair<int, int> l_ret;
	glfwGetWindowSize(m_data->m_window, &l_ret.first, &l_ret.second);
	return l_ret;
}

std::pair<int, int> glfwAdapter::getWindowPos()
{
	std::pair<int, int> l_ret;
	glfwGetWindowPos(m_data->m_window, &l_ret.first, &l_ret.second);
	return l_ret;
}

void glfwAdapter::handleSwapBuffersRequest()
{
	glfwSwapBuffers(m_data->m_window);
}

bool glfwAdapter::queryContextChanged()
{
	return m_parameters_changed;
}

renderContextParameters glfwAdapter::queryContextParameters()
{
	m_parameters_changed = false;
	return m_parameters;
}

void glfwAdapter::updateAssetPath(const std::string& /*newPath*/)
{
}

void glfwAdapter::draw(const renderable & /*r*/)
{
}

void glfwAdapter::updateCameraMatrices(const cameraData & /*c*/)
{
}

void glfwAdapter::enableMouseCapture()
{
	// disable the mouse for this window
	glfwSetInputMode(m_data->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// increase the mouse resolution, if possible (removes OS acceleration and effects)
	if (glfwRawMouseMotionSupported())
	{
		m_data->m_original_raw_mouse_motion = glfwGetInputMode(m_data->m_window, GLFW_RAW_MOUSE_MOTION);
		glfwSetInputMode(m_data->m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
}

void glfwAdapter::disableMouseCapture()
{
	// restore mouse resolution setting
	if(glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(m_data->m_window, GLFW_RAW_MOUSE_MOTION, m_data->m_original_raw_mouse_motion);
	}
	// enable the mouse for this window
	glfwSetInputMode(m_data->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void glfwAdapter::setKeyboardCallback(keyboardCallbackFunction p_function_ptr)
{
	m_keyboard_callback = p_function_ptr;
}

void glfwAdapter::setMousePosCallback(mousePosCallbackFunction p_function_ptr)
{
	m_mouse_pos_callback = p_function_ptr;
}

void glfwAdapter::setMouseButtonCallback(mouseButtonCallbackFunction p_function_ptr)
{
	m_mouse_button_callback = p_function_ptr;
}

void glfwAdapter::setGamepadButtonCallback(gamepadButtonCallbackFunction p_function_ptr)
{
	//do nothing: no gamepad through glfw
	(void)p_function_ptr;
}

void glfwAdapter::setGamepadAxisCallbackFunction(gamepadAxisCallbackFunction p_function_ptr)
{
	//do nothing: no gamepad through glfw
	(void)p_function_ptr;
}

scancodeWrapper glfwAdapter::mapLetterToScancode(unsigned p_letter)
{
	scancodeWrapper result = { 0 };
	result.scancode = glfwGetKeyScancode(p_letter);
	return result;
}

