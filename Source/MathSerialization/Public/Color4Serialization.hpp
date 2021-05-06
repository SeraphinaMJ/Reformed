#pragma once

#include <Color4.hpp>
#include <Reflection.hpp>

/*!***************************************************************************************
\brief  Converts this into a type
\return Type equivalent.
*****************************************************************************************/
typeRT toTypeRT(const color4& p_color4);

/*!***************************************************************************************
\brief  Converts type to vector.
\param p_type - type to convert
*****************************************************************************************/
color4 toColor4(typeRT& p_type);
