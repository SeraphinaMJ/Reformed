#pragma once

#include <Color.hpp>
#include <Reflection.hpp>


typeRT toTypeRT(const color& p_color);
    
/*!***************************************************************************************
\brief  Converts type to vector.
\param p_type - type to convert
*****************************************************************************************/
color toColor(typeRT& p_typeRT);

