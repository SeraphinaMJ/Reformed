/*!***************************************************************************************
\file       InputComponent.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the InputComponent class member functions.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "InputComponent.hpp"
//======== 1st Party Includes ==========================================================//
#include "InputFramework.hpp"
//======== 3rd Party Includes ==========================================================//
#include <algorithm>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
inputComponent::inputComponent(typeRT& p_type_data)
{
	updateFromTypeRT(p_type_data);
}

inputComponent::~inputComponent()
{
	clearAllMaps();
}

//======== Getters & Setters ===========================================================//
typeRT inputComponent::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());

	typeRT contextRT = typeRT("context", context);
	result.insertMember(contextRT);

	std::vector<typeRT> l_map;
	for(auto l_iter : nameToLetterMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("nameToLetterMapEntry");
		l_entry.insertMember(typeRT("name", l_iter.first));
		l_entry.insertMember(typeRT("letter", l_iter.second));
		l_map.push_back(l_entry);
	}
	typeRT nameToLetterMapRT = typeRT("nameToLetterMap", l_map);
	result.insertMember(nameToLetterMapRT);

	l_map.clear();
	for(auto l_iter : nameToAxisMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("nameToAxisMapEntry");
		l_entry.insertMember(typeRT("name", l_iter.first));
		l_entry.insertMember(typeRT("axis", l_iter.second));
		l_map.push_back(l_entry);
	}
	typeRT nameToAxisMapRT = typeRT("nameToAxisMap", l_map);
	result.insertMember(nameToAxisMapRT);

	l_map.clear();
	for(auto l_iter : nameToCursorMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("nameToCursorMapEntry");
		l_entry.insertMember(typeRT("name", l_iter.first));
		l_entry.insertMember(typeRT("cursor", static_cast<unsigned int>(l_iter.second)));
		l_map.push_back(l_entry);
	}
	typeRT nameToCursorMapRT = typeRT("nameToCursorMap", l_map);
	result.insertMember(nameToCursorMapRT);

	l_map.clear();
	for(auto l_iter : nameToGamepadButtonMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("nameToGamepadButtonMapEntry");
		l_entry.insertMember(typeRT("name", l_iter.first));
		l_entry.insertMember(typeRT("button", std::get<0>(l_iter.second)));
		l_entry.insertMember(typeRT("controllerID", std::get<1>(l_iter.second)));
		l_map.push_back(l_entry);
	}
	typeRT nameToGamepadButtonMapRT = typeRT("nameToGamepadButtonMap", l_map);
	result.insertMember(nameToGamepadButtonMapRT);

	l_map.clear();
	for(auto l_iter: nameToMouseButtonMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("nameToMouseButtonMapEntry");
		l_entry.insertMember(typeRT("name", l_iter.first));
		l_entry.insertMember(typeRT("mouseButton", static_cast<unsigned int>(l_iter.second)));
		l_map.push_back(l_entry);
	}
	typeRT nameToMouseButtonMapRT = typeRT("nameToMouseButtonMap", l_map);
	result.insertMember(nameToMouseButtonMapRT);

	l_map.clear();
	for(auto l_iter : nameToGamepadAxisMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("nameToGamepadAxisMapEntry");
		l_entry.insertMember(typeRT("name", l_iter.first));
		l_entry.insertMember(typeRT("gamepadAxis", std::get<0>(l_iter.second)));
		l_entry.insertMember(typeRT("controllerID", std::get<1>(l_iter.second)));
		l_map.push_back(l_entry);
	}
	typeRT nameToGamepadAxisMapRT = typeRT("nameToGamepadAxisMap", l_map);
	result.insertMember(nameToGamepadAxisMapRT);

	l_map.clear();
	for(auto l_iter : nameToControllerMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("nameToControllerEntry");
		l_entry.insertMember(typeRT("name", l_iter.first));
		l_entry.insertMember(typeRT("controllerID", l_iter.second));
		l_map.push_back(l_entry);
	}
	typeRT nameToControllerMapRT = typeRT("nameToControllerMap", l_map);
	result.insertMember(nameToControllerMapRT);

	return result;
}

void inputComponent::updateFromTypeRT(typeRT& p_type)
{
	clearAllMaps();
	auto l_members = p_type.members();

	if(l_members.find("context") != l_members.end())
	{
		context = p_type.member("context").getString();
	}
	if (l_members.find("nameToLetterMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("nameToLetterMap").array())
		{
			name l_name = l_entry.member("name").getString();
			letter l_letter = l_entry.member("letter").getUInt();
			addButtonMapping(l_name, l_letter);
		}
	}
	if(l_members.find("nameToAxisMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("nameToAxisMap").array())
		{
			name l_name = l_entry.member("name").getString();
			axis l_axis = l_entry.member("axis").getInt();
			addAxisMapping(l_name, l_axis);
		}
	}
	if(l_members.find("nameToCursorMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("nameToCursorMap").array())
		{
			Cursor l_cursor = static_cast<Cursor>(l_entry.member("cursor").getUInt());
			name l_name = l_entry.member("name").getString();
			addCursorMapping(l_name, l_cursor);
		}
	}
	if(l_members.find("nameToGamepadButtonMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("nameToGamepadButtonMap").array())
		{
			name l_name = l_entry.member("name").getString();
			input_button l_button = l_entry.member("button").getUInt();
			gamepad_id l_controllerid = l_entry.member("controllerID").getUInt();
			addGamepadButtonMapping(l_name, l_button, l_controllerid);
		}
	}
	if(l_members.find("nameToMouseButtonMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("nameToMouseButtonMap").array())
		{
			name l_name = l_entry.member("name").getString();
			MouseButton l_button = static_cast<MouseButton>(l_entry.member("mouseButton").getUInt());
			addButtonMapping(l_name, l_button);
		}
	}
	if(l_members.find("nameToGamepadAxisMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("nameToGamepadAxisMap").array())
		{
			name l_name = l_entry.member("name").getString();
			axis l_gamepadAxis = l_entry.member("gamepadAxis").getUInt();
			gamepad_id l_controllerID = l_entry.member("controllerID").getUInt();
			addGamepadAxisMapping(l_name, l_gamepadAxis, l_controllerID);
		}
	}
	if(l_members.find("nameToControllerMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("nameToControllerMap").array())
		{
			name l_name = l_entry.member("name").getString();
			gamepad_id l_controller = l_entry.member("controllerID").getUInt();
			addControllerStateMapping(l_name, l_controller);
		}
	}
}


//======== Overrides ===================================================================//

//======== Functionality ===============================================================//
void inputComponent::addButtonMapping(name const & p_name, letter p_letter)
{
	// make sure letter is uppercase
	if (p_letter >= 'a' && p_letter <= 'z')
		p_letter = p_letter - 'a' + 'A';

	nameToLetterMap[p_name] = p_letter;
}

void inputComponent::addButtonMapping(name const & p_name, Key p_key)
{
	addButtonMapping(p_name, static_cast<unsigned int>(p_key));
}

void inputComponent::addButtonMapping(name const & p_name, MouseButton p_button)
{
	nameToMouseButtonMap[p_name] = p_button;
}

void inputComponent::removeButtonMapping(std::string p_name)
{
	auto l_entry = nameToLetterMap.find(p_name);
	if(l_entry != nameToLetterMap.end())
	{
		nameToLetterMap.erase(l_entry);
	}
	auto l_entry2 = nameToMouseButtonMap.find(p_name);
	if(l_entry2 != nameToMouseButtonMap.end())
	{
		nameToMouseButtonMap.erase(l_entry2);
	}
}

void inputComponent::addAxisMapping(name const & p_name, axis p_axiscode)
{
	nameToAxisMap[p_name] = p_axiscode;
}

void inputComponent::removeAxisMapping(name const & p_name)
{
	auto l_entry = nameToAxisMap.find(p_name);
	if(l_entry != nameToAxisMap.end())
	{
		nameToAxisMap.erase(l_entry);
	}
}

void inputComponent::addCursorMapping(name const & p_name, Cursor p_cursorcode)
{
	nameToCursorMap[p_name] = p_cursorcode;
}

void inputComponent::removeCursorMapping(name const & p_name)
{
	auto l_entry = nameToCursorMap.find(p_name);
	if(l_entry != nameToCursorMap.end())
	{
		nameToCursorMap.erase(l_entry);
	}
}

void inputComponent::addGamepadButtonMapping(name const & p_name, input_button p_button, gamepad_id p_controllerid)
{
	nameToGamepadButtonMap[p_name] = std::make_pair(p_button, p_controllerid);
}

void inputComponent::removeGamepadButtonMapping(const std::string& p_name)
{
	auto l_entry = nameToGamepadButtonMap.find(p_name);
	if(l_entry != nameToGamepadButtonMap.end())
	{
		nameToGamepadButtonMap.erase(l_entry);
	}
}

void inputComponent::addGamepadAxisMapping(name const & p_name, axis p_axis, gamepad_id p_controllerid)
{
	nameToGamepadAxisMap[p_name] = std::make_pair(p_axis, p_controllerid);
}

void inputComponent::removeGamepadAxisMapping(name const & p_name)
{
	auto l_entry = nameToGamepadAxisMap.find(p_name);
	if(l_entry != nameToGamepadAxisMap.end())
	{
		nameToGamepadAxisMap.erase(l_entry);
	}
}

void inputComponent::addControllerStateMapping(name const & p_name, gamepad_id p_id)
{
	nameToControllerMap[p_name] = p_id;
}

void inputComponent::removeControllerStateMapping(name const & p_name)
{
	auto l_entry = nameToControllerMap.find(p_name);
	if(l_entry != nameToControllerMap.end())
	{
		nameToControllerMap.erase(l_entry);
	}
}

void inputComponent::setContext(std::string p_context)
{
    context = p_context;
}

std::string inputComponent::getContext() const
{
    return context;
}

void inputComponent::clearAllMaps()
{
	nameToLetterMap.clear();
	nameToMouseButtonMap.clear();
	nameToGamepadButtonMap.clear();
	nameToAxisMap.clear();
	nameToGamepadAxisMap.clear();
	nameToCursorMap.clear();
	nameToControllerMap.clear();
}

void inputComponent::setMouseCapture(bool p_enable)
{
	if(p_enable)
	{
		inputFramework::enableMouseCapture();
	}
	else
	{
		inputFramework::disableMouseCapture();
	}
}

//======== Helper Functions ============================================================//
bool inputComponent::getButtonState(name const& p_name)
{
	auto letter_entry = nameToLetterMap.find(p_name);
	bool letter_value = false;
	if(letter_entry != nameToLetterMap.end())
	{
		letter_value = inputFramework::getKey(letter_entry->second);
	}
	auto mouse_entry = nameToMouseButtonMap.find(p_name);
	bool mouse_value = false;
	if(mouse_entry != nameToMouseButtonMap.end())
	{
		actionType val = inputFramework::getMouseButton(mouse_entry->second);
		mouse_value = (val == actionType::Pressed || val == actionType::Repeat);
	}
	auto gamepad_entry = nameToGamepadButtonMap.find(p_name);
	bool gamepad_value = false;
	if(gamepad_entry != nameToGamepadButtonMap.end())
	{
		actionType val = inputFramework::getGamepadButton(std::get<0>(gamepad_entry->second), std::get<1>(gamepad_entry->second));
		gamepad_value = (val == actionType::Pressed || val == actionType::Repeat);
	}

	return letter_value || mouse_value || gamepad_value;
}

float inputComponent::getAxisState(name const & p_name)
{
	auto axis_entry = nameToAxisMap.find(p_name);
	float axis_value = 0;
	if(axis_entry != nameToAxisMap.end())
	{
		axis_value = inputFramework::getAxis(axis_entry->second);
	}
	auto gamepad_entry = nameToGamepadAxisMap.find(p_name);
	float gamepad_value = 0;
	if(gamepad_entry != nameToGamepadAxisMap.end())
	{
		gamepad_value = inputFramework::getGamepadAxis(
		                     std::get<0>(gamepad_entry->second), 
		                     std::get<1>(gamepad_entry->second));
	}
	return std::max<float>(axis_value, gamepad_value);
}

bool inputComponent::getConnectionState(name const& p_name)
{
	auto l_entry = nameToControllerMap.find(p_name);
	bool l_value = false;
	if(l_entry != nameToControllerMap.end())
	{
		actionType l_action = inputFramework::getControllerState(l_entry->second);
		l_value = l_action == actionType::Repeat
		       || l_action == actionType::Pressed;
	}
	return l_value;
}

cursorPosition inputComponent::getCursorState(name const & p_name)
{
	auto cursor_entry = nameToCursorMap.find(p_name);
	cursorPosition cursor_value = std::make_pair(0.0, 0.0);
	if (cursor_entry != nameToCursorMap.end())
	{
		cursor_value = inputFramework::getMousePosition();
		// cursor_value = inputFramework::getCursorPosition(cursor_entry->second);
	}
	return cursor_value;
}



