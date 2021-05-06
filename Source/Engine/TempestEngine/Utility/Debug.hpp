/*!***************************************************************************************
\file       GraphicsSystem.hpp
\author     Cody Cannell
\date       3/1/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief			The front end for all debugging calls. Every call is self contained in its
						own sub class.  This is just a adapter class. no logic allowed
*****************************************************************************************/
#pragma once

#include <Color.hpp>
struct vector4;

/*!***************************************************************************************
\par class: debug
\brief The front end for all debugging calls
*****************************************************************************************/
class debug
{

public:

	/*!*******************************************************************************
	\brief  calls internal function textRenderer::displayText
	\param  p_text  - text to display
	\param  p_style - name of the font file
	\param  p_x     - x coord
	\param  p_y     - y coord
	\param  p_size  - size of the text
	\param  p_color - color of the text
	*********************************************************************************/
	static void displayText(std::string p_text, std::string p_style, float p_x, float p_y, float p_size, vector4 p_color);

};