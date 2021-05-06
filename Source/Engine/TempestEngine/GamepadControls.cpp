/*!***************************************************************************************
\file       GamepadControls.cpp
\author     Henry Brobeck
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "GamepadControls.hpp"
#include "Components/InputComponent.hpp"


namespace
{
	//names of inputComponent aliases for buttons
	std::string gpb_names[] = { "dpadLeft","dpadUp", "dpadRight",
	"dpadDown","select","start","x","y","a","b","leftStick","rightStick","leftShoulder","rightShoulder" };

	//names of inputComponent aliases for axis
	std::string gpa_names[] = { "leftX","leftY","rightX","rightY","leftTrigger","rightTrigger" };

	//names of inputComponent aliases for axis as buttons
	std::tuple<std::string, std::string> gpa_tuple[] = { {"leftXn","leftXp"},{"leftYn","leftYp"},
	{"rightXn","rightXp"},{"rightYn","rightYp"},{"","leftTp"},{"","rightTp"}};

	//keys for gpb's
	char gpb_keys[] = { 'J','I','L','K', 'B','N','Z','X','C','V','T','Y','Q','E' };
	
	std::tuple<char, char> gpa_keys[] = { {'A','D'},{'S','W'},{'\0','\0'},{'\0','\0'},{'\0','Q'},{'\0','E'} };
}

const std::string& gpaToString(GamepadAxis p_axis)
{
	return gpa_names[p_axis];
}

const std::string& gpbToString(GamepadButton p_button)
{
	return gpb_names[p_button];
}

const std::tuple<std::string,std::string>& gpaToButtonString(GamepadAxis p_axis)
{
	return gpa_tuple[p_axis];
}

char gpbToKey(GamepadButton p_button)
{
	return gpb_keys[p_button];
}

const std::tuple<char, char>& gpaToKeys(GamepadAxis p_axis)
{
	return gpa_keys[p_axis];
}
