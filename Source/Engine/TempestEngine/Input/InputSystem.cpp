/*!***************************************************************************************
\file       InputSystem.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the InputSystem class member functions.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "InputSystem.hpp"
//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <InputFramework.hpp>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//
void inputSystem::addInputSource(inputSourceInterface* p_input_source_ptr)
{
	m_inputSources.push_back(p_input_source_ptr);
}

//======== Overrides ===================================================================//
void inputSystem::onInitialize()
{
	// attach to the input source by setting up the callbacks
	for(auto& source : m_inputSources)
	{
		inputFramework::attachInputSource(source);
	}
}

void inputSystem::onShutdown()
{
	// detach from the input source because our functions are about to go away.
	for(auto & source : m_inputSources)
	{
		inputFramework::detachInputSource(source);
	}
}

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//


