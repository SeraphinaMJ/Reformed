/*!***************************************************************************************
\file       Matrix4x4.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definition of the Matrix4x4 class. ( column-major order )
*****************************************************************************************/

//========Self Include==================================================================//
//========1st Party Includes============================================================//
#include "Matrix4x4.hpp"
//========3rd Party Includes============================================================//
#include "Quaternion.hpp"
#include "MathFunctions.hpp"
#include "Vector4.hpp"
#include <sstream>
#include <tuple>

#include <cmath>
#include <cstring>

//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Declarations===========================================================//

///////========================================================================///////
///////   Public                                                               ///////
///////========================================================================///////

	//==============================================================================//
	//        Constructor                                                           //
	//==============================================================================//
matrix4x4::matrix4x4() noexcept
{
	m00 = 1;
	m01 = 0;
	m02 = 0;
	m03 = 0;
	m10 = 0;
	m11 = 1;
	m12 = 0;
	m13 = 0;
	m20 = 0;
	m21 = 0;
	m22 = 1;
	m23 = 0;
	m30 = 0;
	m31 = 0;
	m32 = 0;
	m33 = 1;
}



matrix4x4::matrix4x4(const float p_other[16]) noexcept
{
	m00 = p_other[0];
	m01 = p_other[1];
	m02 = p_other[2];
	m03 = p_other[3];
	m10 = p_other[4];
	m11 = p_other[5];
	m12 = p_other[6];
	m13 = p_other[7];
	m20 = p_other[8];
	m21 = p_other[9];
	m22 = p_other[10];
	m23 = p_other[11];
	m30 = p_other[12];
	m31 = p_other[13];
	m32 = p_other[14];
	m33 = p_other[15];
}



matrix4x4::matrix4x4(const vector3& forward, const vector3& up, const vector3& right)
{

	m_mat4[0][0] = right.x;	m_mat4[0][1] = up.x; m_mat4[0][2] = forward.x; m_mat4[0][3] = 0;
	m_mat4[1][0] = right.y;	m_mat4[1][1] = up.y; m_mat4[1][2] = forward.y; m_mat4[1][3] = 0;
	m_mat4[2][0] = right.z;	m_mat4[2][1] = up.z; m_mat4[2][2] = forward.z; m_mat4[2][3] = 0;
	m_mat4[3][0] = 0;		    m_mat4[3][1] = 0;		 m_mat4[3][2] = 0;		     m_mat4[3][3] = 1;
}


//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//
//matrix4x4& matrix4x4::operator+=(const matrix4x4& p_rhs)
//{
//    data += p_rhs.data;
//}

//matrix4x4& matrix4x4::operator-=(const matrix4x4& p_rhs)
//{
//    data -= p_rhs.data;
//}

const vector4& matrix4x4::operator[](const unsigned p_rowIndex) const
{
	return m_mat4[p_rowIndex];
}

vector4& matrix4x4::operator[](const unsigned p_rowIndex)
{
	return m_mat4[p_rowIndex];
}

matrix4x4 matrix4x4::operator*(const matrix4x4& p_rhs) const
{
	matrix4x4 l_temp = *this;
	l_temp *= p_rhs;

	return l_temp;
}

matrix4x4 matrix4x4::concat(const matrix4x4& p_rhs) const
{
	matrix4x4 ret;

	//  r_0 | i_0 i_1 i_2 i_3 |    i_0 | c_0 c_1 c_2 c_3 |
	//  r_1 | i_0 i_1 i_2 i_3 | *  i_1 | c_0 c_1 c_2 c_3 |
	//  r_2 | i_0 i_1 i_2 i_3 |    i_2 | c_0 c_1 c_2 c_3 |
	//  r_3 | i_0 i_1 i_2 i_3 |    i_3 | c_0 c_1 c_2 c_3 |

	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < 4; ++c)
		{
			float accumulator = 0;

			for (int i = 0; i < 4; ++i)
			{
				accumulator += m_mat4[r][i] * p_rhs.m_mat4[i][c];
			}

			ret.m_mat4[r][c] = accumulator;
		}
	}

	return ret;
}

//matrix4x4& matrix4x4::operator/=(const matrix4x4& rhs)
//{
//    data /= rhs.data;
//}

//matrix4x4& matrix4x4::operator+=(const float& p_rhs)
//{
//    data += p_rhs;
//}

//matrix4x4& matrix4x4::operator-=(const float& p_rhs)
//{
//    data -= p_rhs;
//}

matrix4x4& matrix4x4::operator*=(const matrix4x4& p_rhs)
{
	matrix4x4 & l_this = *this;
	return l_this = concat(p_rhs);
}

matrix4x4& matrix4x4::operator*=(const float& p_rhs)
{
	matrix4x4 & l_this = *this;
	l_this[0] *= p_rhs;
	l_this[1] *= p_rhs;
	l_this[2] *= p_rhs;
	l_this[3] *= p_rhs;
	return *this;
}

//matrix4x4& matrix4x4::operator/=(const float& p_rhs)
//{
//    data /= p_rhs;
//}

  //matrix4x4 matrix4x4::operator+(const matrix4x4& p_rhs) const
  //{
  //    matrix4x4 temp = *this;
  //    temp += p_rhs;
  //    return temp;
  //}

  //matrix4x4 matrix4x4::operator-(const matrix4x4& p_rhs) const
  //{
  //    matrix4x4 temp = *this;
  //    temp -= p_rhs;
  //    return temp;
  //}

vector4 matrix4x4::operator*(const vector4& p_rhs) const
{
	return vector4(m_mat4[0][0] * p_rhs[0] + m_mat4[0][1] * p_rhs[1] + m_mat4[0][2] * p_rhs[2] + m_mat4[0][3] * p_rhs[3],
		m_mat4[1][0] * p_rhs[0] + m_mat4[1][1] * p_rhs[1] + m_mat4[1][2] * p_rhs[2] + m_mat4[1][3] * p_rhs[3],
		m_mat4[2][0] * p_rhs[0] + m_mat4[2][1] * p_rhs[1] + m_mat4[2][2] * p_rhs[2] + m_mat4[2][3] * p_rhs[3],
		m_mat4[3][0] * p_rhs[0] + m_mat4[3][1] * p_rhs[1] + m_mat4[3][2] * p_rhs[2] + m_mat4[3][3] * p_rhs[3]);
}

//matrix4x4 matrix4x4::operator/(const matrix4x4& p_rhs) const
//{
//    matrix4x4 temp = *this;
//    temp /= p_rhs;
//    return temp;
//}

//matrix4x4 matrix4x4::operator+(const float& p_rhs) const
//{
//    matrix4x4 temp = *this;
//    temp += p_rhs;
//    return temp;
//}

//matrix4x4 matrix4x4::operator-(const float& p_rhs) const
//{
//    matrix4x4 temp = *this;
//    temp -= p_rhs;
//    return temp;
//}

matrix4x4 matrix4x4::operator*(const float& p_rhs) const
{
	matrix4x4 l_result = *this;

	l_result *= p_rhs;

	return l_result;
}

//matrix4x4 matrix4x4::operator/(const float& p_rhs) const
//{
//    matrix4x4 temp = *this;
//    temp /= p_rhs;
//    return temp;
//}

bool matrix4x4::operator==(const matrix4x4& p_rhs) const
{
	return std::equal(std::begin(m_array1d), std::end(m_array1d),
			          std::begin(p_rhs.m_array1d), epsilonCompare);
#if 0
	return m_mat4[0] == p_rhs.m_mat4[0]
		&& m_mat4[1] == p_rhs.m_mat4[1]
		&& m_mat4[2] == p_rhs.m_mat4[2]
		&& m_mat4[3] == p_rhs.m_mat4[3];
#endif
}

bool matrix4x4::operator!=(const matrix4x4& p_rhs) const
{
	return !(*this == p_rhs);
}

void matrix4x4::decompose(vector3 & translate, quaternion & rotate, vector3 & scale) const
{
	/* vector3 shear;
	decompose(translate, rotate, scale, shear);*/

	matrix4x4 l_mat = *this;

	scale[0] = l_mat.right().distance();
	scale[1] = l_mat.up().distance();
	scale[2] = l_mat.forward().distance();

	l_mat.orthonormalize();



	translate[0] = l_mat.position().x;
	translate[1] = l_mat.position().y;
	translate[2] = l_mat.position().z;



	//vector3 v = vector3(mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1],
	//    mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2],
	//    mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

	//float dot = v.x * mat[0][0] + v.y * mat[0][1] + v.z *  mat[0][2];

	//if (dot < 0.0f)
	//{
	//    mat *= -1.0f;
	//    scale *= -1.0f;
	//}

	rotate = l_mat.toQuaternion().Conjugated();
}

void matrix4x4::decompose(vector3 & p_position, quaternion & p_rotation, vector3 & p_scale, vector3 & p_shear) const
{
	//    L(X)  U(Y)  F(Z)   T      <=  direction(axis)
	//  | V1.x  V2.x  V3.x  V4.x |
	//  | V1.y  V2.y  V3.y  V4.y |
	//  | V1.z  V2.z  V3.z  V4.z |
	//  | V1.w  V2.w  V3.w  V4.w |


	// Sx = ||<V1.x,V1.y,V1.z>||
	// Sy = ||<V2.x,V2.y,V2.z>||
	// Sz = ||<V3.x,V3.y,V3.z>||

			// translation
	p_position = vector3(m_mat4[0][3], m_mat4[1][3], m_mat4[2][3]);



	//  m00, m01, m02, m03,
	//  m10, m11, m12, m13,
	//  m20, m21, m22, m23,
	//  m30, m31, m32, m33;

			 // rotation
	matrix4x4 l_rotation;

	//        X'                                    Y'                              Z'
	l_rotation.m_mat4[0][0] = m_mat4[0][0];  l_rotation.m_mat4[0][1] = m_mat4[0][1];  l_rotation.m_mat4[0][2] = m_mat4[0][2];
	l_rotation.m_mat4[1][0] = m_mat4[1][0];  l_rotation.m_mat4[1][1] = m_mat4[1][1];  l_rotation.m_mat4[1][2] = m_mat4[1][2];
	l_rotation.m_mat4[2][0] = m_mat4[2][0];  l_rotation.m_mat4[2][1] = m_mat4[2][1];  l_rotation.m_mat4[2][2] = m_mat4[2][2];


	// get scale X from the magnitude of X
	p_scale.x = vector3(m_mat4[0][0], m_mat4[1][0], m_mat4[2][0]).distance();

	//normalized X
	l_rotation.m_mat4[0][0] /= p_scale.x;
	l_rotation.m_mat4[1][0] /= p_scale.x;
	l_rotation.m_mat4[2][0] /= p_scale.x;

	//ShearXY is the dot product of X' and Y'
	p_shear.z = l_rotation[0][0] * l_rotation[0][1] +
		l_rotation[1][0] * l_rotation[1][1] +
		l_rotation[2][0] * l_rotation[2][1];


	//Make Y' orthogonal to X' by " Y' = Y' - (ShearXY * X') "
	l_rotation.m_mat4[0][1] -= p_shear.z * l_rotation[0][0];
	l_rotation.m_mat4[1][1] -= p_shear.z * l_rotation[0][1];
	l_rotation.m_mat4[2][1] -= p_shear.z * l_rotation[2][0];



	p_scale.y = vector3(m_mat4[0][1], m_mat4[1][1], m_mat4[2][1]).distance();

	//normalized X
	l_rotation.m_mat4[0][1] /= p_scale.y;
	l_rotation.m_mat4[1][1] /= p_scale.y;
	l_rotation.m_mat4[2][1] /= p_scale.y;

	//ShearXY is divided by ScaleY to get it's final value
	p_shear.z /= p_scale.y;


	//ShearXZ is the dot product of X' and Z'
	p_shear.y = l_rotation[0][0] * l_rotation[0][2] +
		l_rotation[1][0] * l_rotation[1][2] +
		l_rotation[2][0] * l_rotation[2][2];

	//ShearYZ is the dot product of Y' and Z'
	p_shear.x = l_rotation[0][1] * l_rotation[0][2] +
		l_rotation[1][1] * l_rotation[1][2] +
		l_rotation[2][1] * l_rotation[2][2];

	//Make Z' orthogonal to X' by " Z' = Z' - (ShearXZ * X') "
	l_rotation.m_mat4[0][2] -= p_shear.y * l_rotation[0][0];
	l_rotation.m_mat4[1][2] -= p_shear.y * l_rotation[1][0];
	l_rotation.m_mat4[2][2] -= p_shear.y * l_rotation[2][0];

	//Make Z' orthogonal to Y' by " Z' = Z' - (ShearYZ * Y') "
	l_rotation.m_mat4[0][2] -= p_shear.x * l_rotation[0][1];
	l_rotation.m_mat4[1][2] -= p_shear.x * l_rotation[1][1];
	l_rotation.m_mat4[2][2] -= p_shear.x * l_rotation[2][1];


	p_scale.z = vector3(m_mat4[0][2], m_mat4[1][2], m_mat4[2][2]).distance();

	//Z' is normalized
	l_rotation.m_mat4[0][2] /= p_scale.z;
	l_rotation.m_mat4[1][2] /= p_scale.z;
	l_rotation.m_mat4[2][2] /= p_scale.z;

	//ShearXZ is divided by ScaleZ to get it's final value
	p_shear.y /= p_scale.z;

	//ShearYZ is divided by ScaleZ to get it's final value
	p_shear.x /= p_scale.z;

	//If the determinant is negative, then the rotation and scale contain a flip
	vector3 v = vector3(
		l_rotation[1][1] * l_rotation[2][2] - l_rotation[2][1] * l_rotation[1][2],
		l_rotation[2][1] * l_rotation[0][2] - l_rotation[0][1] * l_rotation[2][2],
		l_rotation[0][1] * l_rotation[1][2] - l_rotation[1][1] * l_rotation[0][2]);

	float dot = v.x * l_rotation[0][0] + v.y * l_rotation[1][0] + v.z *  l_rotation[2][0];

	if (dot < 0.0f)
	{
		l_rotation *= -1.0f;
		p_scale *= -1.0f;
	}

	p_rotation = l_rotation.toQuaternion();

}

quaternion matrix4x4::toQuaternion() const
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


	// get the trace
	//  trace = 1-2(Y*Y)-2(Z*Z) + 1-2(X*X)-2(Z*Z) + 1-2(X*X)-2(Y*Y) + 1
	//        = 4 - 4(x*x + y*y + z*z)
	// if rotation matrix  // (w * w + x * x + y * y + z * z = 1)
	// so trace = 4(W*W)
	// W = sqrt(trace);
	// X = (M21 - M12)/4W   Y = (M02 - M20)/ 4W  Z = (M10 - M01)/ 4W
	// if W = 0 then


	quaternion q;

	float trace = m_mat4[0][0] + m_mat4[1][1] + m_mat4[2][2];

	float s;
	int i, j, k;

	if (trace > 0.0f)
	{
		s = sqrt(trace + 1.0f);
		q[3] = s * .5f;
		s = .5f / s;

		q[0] = (m_mat4[1][2] - m_mat4[2][1]) * s;
		q[1] = (m_mat4[2][0] - m_mat4[0][2]) * s;
		q[2] = (m_mat4[0][1] - m_mat4[1][0]) * s;

	}
	else
	{
		i = 0; // X = 0
		if (m_mat4[1][1] > m_mat4[0][0]) i = 1;
		if (m_mat4[2][2] > m_mat4[i][i]) i = 2;
		j = (i + 1) % 3; k = (j + 1) % 3;
		s = 2.0f * sqrt(m_mat4[i][i] - (m_mat4[j][j] + m_mat4[k][k]) + 1.0f);
		q[i] = 0.25f * s;

		q[3] = (m_mat4[j][k] - m_mat4[k][j]) / s;
		q[j] = (m_mat4[i][j] + m_mat4[j][i]) / s;
		q[k] = (m_mat4[i][k] + m_mat4[k][i]) / s;
	}

	return q;
}

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////
matrix4x4 matrix4x4::build(const vector3 &p_position, const quaternion &p_rotation, const vector3 &p_scale)
{

	matrix4x4 l_trans = matrix4x4::translation(p_position);


	/* matrix4x4 l_rotX = matrix4x4::rotationAxis(vector3::UnitX, p_rotation.getX());
	 matrix4x4 l_rotY = matrix4x4::rotationAxis(vector3::UnitY, p_rotation.getY());
	 matrix4x4 l_rotZ = matrix4x4::rotationAxis(vector3::UnitZ, p_rotation.getZ());*/

	 // convert eular rotation to quaternion
	matrix4x4 l_rot = p_rotation.toMatrix4x4();

	matrix4x4 l_scal = matrix4x4::scale(p_scale);

	matrix4x4 l_ret = l_trans * l_rot *l_scal;
	return l_ret;

}

matrix4x4 matrix4x4::translation(const vector3 & position)
{

	matrix4x4 l_result;

	l_result.m_mat4[0][3] = position.getX();
	l_result.m_mat4[1][3] = position.getY();
	l_result.m_mat4[2][3] = position.getZ();
	l_result.m_mat4[3][3] = 1;

	return l_result;
}

//matrix4x4 matrix4x4::rotationAxisX(float angle)
//{
//    matrix4x4 temp;
//
//    temp[1][1] = glm::cos(angle);
//    temp[1][2] = -glm::sin(angle);
//    temp[2][1] = glm::sin(angle);
//    temp[2][2] = glm::cos(angle);
//
//    return temp;
//}
//
//matrix4x4 matrix4x4::rotationAxisY(float angle)
//{
//    matrix4x4 temp;
//
//    temp[0][0] = glm::cos(angle);
//    temp[0][2] = glm::sin(angle);
//    temp[2][0] = -glm::sin(angle);
//    temp[2][2] = glm::cos(angle);
//
//    return temp;
//}
//
//matrix4x4 matrix4x4::rotationAxisZ(float angle)
//{
//    matrix4x4 temp;
//
//    temp[0][0] = glm::cos(angle);
//    temp[0][1] = -glm::sin(angle);
//    temp[1][0] = glm::sin(angle);
//    temp[1][1] = glm::cos(angle);
//
//    return temp;
//}

matrix4x4 matrix4x4::rotationAxis(const vector3 & p_axis, float p_angle)
{
	//https://en.wikipedia.org/wiki/Rotation_matrix
	//| [cos(0)+x^2(1-cos(0))] [xy(1-cos(0))-zsin(0)] [xz(1-cos(0))+ysin(0)] |
	//| [xy(1-cos(0))+zsin(0)] [cos(0)+y^2(1-cos(0))] [yz(1-cos(0))-xsin(0)] |
	//| [xz(1-cos(0))-ysin(0)] [yz(1-cos(0))+xsin(0)] [cos(0)+z^2(1-cos(0))] |

	const vector3 p_axis_norm = p_axis.normalize();

	float l_c0 = std::cos(p_angle);
	float l_ic0 = 1.0f - l_c0;
	float l_s0 = std::sin(p_angle);

	float l_x = p_axis_norm.getX();
	float l_y = p_axis_norm.getY();
	float l_z = p_axis_norm.getZ();

	float l_xsq = l_x * l_x;
	float l_ysq = l_y * l_y;
	float l_zsq = l_z * l_z;

	matrix4x4 l_result;

	l_result.m_mat4[0][0] = l_c0 + (l_xsq * l_ic0);
	l_result.m_mat4[0][1] = (l_x*l_y*l_ic0) - l_z * l_s0;
	l_result.m_mat4[0][2] = (l_x*l_z*l_ic0) + l_y * l_s0;
	l_result.m_mat4[1][0] = (l_x*l_y*l_ic0) + l_z * l_s0;
	l_result.m_mat4[1][1] = l_c0 + (l_ysq * l_ic0);
	l_result.m_mat4[1][2] = (l_y*l_z*l_ic0) - l_x * l_s0;
	l_result.m_mat4[2][0] = (l_x*l_z*l_ic0) - l_y * l_s0;
	l_result.m_mat4[2][1] = (l_y*l_z*l_ic0) + l_x * l_s0;
	l_result.m_mat4[2][2] = l_c0 + (l_zsq * l_ic0);

	return l_result;
}

matrix4x4 matrix4x4::computeProjMatrix(float p_fov, float p_aspect, float p_znear, float p_zfar, bool)
{
	matrix4x4 l_mat;

	l_mat.m_mat4[0][0] = /*(2.0f * D)/ width;*/ /*(leftHand ? 1 : -1) **/   1 / std::tan(p_fov / 2.0f);
	l_mat.m_mat4[1][1] = /*(2.0f * D) / height;*/ p_aspect / std::tan(p_fov / 2.0f);
	l_mat.m_mat4[2][2] = (p_znear + p_zfar) / (p_znear - p_zfar);
	l_mat.m_mat4[2][3] = (2.0f* p_znear *p_zfar) / (p_znear - p_zfar);
	l_mat.m_mat4[3][2] = -1.00000000f;
	l_mat.m_mat4[3][3] = 0;

	return l_mat;
}

matrix4x4 matrix4x4::scale(const vector3 & p_scale)
{
	matrix4x4 l_result;
	l_result.m_mat4[0][0] = (p_scale.getX()) ? p_scale.getX() : 1;
	l_result.m_mat4[1][1] = (p_scale.getY()) ? p_scale.getY() : 1;
	l_result.m_mat4[2][2] = (p_scale.getZ()) ? p_scale.getZ() : 1;
	l_result.m_mat4[3][3] = 1;

	//glm::scale(result, scale);
	return l_result;
}

void matrix4x4::orthonormalize()
{
	vector3 nforward = forward().normalize();
	vector3 nup = up().normalize();
	vector3 nright = right().normalize();

	m_mat4[0][0] = nright.x;
	m_mat4[0][1] = nup.x;
	m_mat4[0][2] = nforward.x;
	m_mat4[1][0] = nright.y;
	m_mat4[1][1] = nup.y;
	m_mat4[1][2] = nforward.y;
	m_mat4[2][0] = nright.z;
	m_mat4[2][1] = nup.z;
	m_mat4[2][2] = nforward.z;
}

matrix4x4 matrix4x4::inverse() const
{
	matrix4x4 inverted;
	float determinant = getDeterminant();
	//ErrorIf(Math::IsZero(determinant), "Matrix4 - Uninvertible matrix.");
	determinant = 1.0f / determinant;
	inverted.m00 = m12 * m23 * m31 - m13 * m22 * m31;
	inverted.m00 += m13 * m21 * m32 - m11 * m23 * m32;
	inverted.m00 += m11 * m22 * m33 - m12 * m21 * m33;
	inverted.m00 *= determinant;

	inverted.m01 = m03 * m22 * m31 - m02 * m23 * m31;
	inverted.m01 += m01 * m23 * m32 - m03 * m21 * m32;
	inverted.m01 += m02 * m21 * m33 - m01 * m22 * m33;
	inverted.m01 *= determinant;

	inverted.m02 = m02 * m13 * m31 - m03 * m12 * m31;
	inverted.m02 += m03 * m11 * m32 - m01 * m13 * m32;
	inverted.m02 += m01 * m12 * m33 - m02 * m11 * m33;
	inverted.m02 *= determinant;

	inverted.m03 = m03 * m12 * m21 - m02 * m13 * m21;
	inverted.m03 += m01 * m13 * m22 - m03 * m11 * m22;
	inverted.m03 += m02 * m11 * m23 - m01 * m12 * m23;
	inverted.m03 *= determinant;

	inverted.m10 = m13 * m22 * m30 - m12 * m23 * m30;
	inverted.m10 += m10 * m23 * m32 - m13 * m20 * m32;
	inverted.m10 += m12 * m20 * m33 - m10 * m22 * m33;
	inverted.m10 *= determinant;

	inverted.m11 = m02 * m23 * m30 - m03 * m22 * m30;
	inverted.m11 += m03 * m20 * m32 - m00 * m23 * m32;
	inverted.m11 += m00 * m22 * m33 - m02 * m20 * m33;
	inverted.m11 *= determinant;

	inverted.m12 = m03 * m12 * m30 - m02 * m13 * m30;
	inverted.m12 += m00 * m13 * m32 - m03 * m10 * m32;
	inverted.m12 += m02 * m10 * m33 - m00 * m12 * m33;
	inverted.m12 *= determinant;

	inverted.m13 = m02 * m13 * m20 - m03 * m12 * m20;
	inverted.m13 += m03 * m10 * m22 - m00 * m13 * m22;
	inverted.m13 += m00 * m12 * m23 - m02 * m10 * m23;
	inverted.m13 *= determinant;

	inverted.m20 = m11 * m23 * m30 - m13 * m21 * m30;
	inverted.m20 += m13 * m20 * m31 - m10 * m23 * m31;
	inverted.m20 += m10 * m21 * m33 - m11 * m20 * m33;
	inverted.m20 *= determinant;

	inverted.m21 = m03 * m21 * m30 - m01 * m23 * m30;
	inverted.m21 += m00 * m23 * m31 - m03 * m20 * m31;
	inverted.m21 += m01 * m20 * m33 - m00 * m21 * m33;
	inverted.m21 *= determinant;

	inverted.m22 = m01 * m13 * m30 - m03 * m11 * m30;
	inverted.m22 += m03 * m10 * m31 - m00 * m13 * m31;
	inverted.m22 += m00 * m11 * m33 - m01 * m10 * m33;
	inverted.m22 *= determinant;

	inverted.m23 = m03 * m11 * m20 - m01 * m13 * m20;
	inverted.m23 += m00 * m13 * m21 - m03 * m10 * m21;
	inverted.m23 += m01 * m10 * m23 - m00 * m11 * m23;
	inverted.m23 *= determinant;

	inverted.m30 = m12 * m21 * m30 - m11 * m22 * m30;
	inverted.m30 += m10 * m22 * m31 - m12 * m20 * m31;
	inverted.m30 += m11 * m20 * m32 - m10 * m21 * m32;
	inverted.m30 *= determinant;

	inverted.m31 = m01 * m22 * m30 - m02 * m21 * m30;
	inverted.m31 += m02 * m20 * m31 - m00 * m22 * m31;
	inverted.m31 += m00 * m21 * m32 - m01 * m20 * m32;
	inverted.m31 *= determinant;

	inverted.m32 = m02 * m11 * m30 - m01 * m12 * m30;
	inverted.m32 += m00 * m12 * m31 - m02 * m10 * m31;
	inverted.m32 += m01 * m10 * m32 - m00 * m11 * m32;
	inverted.m32 *= determinant;

	inverted.m33 = m01 * m12 * m20 - m02 * m11 * m20;
	inverted.m33 += m02 * m10 * m21 - m00 * m12 * m21;
	inverted.m33 += m00 * m11 * m22 - m01 * m10 * m22;
	inverted.m33 *= determinant;

	return inverted;
}

void matrix4x4::invert()
{
	*this = inverse();
}

float matrix4x4::getDeterminant() const
{

	// https://stackoverflow.com/questions/2937702/i-want-to-find-determinant-of-4x4-matrix-in-c-sharp
	float det = m03 * m12 * m21 * m30 - m02 * m13 * m21 * m30;
	det += m01 * m13 * m22 * m30 - m03 * m11 * m22 * m30;
	det += m02 * m11 * m23 * m30 - m01 * m12 * m23 * m30;
	det += m02 * m13 * m20 * m31 - m03 * m12 * m20 * m31;
	det += m03 * m10 * m22 * m31 - m00 * m13 * m22 * m31;
	det += m00 * m12 * m23 * m31 - m02 * m10 * m23 * m31;
	det += m03 * m11 * m20 * m32 - m01 * m13 * m20 * m32;
	det += m00 * m13 * m21 * m32 - m03 * m10 * m21 * m32;
	det += m01 * m10 * m23 * m32 - m00 * m11 * m23 * m32;
	det += m01 * m12 * m20 * m33 - m02 * m11 * m20 * m33;
	det += m02 * m10 * m21 * m33 - m00 * m12 * m21 * m33;
	det += m00 * m11 * m22 * m33 - m01 * m10 * m22 * m33;

	return det;
}

matrix4x4 matrix4x4::transpose() const
{
	return matrix4x4::transpose(*this);
}

matrix4x4 matrix4x4::transpose(const matrix4x4 & p_other)
{
	matrix4x4 l_result;
#if 0
	l_result[0] = vector4(p_other.m_mat4[0][0], p_other.m_mat4[1][0], p_other.m_mat4[2][0], p_other.m_mat4[3][0]);
	l_result[1] = vector4(p_other.m_mat4[0][1], p_other.m_mat4[1][1], p_other.m_mat4[2][1], p_other.m_mat4[3][1]);
	l_result[2] = vector4(p_other.m_mat4[0][2], p_other.m_mat4[1][2], p_other.m_mat4[2][2], p_other.m_mat4[3][2]);
	l_result[3] = vector4(p_other.m_mat4[0][3], p_other.m_mat4[1][3], p_other.m_mat4[2][3], p_other.m_mat4[3][3]);
#else
	for(auto i = 0; i < 4; ++i) {
		for(auto j = 0; j < 4; ++j) {
			l_result.m_mat4[i][j] = p_other.m_mat4[j][i];
		}
	}
#endif
	return l_result;
}

float * matrix4x4::toFloatPtr()
{
	return m_array1d.data();
}

float const * matrix4x4::toFloatPtr() const
{
	return m_array1d.data();
}

std::string matrix4x4::toStringPtr() const
{
	std::stringstream l_ss;

	l_ss << "|" << m_mat4[0][0] << " " << m_mat4[0][1] << " " << m_mat4[0][2] << " " << m_mat4[0][3] << "|\n"
		<< "|" << m_mat4[1][0] << " " << m_mat4[1][1] << " " << m_mat4[1][2] << " " << m_mat4[1][3] << "|\n"
		<< "|" << m_mat4[2][0] << " " << m_mat4[2][1] << " " << m_mat4[2][2] << " " << m_mat4[2][3] << "|\n"
		<< "|" << m_mat4[3][0] << " " << m_mat4[3][1] << " " << m_mat4[3][2] << " " << m_mat4[3][3] << "|";

	return l_ss.str();
}

matrix4x4 matrix4x4::GetIdentitiyMatrix()
{
	return matrix4x4();
}

std::ostream & matrix4x4::operator<<(std::ostream & os) const
{
	os << toStringPtr();
	return os;
}

//matrix4x4 createRotationMatrixFromEularAngles(vector3 p_eular)
//{
//    quaternion l_q = toQuaternion(p_eular);
//
//    return l_q.toMatrix4x4();
//}

matrix4x4 operator*(float lhs, const matrix4x4 & rhs)
{
	return rhs * lhs;
}


matrix4x4 matrix4x4::computeOrthographicMatrix(
	const float p_left,   const float p_right,
	const float p_bottom, const float p_top,
	const float p_znear,  const float p_zfar,
	bool p_leftHand)
{
	return (p_leftHand ? orthoLH : orthoRH)(p_left, p_right, p_bottom, p_top, p_znear, p_zfar);
}

matrix4x4 matrix4x4::orthoLH(
	const float p_left,   const float p_right,
	const float p_bottom, const float p_top,
	const float p_znear,  const float p_zfar)
{
	matrix4x4 l_mat;
	l_mat.m00 = -2.0f / (p_right - p_left);
	l_mat.m30 = 0;//-(p_right + p_left)/ (p_right - p_left);
	l_mat.m11 = 2.0f / (p_top - p_bottom);
	l_mat.m31 = 0;//-(p_top + p_bottom)/(p_top - p_bottom	);
	l_mat.m22 = 2.0f / (p_zfar - p_znear);
	l_mat.m31 = (p_zfar + p_znear) / (p_zfar - p_znear);
	l_mat.m33 = 1.0f;
	return l_mat;
}

matrix4x4 matrix4x4::orthoRH(
	const float p_left,   const float p_right,
	const float p_bottom, const float p_top,
	const float p_znear,  const float p_zfar)
{
	matrix4x4 l_mat;
	l_mat.m00 = -2.0f / (p_right - p_left);
	l_mat.m03 = 0;//-(p_right + p_left)/ (p_right - p_left);
	l_mat.m11 = 2.0f / (p_top - p_bottom);
	l_mat.m13 = 0;//-(p_top + p_bottom)/(p_top - p_bottom	);
	l_mat.m22 = 2.0f / (p_zfar - p_znear);
	l_mat.m13 = (p_zfar + p_znear) / (p_zfar - p_znear);
	l_mat.m33 = 1.0f;
	return l_mat;
}

matrix4x4 matrix4x4::computeModelMatrixFromLookAtVector(vector3 const& p_position, vector3 const& p_point)
{
	matrix4x4 l_m;

	vector3 p_forward = (p_position - p_point).normalize();

	l_m[0][2] = p_forward.x;
	l_m[1][2] = p_forward.y;
	l_m[2][2] = p_forward.z;
	l_m[3][2] = 0;

	// get up vector
	vector3 l_rightVector = vector3(0, 1, 0).crossP(p_forward);

	l_m[0][0] = l_rightVector.x;
	l_m[1][0] = l_rightVector.y;
	l_m[2][0] = l_rightVector.z;
	l_m[3][0] = 0;

	vector3 l_upVector = p_forward.crossP(l_rightVector);

	l_m[0][1] = l_upVector.x;
	l_m[1][1] = l_upVector.y;
	l_m[2][1] = l_upVector.z;
	l_m[3][1] = 0;

	l_m[0][3] = p_position.x;
	l_m[1][3] = p_position.y;
	l_m[2][3] = p_position.z;
	l_m[3][3] = 1;


	return l_m;
}


void matrix4x4::computeModelMatrixFromLookAtVector2(vector3 const& p_position, vector3 const& p_point)
{
	vector3 l_rightVector, l_upVector, l_forward;
	std::tie(l_rightVector, l_upVector, l_forward) =
		[](vector3 const& p_pos, vector3 const& p_point)
		-> std::tuple<vector3, vector3, vector3>
	{
		vector3 l_forward = (p_pos - p_point).normalize();

		float l_dotp = l_forward.dotP(vector3{ 0,1,0 });
		if (epsilonCompare(l_dotp, 1) || epsilonCompare(l_dotp, -1))
		{
			return std::make_tuple(vector3{ 1,0,0 }, vector3{ 0,0,1 }, vector3{ 0,-1,0 });
		}
		else
		{
			vector3 l_rightVector = vector3(0, 1, 0).crossP(l_forward);
			vector3 l_upVector = l_forward.crossP(l_rightVector);
			return std::make_tuple(l_rightVector, l_upVector, l_forward);
		}
	}(p_position, p_point);

	colAssign(0, vector4(l_rightVector, 0));
	colAssign(1, vector4(l_upVector, 0));
	colAssign(2, vector4(l_forward, 0));
	colAssign(3, vector4(p_position, 1));
}

void matrix4x4::swapYZ()
{
	/// before
	// 1 0 0 0
	// 0 1 0 0
	// 0 0 1 0
	// 0 0 0 1
	/// after
	// 1 0 0 0
	// 0 0 1 0
	// 0 1 0 0
	// 0 0 0 1

	for (size_t i = 0; i != 3; ++i)
		std::swap(m_mat4[i][1], m_mat4[i][2]);
}

void matrix4x4::rowAssign(unsigned row, vector4 const & vec)
{
	m_mat4[row] = vec;
}

void matrix4x4::colAssign(unsigned col, vector4 const& vec)
{
	for (size_t i = 0; i != 4; ++i)
		m_mat4[i][col] = vec[i];
}
