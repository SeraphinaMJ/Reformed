/*!***************************************************************************************
\file       GlfwWindow.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include <string>

struct glfwWindowData;

class glfwWindow
{
public:
	glfwWindow(int p_version_major, int p_version_minor, int p_width, int p_height, std::string p_title, bool fullscreen);
	virtual ~glfwWindow();

	glfwWindowData * getDataPointer() const { return m_data; }
	
private:
	glfwWindowData * m_data;

	//friend class glfwAdapter;
};