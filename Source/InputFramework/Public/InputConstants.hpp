/*!***************************************************************************************
\file       InputConstants.hpp
\author     Aaron Damyen
\date       10/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of constants for the input framework
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <utility>
//======== Types =======================================================================//
using gamepad_id = unsigned int;
using cursorPosition = std::pair<double, double>;
using scancode = unsigned int;
using letter = unsigned int;
using axis = unsigned int;
using input_button = unsigned int;

//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par enum: Key
\brief  Defines all the keys that the input component understands and can monitor.
*****************************************************************************************/
enum class Key
{
	A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	ESC = 256, /* F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,*/
	Grave = '`', Digit_1 = '1', Digit_2, Digit_3, Digit_4, Digit_5, Digit_6, Digit_7, Digit_8, Digit_9, Digit_0 = '0',
	Hyphen = '-', Equal = '=', /*Backspace, */
	Tab = '\t', Open_Bracket = '[', Close_Bracket = ']', Backslash = '\\',
	Semicolon = ';', Apostrophe = '\'', Enter = 257,
	/*Shift_Left,*/ Comma = ',', Period = '.', Slash = '/', /*Shift_Right, */
	Control_Left = 341, /*Alt_Left,*/ Space, /*Alt_Right, Control_Right,
	Insert, Home, Page_Up, Delete, End, Page_Down, */
	Up = 265, Left = 263, Down = 264, Right = 262,
	/*Num_Slash, Num_Asterisk, Num_Minus, Num_Plus, Num_Enter,
	Num_0, Num_1, Num_2, Num_3, Num_4, Num_5, Num_6, Num_7, Num_8, Num_9, Num_Period,*/

};

static constexpr unsigned int MAXIMUM_KEY = 1024;

/*!***************************************************************************************
\par enum: MouseButton
\brief   This is the enumeration for the different mouse buttons available
*****************************************************************************************/
enum class MouseButton
{
	Left = 0,
	Right = 1,
	Middle = 2,
};

/*!***************************************************************************************
\brief  Static constant for the maximum number of mouse button this system can handle
*****************************************************************************************/
static constexpr unsigned int MAXIMUM_MOUSEBUTTON = static_cast<unsigned int>(MouseButton::Middle) + 1;

static constexpr unsigned int MAXIMUM_AXIS = 1024;

/*!***************************************************************************************
\par enum: Cursor
\brief  Defines all the cursors that the input component understands and can monitor.
*****************************************************************************************/
enum class Cursor { Default_Mouse = 0 };

