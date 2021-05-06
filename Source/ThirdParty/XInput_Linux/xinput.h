/*!***************************************************************************************
\file       xinput.h
\author     Tyler Robbins
\date       9/7/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a header file meant to mimic xinput.h as found on the Windows OS
\details Note that the types and constants found here were mostly pulled from
         either the official Microsoft Documentation, or from the XInput header
         itself. Also note that this file has been stripped down to just the
         essentials needed for the project to reduce development time and code
         complexity.
*****************************************************************************************/

#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include "windef.h"

//========Types=========================================================================//

/*!***************************************************************************************
\par enum: _XInputGamepadButtons
\brief   All possible button types on a gamepad.
*****************************************************************************************/
typedef enum _XInputGamepadButtons {
    XINPUT_GAMEPAD_DPAD_UP         = 0x0001,
    XINPUT_GAMEPAD_DPAD_DOWN       = 0x0002,
    XINPUT_GAMEPAD_DPAD_LEFT       = 0x0004,
    XINPUT_GAMEPAD_DPAD_RIGHT      = 0x0008,
    XINPUT_GAMEPAD_START           = 0x0010,
    XINPUT_GAMEPAD_BACK            = 0x0020,
    XINPUT_GAMEPAD_LEFT_THUMB      = 0x0040,
    XINPUT_GAMEPAD_RIGHT_THUMB     = 0x0080,
    XINPUT_GAMEPAD_LEFT_SHOULDER   = 0x0100,
    XINPUT_GAMEPAD_RIGHT_SHOULDER  = 0x0200,
    XINPUT_GAMEPAD_A               = 0x1000,
    XINPUT_GAMEPAD_B               = 0x2000,
    XINPUT_GAMEPAD_X               = 0x4000,
    XINPUT_GAMEPAD_Y               = 0x8000
} XInputGamepadButtons;

//========Defines=======================================================================//

//! Maximum number of gamepad controllers we can have.
#define XUSER_MAX_COUNT 4

//========Forward Deceleration==========================================================//

/*!***************************************************************************************
\par struct: _XINPUT_GAMEPAD
\brief   Describes the current state of the Xbox 360 Controller.
\details For more information, see:
         https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
*****************************************************************************************/
typedef struct _XINPUT_GAMEPAD {
    /**
    \brief Bitmask of the device digital buttons, as follows. A set bit indicates that the
           corresponding button is pressed.
    */
    WORD wButtons;

    /**
    \brief The current value of the left trigger analog control. The value is between 0
           and 255.
    */
    BYTE bLeftTrigger;

    /**
    \brief The current value of the right trigger analog control. The value is between 0
           and 255.
    */
    BYTE bRightTrigger;

    /**
    \brief Left thumbstick x-axis value. The value is between -32768 and 32767.
    */
    SHORT sThumbLX;

    /**
    \brief Left thumbstick y-axis value. The value is between -32768 and 32767.
    */
    SHORT sThumbLY;

    /**
    \brief Right thumbstick x-axis value. The value is between -32768 and 32767.
    */
    SHORT sThumbRX;

    /**
    \brief Right thumbstick y-axis value. The value is between -32768 and 32767.
    */
    SHORT sThumbRY;
} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;

/*!***************************************************************************************
\par struct: _XINPUT_STATE
\brief   Represents the state of a controller.
\details For more information, see:
         https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_state
*****************************************************************************************/
typedef struct _XINPUT_STATE {
    /**
    \brief State packet number. The packet number indicates whether there have been any
           changes in the state of the controller. If the dwPacketNumber member is the
           same in sequentially returned XINPUT_STATE structures, the controller state has
           not changed.
    */
    DWORD dwPacketNumber;

    /**
    \brief XINPUT_GAMEPAD structure containing the current state of an Xbox 360 Controller.
    */
    XINPUT_GAMEPAD Gamepad;
} XINPUT_STATE, *PXINPUT_STATE;

/*!***************************************************************************************
\par struct: _XINPUT_VIBRATION
\brief   Specifies motor speed levels for the vibration function of a controller.
\details For more information, see:
         https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_vibration
*****************************************************************************************/
typedef struct _XINPUT_VIBRATION {
    /**
    \brief Speed of the left motor. Valid values are in the range 0 to 65,535.
           Zero signifies no motor use; 65,535 signifies 100 percent motor use.
    */
    WORD wLeftMotorSpeed;

    /**
    \brief Speed of the right motor. Valid values are in the range 0 to 65,535.
           Zero signifies no motor use; 65,535 signifies 100 percent motor use.
    */
    WORD wRightMotorSpeed;
} XINPUT_VIBRATION, *PXINPUT_VIBRATION;

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

#ifdef __cplusplus
extern "C" {
#endif

// void XInputEnable(BOOL);
DWORD XInputSetState(DWORD, XINPUT_VIBRATION*);
DWORD XInputGetState(DWORD, XINPUT_STATE*);
// DWORD XInputGetKeystroke(DWORD, DWORD, PXINPUT_KEYSTROKE);
// DWORD XInputGetCapabilities(DWORD, DWORD, XINPUT_CAPABILITIES*);
// DWORD XInputGetDSoundAudioDeviceGuids(DWORD, GUID*, GUID*);
// DWORD XInputGetBatteryInformation(DWORD, BYTE, XINPUT_BATTERY_INFORMATION*);

#ifdef __cplusplus
}
#endif

