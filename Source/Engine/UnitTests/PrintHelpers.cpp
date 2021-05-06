/*!***************************************************************************************
\file       PrintHelpers.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "PrintHelpers.hpp"

#include <Matrix4x4.hpp>

void glm::PrintTo(const vec2& p_obj, std::ostream* p_os)
{
	*p_os << p_obj.x << ", " << p_obj.y;
}

void glm::PrintTo(const vec3& p_obj, std::ostream* p_os)
{
	*p_os << p_obj.x << ", " << p_obj.y << ", " << p_obj.z;
}

void glm::PrintTo(const vec4& p_obj, std::ostream* p_os)
{
	*p_os << p_obj.x << ", " << p_obj.y << ", " << p_obj.z << ", " << p_obj.w;
}

void PrintTo(const matrix4x4& p_obj, std::ostream* p_os)
{
	*p_os << p_obj.toStringPtr();
}

void PrintTo(const quaternion& p_obj, std::ostream* p_os)
{
	*p_os << p_obj.getX() << ", " << p_obj.getY() << ", " << p_obj.getZ() << ", " << p_obj.getW();
}
