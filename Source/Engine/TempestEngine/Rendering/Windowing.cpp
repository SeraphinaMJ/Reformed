/*!***************************************************************************************
\file       Windowing.cpp
\author     Aaron Damyen
\date       5/7/2018
\copyright  All content ? 2017-2018 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  Implementation of the windowing class member functions.
*****************************************************************************************/
//==========1st Party Includes==========================================================//
#include "../Precompiled.h"
#include "Windowing.hpp"
//#include "Windows.h"
#include "../Application.hpp"
#include "../SceneManagement/Transform.hpp"
#include "../SystemManager.hpp"
#include "../Components/Camera.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include "../Components/Renderer.hpp"
#include "../Components/Camera.hpp"
#include <GLFW/glfw3.h>


//==========3rd Party Includes==========================================================//


///////////====================================================================///////////
/////////// Namespace                                                          ///////////
///////////====================================================================///////////

//////////======================================================================//////////
//////////  Class / Struct                                                      //////////
//////////======================================================================//////////

/////////========================================================================/////////
/////////   Public/Protected/Private                                             /////////
/////////========================================================================/////////

////////==========================================================================////////
////////    (Non-)Static                                                          ////////
////////==========================================================================////////

///////============================================================================///////
///////     Data / Functions                                                       ///////
///////============================================================================///////

//////==============================================================================//////
//////      Data / Functions                                                        //////
//////==============================================================================//////

/////================================================================================/////
////        (Non-) Configurable                                                      /////
/////================================================================================/////

/*!***************************************************************************************
\brief  function for getting errors from glfwSetErrorCallback
\param  errnum  - error number
\param  errstr  - error string
*****************************************************************************************/
void errorcallback(int errnum, const char * errstr)
{
	std::stringstream ss;

	ss << "GLFW ERROR "<< errnum << "::"<<errstr<< "\n";

	//  OutputDebugString(ss);
}
////==================================================================================////
///         (Pure/Overloaded/Non-) Virtual                                            ////
////==================================================================================////
void windowing::onInitialize()
{
	m_windowTitle = "RumbleOfToys";
	m_windowWidth = 1280;
	m_windowHeight = 760;

	glfwSetErrorCallback(errorcallback);

	// initialize GLFW & OpenGL 3.3-specific stuff

	/* TODO(Cody): Memory leak, I suspect something not being cleaned up.
	* 'TempestEngine.dll!glfwPlatformGetMonitors() + 0x1C9 bytes' is leaking and being
	* called from 'glfwInit()'. Info from VLD: (Count: 1, Total 32 bytes) */
	if (!glfwInit())
	{
		logger("windowing").error() << "GLFW initialization fail!"; // it said init before, MP changed it
		glfwTerminate();
		return;
	}

	// set up window props
	// version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// checks for depricated code
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// allows forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// open the window
	m_windowHandle = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle.c_str(), NULL, NULL);

	if (!m_windowHandle) {
		logger("windowing").error() << "GLFW window open fail!";
		glfwTerminate();
	}

	// Get buffersize dementions
	glfwGetFramebufferSize(m_windowHandle, &m_bufferWidth, &m_bufferHeight);

	// Set context for GLEW
	glfwMakeContextCurrent(m_windowHandle);

	// link OpenGL using GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		logger("windowing").error() << "GLFW initialization fail!"; // it said init before, MP changed it
		glfwDestroyWindow(m_windowHandle);
		glfwTerminate();
		return;
	}

	glViewport(0, 0, m_bufferWidth, m_bufferHeight);
	std::string message("windowing initialized\n");
	//m_graphics = std::shared_ptr<graphicsSystem>(new graphicsSystem());
	//m_graphics->onInitialize();
	//; // outputs directly to the Output tab in VS.
}

void windowing::onStartFrame()
{
	if (glfwWindowShouldClose(m_windowHandle))
	{
		application::requestQuit();
		return;
	}
	// TEMP: request quitting after a few moments to verify engine cycle works.
	// Poll for all window events
	glfwPollEvents();


	GameObjectFiltering::componentTypeSet pattern;
	pattern.setType(camera::getType());
	auto list = getSystemManager()->getGameObjectGatherer()->getList(pattern);

	if (glfwGetKey(m_windowHandle, GLFW_KEY_A) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalPosition();
		current[0] -= .03f;
		list[0]->getComponent<transform>()->setPosition(current);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_D) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalPosition();
		current[0] += .03f;
		list[0]->getComponent<transform>()->setPosition(current);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_W) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalPosition();
		current[1] += .03f;
		list[0]->getComponent<transform>()->setPosition(current);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_S) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalPosition();
		current[1] -= .03f;
		list[0]->getComponent<transform>()->setPosition(current);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_Q) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalPosition();
		current[2] -= .03f;
		list[0]->getComponent<transform>()->setPosition(current);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_E) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalPosition();
		current[2] += .03f;
		list[0]->getComponent<transform>()->setPosition(current);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_C) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalRotation();
		current[2] += .03f;
		list[0]->getComponent<transform>()->setRotation(current);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_X) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalRotation();
		current[2] -= .03f;
		list[0]->getComponent<transform>()->setRotation(current);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_V) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalRotation();
		current[1] += .03f;
		list[0]->getComponent<transform>()->setRotation(current);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_V) == GLFW_PRESS)
	{
		vector3 current = list[0]->getComponent<transform>()->getGlobalRotation();
		current[1] -= .03f;
		list[0]->getComponent<transform>()->setRotation(current);
	}



	// Clear window
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void windowing::onEndFrame()
{
	glfwSwapBuffers(m_windowHandle);
}

void windowing::onUpdate()
{
	//m_graphics->update();

}

void windowing::onShutdown()
{
	std::string message = "windowing shutting down\n";

	//m_graphics->shutdown();
	//OutputDebugString(message.c_str());
}

//======================================================================================//
//          Helper / Constroctor / Destructor                                          ///
//======================================================================================//

//======================================================================================//
//          Other                                                                       //
//======================================================================================//
