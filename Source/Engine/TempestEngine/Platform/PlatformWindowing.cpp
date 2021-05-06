/*!***************************************************************************************
\file       PlatformWindowing.cpp
\author     Aaron Damyen
\date       8/10/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the platformWindowing class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "PlatformWindowing.hpp"
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include <GL/glew.h>
#include <cassert>
#include <ExternalWindowInterface.hpp>
#include <RenderContextInterface.hpp>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

///////========================================================================///////
///////   Public                                                               ///////
///////========================================================================///////

//==============================================================================//
//        Constructor                                                           //
//==============================================================================//
platformWindowing::platformWindowing(externalWindowInterface * p_window_ptr, renderContextInterface * p_render_context_ptr)
{
	assert(p_window_ptr != nullptr && "pointer to externalWindowInterface cannot be nullptr");
	assert(p_render_context_ptr != nullptr && "pointer to renderContextInterface cannot be nullptr");

	m_window_ptr = p_window_ptr;
	m_render_context_ptr = p_render_context_ptr;
}

std::pair<int, int> platformWindowing::getWindowDimentions()
{
	return m_window_ptr->getWindowDimentions();
}


//==============================================================================//
//        Destructor                                                            //
//==============================================================================//

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////

///////========================================================================///////
///////   Protected                                                            ///////
///////========================================================================///////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////

void platformWindowing::onStartFrame()
{
	m_window_ptr->handleProcessEvents(); // process external window events

	// check for a resolution change
	if(m_render_context_ptr->queryContextChanged())
	{
		// do something here to update the rendering size?? cody??
		//  glviewport only seems to render when the window is really short.
		//glViewport(0, 0, m_render_context_ptr->queryContextParameters().width, m_render_context_ptr->queryContextParameters().height);
	}

}

void platformWindowing::onEndFrame()
{
}

void platformWindowing::presentFrame()
{
	m_render_context_ptr->handleSwapBuffersRequest();
}

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//==============================================================================//
//        Helper                                                                //
//==============================================================================//





