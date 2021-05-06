/*!***************************************************************************************
\file       Vector3.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definition of the Vector3 class.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Vector3.hpp"

//========1st Party Includes============================================================//
#include "Vector4.hpp"
#include "Vector2.hpp"
#include "Quaternion.hpp"
#include "MathFunctions.hpp"
#include <sstream>

//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//
template<typename BinOp>
static vector3 concat(vector3 const& lhs, vector3 const& rhs, BinOp op) noexcept
{
	return vector3{
		op(lhs.x, rhs.x),
		op(lhs.y, rhs.y),
		op(lhs.z, rhs.z)
	};
}

template<typename BinOp>
static vector3 concat(vector3 const& lhs, const float rhs, BinOp op) noexcept
{
	return vector3{
		op(lhs.x, rhs),
		op(lhs.y, rhs),
		op(lhs.z, rhs)
	};
}
	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

		//==============================================================================//
		//        Constructor                                                           //
		//==============================================================================//

vector3::vector3(const float p_scalar) noexcept
	: x{ p_scalar }, y{ p_scalar }, z{p_scalar}
{
}

vector3::vector3(const float p_x, const float p_y, const float p_z) noexcept : x(p_x), y(p_y), z(p_z)
{ }

vector3::vector3(vector2 const& p_xy, float p_z) noexcept : x{p_xy.x}, y{p_xy.y}, z{p_z}
{
}

vector3::vector3(const vector4 & p_other) noexcept : x(p_other.x), y(p_other.y), z(p_other.z)
{ }

//==============================================================================//
//        Destructor                                                            //
//==============================================================================//

//==============================================================================//
//        Operators                                                             //
//==============================================================================//
using vectorN = vector3;
// common vector operations
#include "Vector.inl"

float& vector3::operator[](const unsigned & p_index)
{
	return toFloatPtr()[p_index];
}

float vector3::operator[](const unsigned & p_index) const
{
	return toFloatPtr()[p_index];
}

bool vector3::operator==(const vector3& p_other) const
{
	return x == p_other.x && y == p_other.y && z == p_other.z;
}

bool vector3::operator!=(const vector3& p_other) const
{
	return x != p_other.x || y != p_other.y || z != p_other.z;
}

bool vector3::operator==(const vector3& p_rhs)
{
	return x == p_rhs.x && y == p_rhs.y && z == p_rhs.z;
}



//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////
float vector3::dotP(const vector3 p_rhs) const
{
	return x * p_rhs.x + y * p_rhs.y + z * p_rhs.z;
}

vector3 vector3::crossP(const vector3 p_rhs) const
{
	return vector3(y * p_rhs.z - z * p_rhs.y, z * p_rhs.x - x * p_rhs.z, x * p_rhs.y - y * p_rhs.x);
}

std::string vector3::toStringPtr() const
{
	std::stringstream l_ss;
	l_ss << "(" << x << ", " << y << ", " << z << ")";
	return l_ss.str();
}

///////========================================================================///////
///////   Private                                                              ///////
///////========================================================================///////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////
const vector3 vector3::UnitX = vector3(1.0f, 0.0f, 0.0f);
const vector3 vector3::UnitY = vector3(0.0f, 1.0f, 0.0f);
const vector3 vector3::UnitZ = vector3(0.0f, 0.0f, 1.0f);
const vector3 vector3::Zero = vector3(0.0f, 0.0f, 0.0f);
////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//==============================================================================//
 //        Helper                                                                //
 //==============================================================================//

vector3 vector3::rotate(quaternion const& rotation) const
{
	return vector3(
		vector4(rotation * (*this) *  rotation.Conjugated())
	);
}

void vector3::splat(const float p_value)
{
	*this = vector3{ p_value };
}

vector3 floor(vector3 const& vec3)
{
	return { std::floor(vec3.x), std::floor(vec3.y), std::floor(vec3.z) };
}

vector3 ceil(vector3 const& vec3)
{
	return { std::ceil(vec3.x), std::ceil(vec3.y), std::ceil(vec3.z) };
}

float distance(const vector3& p_obj1, const vector3& p_obj2)
{
	return sqrt(((p_obj1.x - p_obj2.x) * (p_obj1.x - p_obj2.x)) +
		((p_obj1.y - p_obj2.y) * (p_obj1.y - p_obj2.y)) +
		((p_obj1.z - p_obj2.z) * (p_obj1.z - p_obj2.z)));
}
