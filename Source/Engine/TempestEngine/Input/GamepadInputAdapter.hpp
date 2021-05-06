/*!**********************************************************************************************************
\file       GamepadInputAdapter.hpp
\author     Henry Brobeck
\date       10/26/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
************************************************************************************************************/
#pragma once

#include "InputSourceInterface.hpp"
#include "xinput.h"

#define CONTROLLER_MAX_COUNT 4

//========  1st Party Includes  ===========================================================================//
//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Forward Declaration  ==========================================================================//
//========  Enums  ========================================================================================//
//========  Helper Structs  ===============================================================================//

class gamepadInputAdapter final : public inputSourceInterface
{
public:
	//====  Static  =======================================================================================//
	//====  Pure-Virtual  =================================================================================//
	//====  Constructor/Destructor  =======================================================================//

	gamepadInputAdapter();
	~gamepadInputAdapter() = default;

	//====  Accessor Methods  =============================================================================//
	//====  Virtual/Overrides  ============================================================================//
	//====  Operators  ====================================================================================//
	//====  Interface Declarations (For each interface Implemented)  ======================================//
	void enableMouseCapture() override;
	void disableMouseCapture() override;
	void setKeyboardCallback(keyboardCallbackFunction p_function_ptr) override;
	void setMousePosCallback(mousePosCallbackFunction p_function_ptr) override;
	void setMouseButtonCallback(mouseButtonCallbackFunction p_function_ptr) override;
	void setGamepadButtonCallback(gamepadButtonCallbackFunction p_function_ptr) override;
	void setGamepadAxisCallbackFunction(gamepadAxisCallbackFunction p_function_ptr) override;
	scancodeWrapper mapLetterToScancode(unsigned p_letter) override;
	void setConnectionStateCallbackFunction(connectionStateCallbackFunction p_function_ptr) override;
	//====  Everything Else  ==============================================================================//

	void updateState();

	
protected:


private:
	gamepadButtonCallbackFunction m_gamepadButtonCallback{};
	gamepadAxisCallbackFunction m_gamepadAxisCallback{};
	connectionStateCallbackFunction m_connectionCallback{};
	//====  Data  =========================================================================================//
	XINPUT_STATE m_controllerState[CONTROLLER_MAX_COUNT]{};
	XINPUT_STATE m_controllerStateLast[CONTROLLER_MAX_COUNT]{};
	bool m_controllerConnected[CONTROLLER_MAX_COUNT]{};
	bool m_controllerConnectedLast[CONTROLLER_MAX_COUNT]{};
};

// namespace <namespace name>

