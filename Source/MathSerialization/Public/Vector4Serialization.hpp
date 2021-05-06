#pragma once

#include <Vector4.hpp>
#include <Reflection.hpp>

/*!***************************************************************************************
\brief  Converts this into a type
\return Type equivalent.
*****************************************************************************************/
typeRT toTypeRT(const vector4& p_vector4);

/*!***************************************************************************************
\brief  Converts type to vector.
\param p_type - type to convert
*****************************************************************************************/
vector4 toVector4(typeRT& p_type);

