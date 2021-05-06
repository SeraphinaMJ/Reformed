/*!***************************************************************************************
\file       Color4.hpp
\author     Seraphina Kim
\date       2/16/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Used to make basic color4
*****************************************************************************************/
#pragma once
//========1st Party Includes============================================================//
#include <Vector4.hpp>
//========Defines=======================================================================//

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: color
\brief   Used to make basic colors
*****************************************************************************************/
class color4 : public vector4
{
	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////
public:


	//==================================================================================//
	//       Default Constructor                                                        //
	//==================================================================================//

	/*!***************************************************************************************
	\brief  Default constructor
	*****************************************************************************************/
	color4() : vector4(0, 0, 0, 1) {};






	//==================================================================================//
	//       Constructor                                                                //
	//==================================================================================//

	/*!***************************************************************************************
	\brief  Peramiterized constructor that builds a color from rgba values
	\param  p_r - red
	\param  p_g - green
	\param  p_b - blue
	\param  p_a - alpha
	*****************************************************************************************/
	color4(float p_r, float p_g, float p_b, float p_a = 1) : vector4(p_r, p_g, p_b, p_a) {}


};

