/*!**********************************************************************************************************
\file       GamepadInputAdapter.cpp
\author     Henry Brobeck
\date       10/26/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
//========  Self Include  =================================================================================//

#include "../Precompiled.h"

#include "GamepadInputAdapter.hpp"

#include "GamepadControlDefines.hpp"

#include <winnt.h>
#include <cstring>

namespace
{
	void defaultButton(unsigned short p_gamepadCode, actionType action, unsigned short p_id)
	{
		UNUSED_PARAMETER(p_gamepadCode);
		UNUSED_PARAMETER(action);
		UNUSED_PARAMETER(p_id);
	}
	void defaultAxis(const float* p_axisValues, size_t p_numAxis, unsigned short p_id)
	{
		UNUSED_PARAMETER(p_axisValues);
		UNUSED_PARAMETER(p_numAxis);
		UNUSED_PARAMETER(p_id);
	}
	void defaultConnection(unsigned short p_id, actionType p_action)
	{
		UNUSED_PARAMETER(p_id);
		UNUSED_PARAMETER(p_action);
	}

	float normalizeValue(int p_min, int p_max, int p_value)
	{
		auto minOffset = float(-p_min);
		float max = float(p_max) + minOffset;
		float val = p_value + minOffset;
		return (val / max);

	}
} // namespace

gamepadInputAdapter::gamepadInputAdapter() : m_gamepadButtonCallback(defaultButton),
	m_gamepadAxisCallback(defaultAxis),
	m_connectionCallback(defaultConnection),
	m_controllerState{{0, {0, 0, 0, 0, 0, 0, 0}}},
	m_controllerStateLast{{0, {0, 0, 0, 0, 0, 0, 0}}},
	m_controllerConnected{0},
	m_controllerConnectedLast{0}
{
}

void gamepadInputAdapter::enableMouseCapture()
{
	// no use
}

void gamepadInputAdapter::disableMouseCapture()
{
	// no use
}

void gamepadInputAdapter::setKeyboardCallback(keyboardCallbackFunction p_function_ptr)
{
	//no use
	(void)p_function_ptr;
}

void gamepadInputAdapter::setMousePosCallback(mousePosCallbackFunction p_function_ptr)
{
	//no use
	(void)p_function_ptr;
}

void gamepadInputAdapter::setMouseButtonCallback(mouseButtonCallbackFunction p_function_ptr)
{
	//no use
	(void)p_function_ptr;
}

void gamepadInputAdapter::setGamepadButtonCallback(gamepadButtonCallbackFunction p_function_ptr)
{

	m_gamepadButtonCallback = p_function_ptr;
}

void gamepadInputAdapter::setGamepadAxisCallbackFunction(gamepadAxisCallbackFunction p_function_ptr)
{
	m_gamepadAxisCallback = p_function_ptr;
}

scancodeWrapper gamepadInputAdapter::mapLetterToScancode(unsigned p_letter)
{
	UNREFERENCED_PARAMETER(p_letter);
	//no use
	return {};
}

void gamepadInputAdapter::setConnectionStateCallbackFunction(connectionStateCallbackFunction p_function_ptr)
{
	m_connectionCallback = p_function_ptr;
}

void gamepadInputAdapter::updateState()
{

	//copy last frame data over
    std::memcpy(m_controllerStateLast, m_controllerState, sizeof(m_controllerState));
	std::memcpy(m_controllerConnectedLast, m_controllerConnected, sizeof(m_controllerConnectedLast));

	DWORD result;
	for (WORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		memset(&m_controllerState[i], 0, sizeof(XINPUT_STATE));
		result = XInputGetState(i, &m_controllerState[i]);
		m_controllerConnected[i] = (result == ERROR_SUCCESS);

		//process data to callbacks
		//connection state
		if(!m_controllerConnected[i])
		{
			if(m_controllerConnectedLast[i])
			{
				//disconnected
				m_connectionCallback(i, actionType::Released);
			}
			//controller is disconnected, nothing else to process
			continue;
		}

		if(!m_controllerConnectedLast[i])
		{
			m_connectionCallback(i, actionType::Pressed);
		}
		else
		{
			m_connectionCallback(i, actionType::Repeat);
		}
		//do buttons
		for(WORD b = 0; b < XINPUT_BUTTON_OFFSET_MAX; ++b)
		{
			bool current = m_controllerState[i].Gamepad.wButtons & (1u << gpb_offsets[b]);
			bool last = m_controllerStateLast[i].Gamepad.wButtons & (1u << gpb_offsets[b]);

			if(current)
			{
				if(last)
				{
					m_gamepadButtonCallback(gpb_offsets[b], actionType::Repeat, i);
				}
				else
				{
					m_gamepadButtonCallback(gpb_offsets[b], actionType::Pressed, i);
				}
			}
			else
			{
				if(last)
				{
					m_gamepadButtonCallback(gpb_offsets[b], actionType::Released, i);
				}
			}

		}

		//do axis
		float axisData[gpa_count];
		//triggers
		axisData[gpa_leftTrigger] = normalizeValue(TRIG_MIN, TRIG_MAX, m_controllerState[i].Gamepad.bLeftTrigger);
		axisData[gpa_rightTrigger] = normalizeValue(TRIG_MIN, TRIG_MAX, m_controllerState[i].Gamepad.bRightTrigger);
		//axis
		axisData[gpa_thumbLX] = normalizeValue(THUMB_MIN, THUMB_MAX, m_controllerState[i].Gamepad.sThumbLX);
		axisData[gpa_thumbLY] = normalizeValue(THUMB_MIN, THUMB_MAX, m_controllerState[i].Gamepad.sThumbLY);
		axisData[gpa_thumbRX] = normalizeValue(THUMB_MIN, THUMB_MAX, m_controllerState[i].Gamepad.sThumbRX);
		axisData[gpa_thumbRY] = normalizeValue(THUMB_MIN, THUMB_MAX, m_controllerState[i].Gamepad.sThumbRY);

		m_gamepadAxisCallback(axisData, gpa_count, i);

	}



}
