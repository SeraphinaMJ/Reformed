/*!**********************************************************************************************************
\file       GamePadSystem.cpp
\author     Henry Brobeck
\date       10/16/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief 
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "GamePadSystem.hpp"
//========  1st Party Includes  ===========================================================================//
#include "xinput.h"

//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
#define MOTOR_MAX 65535
std::string gamepadSystem::m_name("gamepadSystem");


gamepadSystem::gamepadSystem(): log("GamepadSystem"), m_inputAdapter(new gamepadInputAdapter)
{
}

gamepadSystem::~gamepadSystem()
{
	delete m_inputAdapter;
}

void gamepadSystem::onInitialize()
{
	m_inputAdapter->updateState();
}

void gamepadSystem::onStartFrame()
{
	m_inputAdapter->updateState();
}

void gamepadSystem::onShutdown()
{
}

std::string& gamepadSystem::getName()
{
	return m_name;
}

const std::string& gamepadSystem::name() const
{
	return m_name;
}

gamepadInputAdapter* gamepadSystem::getInputAdapter()
{
	return m_inputAdapter;
}

void gamepadSystem::setFFB(unsigned short p_id, float p_left, float p_right)
{
	XINPUT_VIBRATION vibration;
	DWORD controller = p_id;
	vibration.wLeftMotorSpeed = WORD(p_left * MOTOR_MAX);
	vibration.wRightMotorSpeed = WORD(p_right * MOTOR_MAX);
	XInputSetState(controller, &vibration);
}

//========  Static Deceleration  ==========================================================================//
//========  Static Class Methods  =========================================================================//
