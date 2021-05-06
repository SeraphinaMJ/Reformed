/*!***************************************************************************************
\file       StressTestScript.cpp
\author     Aaron Damyen
\date       3/15/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the Stress Test script functionality.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "StressTestScript.hpp"

//======== 1st Party Includes ==========================================================//
#include "../Components/InputComponent.hpp"
#include "../SystemManagement/SystemMonitor.hpp"
#include "SpawnerScript.h"
#include "../GamepadControls.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
stressTestScript::stressTestScript(typeRT p_data) : scriptCPP(p_data)
{
}


//======== Getters & Setters ===========================================================//

void stressTestScript::onStart()
{
	componentHandle<inputComponent> l_input = getGameObject().lock()->getComponent < inputComponent>();
	l_input->addButtonMapping("showFPS", '`');
	l_input->addGamepadButtonMapping("showFPS", GamepadButton::gpb_last, 0);

	l_input->addButtonMapping("showCounts", '=');
	l_input->addGamepadButtonMapping("showCounts", GamepadButton::gpb_select, 0);

	l_input->addButtonMapping("emit", 'l');
	l_input->addGamepadButtonMapping("emit", GamepadButton::gpb_a, 0);
}

//======== Overrides ===================================================================//
void stressTestScript::onUpdate()
{
	componentHandle<inputComponent> l_input = getGameObject().lock()->getComponent<inputComponent>();
	if (l_input)
	{
		bool l_show_fps = l_input->getButtonState("showFPS");
		getSystem<systemMonitor>()->showFPS(l_show_fps);

		bool l_show_counts = l_input->getButtonState("showCounts");
		getSystem<systemMonitor>()->showCounts(l_show_counts);

		if(l_input->getButtonState("emit"))
		{
			auto l_objects = findObjectsByComponent<spawnerScript>();
			for(auto l_object : l_objects)
			{
				componentHandle<spawnerScript> l_script = l_object->getComponent<spawnerScript>();
				l_script->spawnEnemy(1);
			}
		}
	}
}

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//


