/*!***************************************************************************************
\file       InputFramework.cpp
\author     Aaron Damyen
\date       10/22/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the InputFramework API
*****************************************************************************************/

//======== Self Include ================================================================//
#include "InputFramework.hpp"

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <Logger.hpp>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//
std::list<inputSourceInterface*> inputFramework::inputSourceInterfaces;
std::map<letter, scancode> inputFramework::letterToScancodeMap;

bool inputFramework::keys[MAXIMUM_KEY] = { false };
float inputFramework::axes[MAXIMUM_AXIS] = { 0 };
cursorPosition inputFramework::mousePosition = {0, 0};
actionType inputFramework::mouseButtons[MAXIMUM_MOUSEBUTTON] = { actionType::Released };
actionType inputFramework::gamepadButtons[MAX_CONTROLLERS][XINPUT_BUTTON_OFFSET_MAX] = { { actionType::Released } };
float inputFramework::gamepadAxis[MAX_CONTROLLERS][gpa_count] = { {0} };
actionType inputFramework::gamepadConnection[MAX_CONTROLLERS] = { actionType::Released };

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//
void inputFramework::attachInputSource(inputSourceInterface* p_input_source_ptr)
{
	inputSourceInterfaces.push_back(p_input_source_ptr);

	// setup callback functions
	p_input_source_ptr->setKeyboardCallback(inputFramework::inputKeyboardListener);
	p_input_source_ptr->setMousePosCallback(inputFramework::inputMousePosListener);
	p_input_source_ptr->setMouseButtonCallback(inputFramework::inputMouseButtonListener);
	p_input_source_ptr->setConnectionStateCallbackFunction(inputFramework::inputConnectionStateListener);
	p_input_source_ptr->setGamepadAxisCallbackFunction(inputFramework::inputGamepadAxisListener);
	p_input_source_ptr->setGamepadButtonCallback(inputFramework::inputGamepadButtonListener);
}

void inputFramework::detachInputSource(inputSourceInterface * p_input_source_ptr)
{
	// shutdown callback functions
	p_input_source_ptr->setKeyboardCallback(nullptr);
	p_input_source_ptr->setMousePosCallback(nullptr);
	p_input_source_ptr->setMouseButtonCallback(nullptr);
	p_input_source_ptr->setConnectionStateCallbackFunction(nullptr);
	p_input_source_ptr->setGamepadAxisCallbackFunction(nullptr);
	p_input_source_ptr->setGamepadButtonCallback(nullptr);

	inputSourceInterfaces.remove(p_input_source_ptr);
}


bool inputFramework::getKey(letter p_key)
{
	auto i_iter = letterToScancodeMap.find(p_key);
	unsigned int code = 0;
	if(i_iter == letterToScancodeMap.end())
	{
		code = mapLetterToScancode(p_key);
		if (code != 0)
		{
			letterToScancodeMap[p_key] = code;
		}
	}
	else
	{
		code = i_iter->second;
	}
	// keys are stored by scancode
	return keys[code];
}

float inputFramework::getAxis(axis p_axis)
{
	return axes[p_axis];
}

std::pair<double, double> inputFramework::getMousePosition()
{
	return mousePosition;
}

actionType inputFramework::getMouseButton(MouseButton p_button)
{
	return mouseButtons[static_cast<unsigned int>(p_button)];
}

actionType inputFramework::getGamepadButton(input_button p_button, gamepad_id p_id)
{
	return gamepadButtons[p_id][p_button];
}

float inputFramework::getGamepadAxis(axis p_axis, gamepad_id p_id)
{
	return gamepadAxis[p_id][p_axis];
}

actionType inputFramework::getControllerState(gamepad_id p_id)
{
	return gamepadConnection[p_id];
}

void inputFramework::enableMouseCapture()
{
	for(auto & i_iter : inputSourceInterfaces)
	{
		i_iter->enableMouseCapture();
	}
}

void inputFramework::disableMouseCapture()
{
	for(auto & i_iter : inputSourceInterfaces)
	{
		i_iter->disableMouseCapture();
	}
}

unsigned inputFramework::mapLetterToScancode(letter p_letter)
{
	for(auto & i_iter : inputSourceInterfaces)
	{
		auto code = i_iter->mapLetterToScancode(p_letter).scancode;
		if (code != 0) return code;
	}
	return 0;
}


void inputFramework::inputKeyboardListener(scancodeWrapper p_scancode, actionType p_action, unsigned int /*p_mods*/)
{
	logger("Input").debug() << p_scancode.scancode;
	switch (p_action)
	{
	case actionType::Pressed:
		keys[p_scancode.scancode] = true;
		break;
	case actionType::Released:
		keys[p_scancode.scancode] = false;
		break;
	case actionType::Repeat:
	default:
		// TODO: Log a warning here
		;
	}
}

void inputFramework::inputMousePosListener(double p_xpos, double p_ypos)
{
	mousePosition.first = static_cast<float>(p_xpos);
	mousePosition.second = static_cast<float>(p_ypos);
}

void inputFramework::inputMouseButtonListener(unsigned int p_button, actionType p_action, int p_mods)
{
	mouseButtons[p_button] = p_action;
}

void inputFramework::inputGamepadButtonListener(unsigned short p_gamepadcode, actionType p_action, unsigned short p_id)
{
	gamepadButtons[p_id][p_gamepadcode] = p_action;
}

void inputFramework::inputGamepadAxisListener(const float* p_axisValues, size_t p_numAxis, unsigned short p_id)
{
	for (size_t i = 0; i < p_numAxis; ++i)
	{
		gamepadAxis[p_id][i] = p_axisValues[i];
	}
}

void inputFramework::inputConnectionStateListener(unsigned short p_id, actionType p_action)
{
	gamepadConnection[p_id] = p_action;
}


//======== Helper Functions ============================================================//


