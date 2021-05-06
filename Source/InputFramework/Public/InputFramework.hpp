/*!***************************************************************************************
\file       InputFramework.hpp
\author     Aaron Damyen
\date       10/22/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the InputFramework API which collects various inputs into
        a simplified access point.
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "InputConstants.hpp"
#include "GamepadControlDefines.hpp"
//======== 3rd Party Includes ==========================================================//
#include <InputSourceInterface.hpp>
#include <utility>
#include <map>
#include <list>

//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: inputFramework
\brief   This is 
*****************************************************************************************/
class inputFramework
{
public:
	
	static void attachInputSource(inputSourceInterface * p_input_source_ptr);

	static void detachInputSource(inputSourceInterface * p_input_source_ptr);
	
    /*!***************************************************************************************
	\brief  Retrieves if a specific key is being pressed
	\param p_key - the identifier for the key to check
	\return bool - Returns true if the key is currently pressed, false otherwise
	*****************************************************************************************/
	static bool getKey(letter p_key);

	static float getAxis(axis p_axis);
	

	/*!***************************************************************************************
	\brief  Retrieves if a specific mouse button is being pressed
	\param p_button - the identifier for the mouse button to check
	\return actionType - Returns the current status of the mouse button
	*****************************************************************************************/
	static actionType getMouseButton(MouseButton p_button);

	static cursorPosition getMousePosition();

	static actionType getGamepadButton(input_button p_button, gamepad_id p_id);
	static float getGamepadAxis(axis p_axis, gamepad_id p_id);
	static actionType getControllerState(gamepad_id p_id);

	static void enableMouseCapture();
	
	static void disableMouseCapture();

private:

	/*!***************************************************************************************
	\brief  Converts a letter (or other 'virtual key') into the appropriate scancodeWrapper
	\param p_letter - The letter to convert.  IMPORTANT: On Windows, use uppercase letters!
	\return unsigned int - The scancode equivalent of the parameter letter
	*****************************************************************************************/
	static scancode mapLetterToScancode(letter p_letter);


    /*!***************************************************************************************
	\brief  Handles the callback from the input source for keyboard input.  Note that only the
	        scancodeWrapper is retrieved.  Virtual keys should be translated into scancodes before
	        being used.
	\param p_scancode - the keyboard scancode this event is for
	\param p_action - the action that caused this event
	\param p_mods - modifier keys currently pressed
	*****************************************************************************************/
	static void inputKeyboardListener(scancodeWrapper p_scancode, actionType p_action, unsigned int p_mods);
	/*!***************************************************************************************
	\brief  Handles the callback from the input source for mouse position changes
	\param p_xpos - the current x position of the mouse
	\param p_ypos - the current y position of the mouse
	*****************************************************************************************/
	static void inputMousePosListener(double p_xpos, double p_ypos);
	static void inputMouseButtonListener(unsigned int p_button, actionType p_action, int p_mods);
	/*!***************************************************************************************
	\brief  Handles the callback from the input source for gamepad button changes
	\param p_gamepadcode - the identifier of the button that has changed
	\param p_action - the new value of the button state
	\param p_id - the controller that the button is reported from
	*****************************************************************************************/
	static void inputGamepadButtonListener(unsigned short p_gamepadcode, actionType p_action, unsigned short p_id);
	/*!***************************************************************************************
	\brief  Handles the callback from the input source for gamepad axis changes
	\param p_axisValues - the array of all axis values from the gamepad
	\param p_numAxis - the number of entries in the array
	\param p_id - the controller that the axis values are reported from
	*****************************************************************************************/
	static void inputGamepadAxisListener(const float* p_axisValues, size_t p_numAxis, unsigned short p_id);
	/*!***************************************************************************************
	\brief  Handles the callback from the input source for gamepad state changes
	\param p_id - the controller that has changed state
	\param p_action - the new state of the controller
	*****************************************************************************************/
	static void inputConnectionStateListener(unsigned short p_id, actionType p_action);

	static std::list<inputSourceInterface*> inputSourceInterfaces;
	static std::map<letter, scancode> letterToScancodeMap;
	
	static bool keys[MAXIMUM_KEY]; //!< current status of known buttons (referenced by scancodes)
	static float axes[MAXIMUM_AXIS]; //!< current status of known axes
	static cursorPosition mousePosition; //!< current status of the mouse
	static actionType mouseButtons[MAXIMUM_MOUSEBUTTON]; //!< current status of mouse buttons
	static actionType gamepadButtons[MAX_CONTROLLERS][XINPUT_BUTTON_OFFSET_MAX]; //!< current status of gamepad buttons
	static float gamepadAxis[MAX_CONTROLLERS][gpa_count]; //!< current status of gamepad axes
	static actionType gamepadConnection[MAX_CONTROLLERS]; //!< current status of gamepads

};


