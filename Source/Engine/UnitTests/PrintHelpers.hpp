/*!***************************************************************************************
\file       PrintHelpers.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include <glm/glm.hpp>

#include <ostream>

struct quaternion;
class matrix4x4;

namespace glm {
	/*!***************************************************************************************
	\brief  For google Test pretty printing
	\param p_obj - vector to be printed
	\param p_os - stream to print to
	*****************************************************************************************/
	void PrintTo(const vec2& p_obj, std::ostream* p_os);

	/*!***************************************************************************************
	\brief  For google Test pretty printing
	\param p_obj - vector to be printed
	\param p_os - stream to print to
	*****************************************************************************************/
	void PrintTo(const vec3& p_obj, std::ostream* p_os);

	/*!***************************************************************************************
	\brief  For google Test pretty printing
	\param p_obj - vector to be printed
	\param p_os - stream to print to
	*****************************************************************************************/
	void PrintTo(const vec4& p_obj, std::ostream* p_os);

}

/*!***************************************************************************************
\brief  For google Test pretty printing
\param p_obj - matrix to be printed
\param p_os - stream to print to
*****************************************************************************************/
void PrintTo(const matrix4x4 & p_obj, std::ostream * p_os);

/*!***************************************************************************************
\brief  For google Test pretty printing
\param p_obj - quaternion to be printed
\param p_os - stream to print to
*****************************************************************************************/
void PrintTo(const quaternion & p_obj, std::ostream * p_os);
