/*!**********************************************************************************************************
\file       GamepadControls.hpp
\author     Henry Brobeck
\date       11/2/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
************************************************************************************************************/
#pragma once
#define THUMB_MIN -32768
#define THUMB_MAX 32768
#define TRIG_MIN 0
#define TRIG_MAX 255
#define XINPUT_BUTTON_OFFSET_MAX 14
#define MAX_CONTROLLERS 4

enum gamepadButtons
{
	gpb_dpadUp,
	gpb_dpadDown,
	gpb_dpadLeft,
	gpb_dpadRight,
	gpb_start,
	gpb_back,
	gpb_leftThumb,
	gpb_rightThumb,
	gpb_leftShoulder,
	gpb_rightShoulder,
	gpb_A = 12,
	gpb_B = 13,
	gpb_X = 14,
	gpb_Y = 15,
};

const unsigned short gpb_offsets[] = { gpb_dpadUp,
	gpb_dpadDown,
	gpb_dpadLeft,
	gpb_dpadRight,
	gpb_start,
	gpb_back,
	gpb_leftThumb,
	gpb_rightThumb,
	gpb_leftShoulder,
	gpb_rightShoulder,
	gpb_A,
	gpb_B,
	gpb_X,
	gpb_Y };

enum gamepadAxis
{
	gpa_leftTrigger,
	gpa_rightTrigger,
	gpa_thumbLX,
	gpa_thumbLY,
	gpa_thumbRX,
	gpa_thumbRY,
	gpa_count
};

