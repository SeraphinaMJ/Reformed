/*!***************************************************************************************
\file       GamepadControls.hpp
\author     Henry Brobeck
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

enum GamepadButton
{
	gpb_left,
	gpb_up,
	gpb_right,
	gpb_down,
	gpb_select,
	gpb_start,
	gpb_x,
	gpb_y,
	gpb_a,
	gpb_b,
	gpb_stickLeft,
	gpb_stickRight,
	gpb_shoulderLeft,
	gpb_shoulderRight,
	gpb_last //keep last
};
enum GamepadAxis
{
	gpa_leftX,
	gpa_leftY,
	gpa_rightX,
	gpa_rightY,
	gpa_leftZ, //left trigger
	gpa_rightZ, //right trigger
	gpa_last //keep last
};

const std::string& gpaToString(GamepadAxis p_axis);
const  std::string& gpbToString(GamepadButton p_button);
const std::tuple<std::string, std::string>& gpaToButtonString(GamepadAxis p_axis);
char gpbToKey(GamepadButton p_button);
const std::tuple<char, char>& gpaToKeys(GamepadAxis p_axis);