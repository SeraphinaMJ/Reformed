/*!***************************************************************************************
\file       Debug.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "Debug.hpp"
#include "../Rendering/TextRenderer.hpp"


void debug::displayText(std::string p_text, std::string p_style, float p_x, float p_y, float p_size, vector4 p_color)
{
	textRenderer::displayText(p_text, p_style, p_x, p_y, p_size, p_color);

}
