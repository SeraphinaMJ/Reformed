/*!***************************************************************************************
\file       Color.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Used to make basic colors
*****************************************************************************************/
#pragma once
//========1st Party Includes============================================================//
#include <Vector4.hpp>

//========Defines=======================================================================//

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////
typedef struct
{
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;


/*!***************************************************************************************
\par class: color
\brief   Used to make basic colors
*****************************************************************************************/
class color : public vector4
{
	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////
public:

	//////==========================================================================//////
	//////    Static                                                                //////
	//////==========================================================================//////
	static color const red;     //!<  R=1.0, G=0.0, B=0.0
	static color const green;   //!<  R=0.0, G=1.0, B=0.0
	static color const blue;    //!<  R=0.0, G=0.0, B=1.0
	static color const yellow;  //!<  R=1.0, G=1.0, B=0.0
	static color const cyan;    //!<  R=0.0, G=1.0, B=1.0
	static color const magenta; //!<  R=1.0, G=0.0, B=1.0
	static color const white;   //!<  R=1.0, G=1.0, B=1.0
	static color const black;   //!<  R=0.0, G=0.0, B=0.0
	static color const gray;    //!<  R=0.5, G=0.5, B=0.5



	//==================================================================================//
	//       Default Constructor                                                        //
	//==================================================================================//

	/*!***************************************************************************************
	\brief  Default constructor
	*****************************************************************************************/
	color() noexcept : vector4(0, 0, 0, 1) {}

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
	color(float p_r, float p_g, float p_b, float p_a = 1) noexcept : vector4(p_r, p_g, p_b, p_a) {}

	color(color const&) noexcept = default;
	color(color &&) noexcept = default;

	color& operator=(color const&) noexcept = default;
	color& operator=(color&&) noexcept = default;


	//((int)(ImSaturate(_VAL) * 255.0f + 0.5f))  
	//{ return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
	/*#define IM_COL32_R_SHIFT    0
	#define IM_COL32_G_SHIFT    8
	#define IM_COL32_B_SHIFT    16
	#define IM_COL32_A_SHIFT    24*/
	/*float color_Saturate(float f)
	{
		return ((f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f);

	}
	float color2Float(float cf_r, float cf_g, float cf_b, float cf_a)
	{

		unsigned out = 0;
			out = ((unsigned)(color_Saturate(cf_r)* 255.0f + 0.5f) << 0);
			out |= ((unsigned)(color_Saturate(cf_g)* 255.0f + 0.5f) << 8);
			out |= ((unsigned)(color_Saturate(cf_b)* 255.0f + 0.5f) << 16);
			out |= ((unsigned)(color_Saturate(cf_a)* 255.0f + 0.5f) << 24);
			return out;
	}*/

};

