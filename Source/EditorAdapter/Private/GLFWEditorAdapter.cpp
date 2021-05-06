/*!***************************************************************************************
\file       GLFWEditorAdapter.cpp
\author     Aaron Damyen
\date       11/14/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the EngineInterfaces specifically for the Editor
*****************************************************************************************/

//======== Self Include ================================================================//
#include "GLFWEditorAdapter.hpp"
//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

glfwEditorAdapter::glfwEditorAdapter(glfwWindowData* p_data)
	: glfwAdapter(p_data)
{ }


void glfwEditorAdapter::handleProcessEvents()
{
	// no longer handled here, but done by the editor rendering
}

void glfwEditorAdapter::handleResizeRequest(int p_width, int p_height)
{
	// ignore resize requests
}

void glfwEditorAdapter::handleFullscreenToggleRequest()
{
	// ignore such requests
}

void glfwEditorAdapter::handleQuitRequest()
{
	// ignore any requests to quit from the engine
	// TODO: handle this properly (propogate it to the editor)
}

std::pair<int, int> glfwEditorAdapter::getWindowDimentions()
{
	return std::pair<int, int>();
}

void glfwEditorAdapter::resize(int p_width, int p_height)
{
	if (m_parameters.width != p_width || m_parameters.height != p_height)
	{
		m_parameters.width = p_width;
		m_parameters.height = p_height;
		m_parameters_changed = true;
	}
}

void glfwEditorAdapter::setOffset(int p_x, int p_y)
{
	m_x_offset = p_x;
	m_y_offset = p_y;
}

void glfwEditorAdapter::onGLFWMousePosCallback(double p_xpos, double p_ypos)
{
	glfwAdapter::onGLFWMousePosCallback(p_xpos - m_x_offset, p_ypos - m_y_offset);
}

void glfwEditorAdapter::handleSwapBuffersRequest()
{
	// no longer handled here, but done by the editor rendering
}


//======== Helper Functions ============================================================//


