/*!***************************************************************************************
\file       InputSourceInterface.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include <cstddef> // std::size_t

enum class actionType
{
	Released = 0,
	Pressed = 1,
	Repeat = 2
};

// typedefs
struct scancodeWrapper { unsigned int scancode; };
typedef void(*keyboardCallbackFunction)(scancodeWrapper p_scancode, actionType p_action, unsigned int p_mods);
typedef void(*mousePosCallbackFunction)(double p_xpos, double p_ypos);
typedef void(*mouseButtonCallbackFunction)(unsigned int p_button, actionType p_action, int p_mods);
typedef void(*gamepadButtonCallbackFunction)(unsigned short p_gamepadcode, actionType p_action, unsigned short p_id);
typedef void(*gamepadAxisCallbackFunction)(const float* p_axisValues, size_t p_numAxis, unsigned short p_id);
typedef void(*connectionStateCallbackFunction)(unsigned short p_id, actionType action);

class inputSourceInterface
{
public:
	virtual void enableMouseCapture() = 0;
	virtual void disableMouseCapture() = 0;
	virtual void setKeyboardCallback(keyboardCallbackFunction p_function_ptr) = 0;
	virtual void setMousePosCallback(mousePosCallbackFunction p_function_ptr) = 0;
	virtual void setMouseButtonCallback(mouseButtonCallbackFunction p_function_ptr) = 0;
	virtual void setGamepadButtonCallback(gamepadButtonCallbackFunction p_function_ptr) = 0;
	virtual void setGamepadAxisCallbackFunction(gamepadAxisCallbackFunction p_function_ptr) = 0;
	virtual void setConnectionStateCallbackFunction(connectionStateCallbackFunction p_function_ptr) { (void)p_function_ptr; };
	virtual scancodeWrapper mapLetterToScancode(unsigned int p_letter) = 0;

};

