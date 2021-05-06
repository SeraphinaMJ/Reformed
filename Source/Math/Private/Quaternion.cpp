/*!***************************************************************************************
\file       Quaternion.cpp
\author     Cody Cannell
\date       8/21/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the quaternion class. ( column-major order )
*****************************************************************************************/

//======== Self Include ================================================================//
//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <sstream>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//


#include "Quaternion.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4x4.hpp"
#include <math.h>

quaternion::quaternion()
	: x{ 0 }, y{ 0 }, z{ 0 }, w{ 1 }
{
}

quaternion::quaternion(float p_x, float p_y, float p_z, float p_w)
	: x{ p_x }
	, y{ p_y }
	, z{ p_z }
	, w{ p_w }
{
}

quaternion::quaternion(const vector3 axis, const float angle)
{
	const vector3 nAxis = axis.normalize();
	const float l_sin = std::sin(angle * 0.5f);
	const float l_cos = std::cos(angle * 0.5f);
	x = nAxis.x * l_sin;
	y = nAxis.y * l_sin;
	z = nAxis.z * l_sin;
	w = l_cos;
}

matrix4x4 quaternion::toMatrix4x4() const
{

	//  | 00 04 08 12 |
	//  | 01 05 09 13 |
	//  | 02 06 10 14 |
	//  | 03 07 11 15 |

	  //    column major
		// | 1-2(Y*Y)-2(Z*Z)        2XY+2WZ          2XZ-2WY        0 | 
		// |     2XY-2WZ        1-2(X*X)-2(Z*Z)      2YZ+2WX        0 |
		// |     2XZ+2WY           2YZ-2WX        1-2(X*X)-2(Y*Y)   0 |
		// |        0                 0                  0          1 | 

	  // (w * w + x * x + y * y + z * z = 1)

	  // n = ||q|| 

		// (W, (X,Y,Z))

   // matrix4x4 mat4;


	  //// if quaternion is not normalized
   // if (w * w + x * x + y * y + z * z == 1)
   //     normalize();

   // float xx = x * x;
   // float xy = x * y;
   // float xz = x * z;
   // float xw = x * w;

   // float yy = y * y;
   // float yz = y * z;
   // float yw = y * w;

   // float zz = z * z;
   // float zw = z * w;

   // mat4[0][0] = 1 - 2 * (yy + zz);
   // mat4[0][1] = 2 * (xy - zw);
   // mat4[0][2] = 2 * (xz + yw);

   // mat4[1][0] = 2 * (xy + zw);
   // mat4[1][1] = 1 - 2 * (xx + zz);
   // mat4[1][2] = 2 * (yz - xw);

   // mat4[2][0] = 2 * (xz - yw);
   // mat4[2][1] = 2 * (yz + xw);
   // mat4[2][2] = 1 - 2 * (xx + yy);

   // mat4[0][3] = mat4[1][3] = mat4[2][3] = mat4[3][0] = mat4[3][1] = mat4[3][2] = 0;
   // mat4[3][3] = 1;


	return matrix4x4(getForward(), getUp(), getRight());
}


std::string quaternion::toStringPtr() const
{
	std::stringstream l_ss;
	l_ss << "(" << w << ", " << x << ", " << y << ", " << z << ")";
	return l_ss.str();
}

void quaternion::normalize()
{
	float n = sqrt(x * x + y * y + z * z + w * w);
	x /= n;
	y /= n;
	z /= n;
	w /= n;
}

float& quaternion::operator[](unsigned p_index)
{
	return toVector4()[p_index];
}

float quaternion::operator[](unsigned p_index) const
{
	return toVector4()[p_index];
}

vector4& quaternion::toVector4()
{
	return *(vector4*)this;
}

const vector4& quaternion::toVector4() const
{
	return *(vector4*)this;
}



void quaternion::operator+=(const quaternion & rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
}

quaternion quaternion::operator+(const quaternion& rhs) const
{
	return quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

quaternion quaternion::operator-(const quaternion& rhs) const
{
	return quaternion(*this * rhs.Conjugated());
}

quaternion quaternion::operator*(const float rhs) const
{
	return quaternion(x * rhs, y * rhs, z * rhs, w * rhs);
}

quaternion quaternion::operator*(const vector3 rhs) const
{
	return quaternion(w * rhs.x + y * rhs.z - z * rhs.y,
		w * rhs.y + z * rhs.x - x * rhs.z,
		w * rhs.z + x * rhs.y - y * rhs.x,
		-x * rhs.x - y * rhs.y - z * rhs.z);
}

quaternion quaternion::operator*(quaternion quat) const
{
	return quaternion(w * quat.x + x * quat.w + y * quat.z - z * quat.y,
		w * quat.y + y * quat.w + z * quat.x - x * quat.z,
		w * quat.z + z * quat.w + x * quat.y - y * quat.x,
		w * quat.w - x * quat.x - y * quat.y - z * quat.z);
}

void quaternion::operator*=(quaternion rhs)
{
	x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
	y = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
	z = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;
	w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
}

void quaternion::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
}

bool quaternion::operator==(const quaternion& p_other) const
{
	return x == p_other.x && y == p_other.y && z == p_other.z && w == p_other.w;
}

void quaternion::Conjugate()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}

quaternion quaternion::Conjugated() const
{
	return quaternion(-x, -y, -z, w);
}