#pragma once

#include <Vector2.hpp>
#include <Reflection.hpp>

/*!***************************************************************************************
\brief  Converts this into a type
\return Type equivalent.
*****************************************************************************************/
typeRT toTypeRT(const vector2& p_vector2);

/*!***************************************************************************************
\brief  Converts type to vector.
\param p_type - type to convert
*****************************************************************************************/
vector2 toVector2(typeRT& p_type);
