/*!***************************************************************************************
\file       GLFWEditorAdapter.hpp
\author     Aaron Damyen
\date       11/14/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the EngineInterfaces for the Editor
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <GLFWAdapter.hpp>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/

class glfwEditorAdapter : public glfwAdapter
{
public:
	glfwEditorAdapter(glfwWindowData * p_data);
	
	void handleSwapBuffersRequest() override;
	void handleQuitRequest() override;
	void handleProcessEvents() override;
	void handleResizeRequest(int p_width, int p_height) override;
	void handleFullscreenToggleRequest() override;
	
	std::pair<int, int> getWindowDimentions() override;

	void resize(int p_width, int p_height);
	void setOffset(int p_x, int p_y);

protected:
	void onGLFWMousePosCallback(double p_xpos, double p_ypos) override;
	
private:
	int m_x_offset = 0;
	int m_y_offset = 0;
};


/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

// namespace <namespace name>

