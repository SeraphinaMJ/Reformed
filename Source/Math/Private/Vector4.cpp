/*!***************************************************************************************
\file       Vector4.cpp
\author     Cody Cannell
\date       5/22/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      This is the definition of a simple Vector class of four values
*****************************************************************************************/

//========Self Include==================================================================//
#include "Vector4.hpp"
//========1st Party Includes============================================================//
#include "Vector3.hpp"
#include "Quaternion.hpp"
//========3rd Party Includes============================================================//
#include <sstream>
#include <functional>
#ifndef _WIN32
#include <cmath>
#endif

//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

// required for vector.inl
template<typename BinOp>
static vector4 concat(vector4 const& lhs, vector4 const& rhs, BinOp op) noexcept
{
	return vector4{
		op(lhs.x, rhs.x),
		op(lhs.y, rhs.y),
		op(lhs.z, rhs.z),
		op(lhs.w, rhs.w)
	};
}

template<typename BinOp>
static vector4 concat(vector4 const& lhs, const float rhs, BinOp op) noexcept
{
	return vector4{
		op(lhs.x, rhs),
		op(lhs.y, rhs),
		op(lhs.z, rhs),
		op(lhs.w, rhs)
	};
}

	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

vector4::vector4(const float p_scalar) noexcept
	: x{ p_scalar }
	, y{ p_scalar }
	, z{ p_scalar }
	, w{ p_scalar }
{
}

//==============================================================================//
//        Constructor                                                           //
//==============================================================================//
vector4::vector4(const float p_x, const float p_y, const float p_z, const float p_w) noexcept : x(p_x), y(p_y), z(p_z), w(p_w)
{}

vector4::vector4(quaternion const & p_other) noexcept
	: x{p_other.x}, y{p_other.y}, z{p_other.z}, w{p_other.w}
{
}

vector4::vector4(vector3 const& p_vec3, float p_w) noexcept 
	: x(p_vec3.x), y(p_vec3.y), z(p_vec3.z), w(p_w)
{}

vector4::vector4(const float p_other[4])
{
	x = p_other[0];
	y = p_other[1];
	z = p_other[2];
	w = p_other[3];
}

//==============================================================================//
//        Destructor                                                            //
//==============================================================================//

//==============================================================================//
//        Operators                                                             //
//==============================================================================//

using vectorN = vector4;
// common vector operations
#include "Vector.inl"

float& vector4::operator[](const unsigned & p_index)
{
	return toFloatPtr()[p_index];
}

float vector4::operator[](const unsigned & p_index) const
{
	return toFloatPtr()[p_index];
}

bool vector4::operator==(const vector4& p_other) const
{
	return x == p_other.x && y == p_other.y && z == p_other.z && w == p_other.w;
}

bool vector4::operator!=(const vector4& p_other) const
{
	return x != p_other.x || y != p_other.y || z != p_other.z || w != p_other.w;
}

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////

float vector4::dotP(const vector4& p_rhs) const
{
	return x * p_rhs.x + y * p_rhs.y + z * p_rhs.z + w * p_rhs.w;
}

vector4 vector4::crossP(const vector4& p_rhs) const
{
	return vector4(y * p_rhs.z - z * p_rhs.y, z * p_rhs.x - x * p_rhs.z, x * p_rhs.y - y * p_rhs.x, 1);
}

std::string vector4::toStringPtr() const
{
	std::stringstream l_ss;
	l_ss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return l_ss.str();
}

vector3 vector4::toVector3() const noexcept { return vector3(*this); }

