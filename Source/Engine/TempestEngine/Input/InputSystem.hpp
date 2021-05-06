/*!***************************************************************************************
\file       InputSystem.hpp
\author     Aaron Damyen
\date       6/10/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Defines the Input system for converting all input forms into actions within the engine.
*****************************************************************************************/
#pragma once


//======== 1st Party Includes ==========================================================//
#include "../SystemBase.hpp"
//======== 3rd Party Includes ==========================================================//
#include <InputSourceInterface.hpp>
#include <list>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: inputSystem
\brief   Definition of the input system class for wrangling input commands
*****************************************************************************************/
class inputSystem final : public systemBase
{
public:

	inputSystem(inputSourceInterface * p_input_source_ptr) 
	{
		addInputSource(p_input_source_ptr);
	}

	void addInputSource(inputSourceInterface *p_input_source_ptr);

	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("inputSystem"); return n; }
	const std::string& name() const override { return getName(); }

protected:
	/*!***************************************************************************************
	\brief  Handles the initialize event from the system manager to allocate resources and
	        connect to the input source
	*****************************************************************************************/
	void onInitialize() override;

	/*!***************************************************************************************
	\brief  Handles the shutdown event from the system manager to free resources and disconnect
	        from the input source
	*****************************************************************************************/
	void onShutdown() override;

private:
	std::list<inputSourceInterface*> m_inputSources;

};


