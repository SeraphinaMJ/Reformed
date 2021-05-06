#pragma once

#include <Vector3.hpp>
#include <Reflection.hpp>


/*!***************************************************************************************
\brief  Converts this into a type
\return Type equivalent.
*****************************************************************************************/
typeRT toTypeRT(const vector3& p_vector3);

/*!***************************************************************************************
\brief  Converts type to vector.
\param p_type - type to convert
*****************************************************************************************/
vector3 toVector3(typeRT& p_type);

