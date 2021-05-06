/*!***************************************************************************************
\file       Matrix4x4.hpp
\author     Cody Cannell
\date       5/17/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definition of the Matrix4x4 class. ( column-major order )
*****************************************************************************************/
#pragma once



#include <Quaternion.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>
#include <array>

#if 1
#define ColumnBasis
#endif


/////////========================================================================/////////
/////////  Class                                                                 /////////
/////////========================================================================/////////
/*!*************************************************************************************
\par class: matrix4x4
\brief   This represents a 4x4 matrix
***************************************************************************************/
class matrix4x4
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	//======================================================================================//
	//       Operators                                                                      //
	//======================================================================================//


	/*!***************************************************************************************
	\brief  Retrieves the row from the matrix, by copy
	\param  p_rowIndex - The row in the matrix
	\return vector4 copy of the matrix row
	*****************************************************************************************/
	[[nodiscard]] const vector4& operator[](unsigned p_rowIndex) const;

	[[nodiscard]] vector4& operator[](unsigned p_rowIndex);

	/*!***************************************************************************************
	\brief  Adds right-hand matrix to this matrix
	\param  p_rhs - Right-hand matrix
	*****************************************************************************************/
	//matrix4x4& operator+=(const matrix4x4 & p_rhs);

	/*!***************************************************************************************
	\brief  Subtracts right hand matrix to this matrix
	\param  p_rhs - Right-hand matrix
	*****************************************************************************************/
	//matrix4x4& operator-=(const matrix4x4 & p_rhs);

	/*!***************************************************************************************
	\brief  Multiples right-hand matrix to this matrix
	\param  p_rhs - Right hand float
	*****************************************************************************************/
	matrix4x4& operator*=(const matrix4x4 & p_rhs);

	/*!***************************************************************************************
	\brief  Divides right-hand matrix to this matrix
	\param  p_rhs - Right-hand matrix
	*****************************************************************************************/
	//matrix4x4& operator/=(const matrix4x4 & p_rhs);

	/*!***************************************************************************************
	\brief  Adds right-hand float to all elements of this matrix
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	//matrix4x4& operator+=(const float & p_rhs);

	/*!***************************************************************************************
	\brief  Subtracts right-hand float to all elements of this matrix
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	//matrix4x4& operator-=(const float & p_rhs);

	/*!***************************************************************************************
	\brief  Multiples right-hand float to all elements of this matrix
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	matrix4x4& operator*=(const float & p_rhs);

	/*!***************************************************************************************
	\brief  Divides right-hand float to all elements of this matrix
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	//matrix4x4& operator/=(const float & p_rhs);

	/*!***************************************************************************************
	\brief  Makes temp copy of matrix and += right-hand matrix to this matrix
	\param  p_rhs - Right-hand matrix
	\return copy of temp matrix
	*****************************************************************************************/
	//[[nodiscard]] matrix4x4 operator+(const matrix4x4 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of matrix and -= right-hand matrix to this matrix
	\param  p_rhs - Right-hand matrix
	\return copy of temp matrix
	*****************************************************************************************/
	[[nodiscard]] matrix4x4 operator-(const matrix4x4 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of matrix and *= right-hand matrix to this matrix
	\param  p_rhs - Right-hand matrix
	\return copy of temp matrix
	*****************************************************************************************/
	[[nodiscard]] matrix4x4 operator*(const matrix4x4 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of matrix and /= right-hand matrix to this matrix
	\param  p_rhs - Right-hand matrix
	\return copy of temp matrix
	*****************************************************************************************/
	//[[nodiscard]] matrix4x4 operator/(const matrix4x4 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of matrix and += right-hand float to this matrix
	\param  p_rhs - Right-hand matrix
	\return copy of temp matrix
	*****************************************************************************************/
	//[[nodiscard]] matrix4x4 operator+(const float & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of matrix and -= right-hand float to this matrix
	\param  p_rhs - Right-hand float
	\return copy of temp matrix
	*****************************************************************************************/
	//[[nodiscard]] matrix4x4 operator-(const float & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of matrix and *= right-hand float to this matrix
	\param  p_rhs - Right-hand float
	\return copy of temp matrix
	*****************************************************************************************/
	[[nodiscard]] matrix4x4 operator*(const float & p_rhs) const;
	[[nodiscard]] vector4 operator*(const vector4& p_rhs) const;


	/*!***************************************************************************************
	\brief  Makes temp copy of matrix and /= right-hand float to this matrix
	\param  p_rhs - Right-hand float
	\return copy of temp matrix
	*****************************************************************************************/
	//[[nodiscard]] matrix4x4 operator/(const float & p_rhs) const;

	[[nodiscard]] bool operator==(const matrix4x4 & p_rhs) const;
	[[nodiscard]] bool operator!=(const matrix4x4 & p_rhs) const;

	void decompose(vector3 & scale, quaternion & rotate, vector3 & translate) const;

	void decompose(vector3 & p_position, quaternion & p_rotation, vector3 & p_scale, vector3 & p_shear) const;

	[[nodiscard]] quaternion toQuaternion() const;

	/*!***************************************************************************************
	\brief  Static funtion that build a matrix from position rotation and scale
	\param  p_position - position of the object
	\param  p_rotation - vec 3 for rotation of the object in x y and z directions(radians)
	\param  p_scale - scal of the object
	\return Copy of built matrix
	*****************************************************************************************/
	[[nodiscard]] static matrix4x4 build(const vector3 & p_position, const quaternion & p_rotation, const vector3 & p_scale);

	/*!***************************************************************************************
	\brief  Static funciton that build a translation matrix
	\param  p_position - position of the object
	\return Copy of built translation matrix
	*****************************************************************************************/
	[[nodiscard]] static matrix4x4 translation(const vector3 & p_position);

	/*!***************************************************************************************
	\brief  Static function that builds a rotation matrix
	\param  p_axis  - Axis your rotating around
	\param  p_angle - The angle in which you are rotating
	\return copy of the built rotation matrix
	*****************************************************************************************/
	[[nodiscard]] static matrix4x4 rotationAxis(const vector3 & p_axis, float p_angle);

	/*!***************************************************************************************
	\brief  Static function that builds a projection matrix for camera
	\param  p_fov      - field of veiw
	\param  p_aspect   - aspect ratio
	\param  p_znear    - near plain
	\param  p_zfar     - far plain
	\param  p_leftHand - left hand or right hand winding
	\return Copy of the bult projection matix
	*****************************************************************************************/
	[[nodiscard]] static matrix4x4 computeProjMatrix(float p_fov, float p_aspect, float p_znear, float p_zfar, bool p_leftHand = true);
	//static matrix4x4 rotationAxisX(float angle);
 //   static matrix4x4 rotationAxisY(float angle);
 //   static matrix4x4 rotationAxisZ(float angle);


	/*!***************************************************************************************
	\brief  Static function that builds a orthographic matrix for camera
	\param  p_width    - width of the screen
	\param  p_height   - height of the screen
	\param  p_znear    - near plain
	\param  p_zfar     - far plain
	\param  p_leftHand - left hand or right hand winding
	\return Copy of the biult orthographic matix
	*****************************************************************************************/
	[[nodiscard]] static matrix4x4 computeOrthographicMatrix(float p_left, float p_right, float p_bottom,float p_top, float p_znear, float p_zfar, bool m_leftHand = true);

	[[nodiscard]] static matrix4x4 orthoLH(float p_left, float p_right, float p_bottom, float p_top, float p_znear, float p_zfar);
	[[nodiscard]] static matrix4x4 orthoRH(float p_left, float p_right, float p_bottom, float p_top, float p_znear, float p_zfar);



	[[nodiscard]] static matrix4x4 computeModelMatrixFromLookAtVector(vector3 const& p_position, vector3 const& p_lookatVector);
	void computeModelMatrixFromLookAtVector2(vector3 const& p_position, vector3 const& p_lookatVector);

	void swapYZ();


	/*!***************************************************************************************
	\brief  Static function that builds a scale matrix
	\param  p_scale - vec3 for x,y z scale
	\return Copy of the built scale matrix
	*****************************************************************************************/
	[[nodiscard]] static matrix4x4 scale(const vector3 & p_scale);

	/*!***************************************************************************************
	\brief  Gets a pointer to the data
	\return A float pointer to data
	*****************************************************************************************/
	[[nodiscard]] float * toFloatPtr();
	[[nodiscard]] float const* toFloatPtr() const;

	/*!***************************************************************************************
	\brief  Make string for output
	\return Output string
	*****************************************************************************************/
	[[nodiscard]] std::string toStringPtr() const;

	//======================================================================================//
	//        Constructor                                                                   //
	//======================================================================================//

	/*!***************************************************************************************
	\brief  Constructor
	*****************************************************************************************/
	matrix4x4() noexcept;
	matrix4x4(matrix4x4 const&) noexcept = default;
	matrix4x4& operator=(matrix4x4 const&) noexcept = default;
	~matrix4x4() noexcept = default;

	/*!***************************************************************************************
	\brief  Copy Constructor
	\param  p_other - other mat4
	*****************************************************************************************/
	//matrix4x4(const glm::mat4x4 & p_other);

	matrix4x4(const float p_other[16]) noexcept;


	matrix4x4(const vector3& forward, const vector3& up, const vector3& right);



	[[nodiscard]] vector3 right()    const { return vector3(m_mat4[0][0], m_mat4[1][0], m_mat4[2][0]); }
	[[nodiscard]] vector3 up()       const { return vector3(m_mat4[0][1], m_mat4[1][1], m_mat4[2][1]); }
	[[nodiscard]] vector3 forward()  const { return vector3(m_mat4[0][2], m_mat4[1][2], m_mat4[2][2]); }
	[[nodiscard]] vector3 position() const { return vector3(m_mat4[0][3], m_mat4[1][3], m_mat4[2][3]); }

	void orthonormalize();



	/*!***************************************************************************************
	\brief  Inverse function
	\return Copy of the invere function
	*****************************************************************************************/


	[[nodiscard]] matrix4x4 transpose() const;

	[[nodiscard]] matrix4x4 inverse() const;

	void invert();

	[[nodiscard]] float getDeterminant() const;

	/*!***************************************************************************************
	\brief  Transposes the elements of the matrix
	\return Copy of the transposed matrix
	*****************************************************************************************/
	[[nodiscard]] static matrix4x4 transpose(const matrix4x4 & p_other);

	[[nodiscard]] matrix4x4 concat(const matrix4x4& p_rhs) const;

	//==================================================================================//
	//        Getters & Setters                                                         //
	//==================================================================================//

	/*!***************************************************************************************
	\brief  Gets the Identity Matrix
	\return the identity matrix
	*****************************************************************************************/
	[[nodiscard]] static matrix4x4 GetIdentitiyMatrix();

	std::ostream & operator<<(std::ostream & os) const;

	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////
private:

	void rowAssign(unsigned row, vector4 const& vec);
	void colAssign(unsigned col, vector4 const& vec);
	//////============================================================================//////
	//////     Data                                                                   //////
	//////============================================================================//////
	//glm::mat4x4 data; //!< GLM mat4 uses coluumn major
		//vector4 m_data[4];
#ifdef _WIN32
# pragma warning( push )
# pragma warning( disable : 4201)
#endif
// Your function

	//float matrixA[16] =
	//float matrixB[4][4] = ;

	union
	{
		struct
		{
#ifdef ColumnBasis              //                          R   U   F   T
			float m00, m01, m02, m03, //  ⎡ Rx Ux Fx Tx ⎤  v1 = {  0,  1,  2,  3 }
				m10, m11, m12, m13, //  ⎢ Ry Uy Fy Ty ⎥  v2 = {  4,  5,  6,  7 }
				m20, m21, m22, m23, //  ⎢ Rz Uz Fz Tz ⎥  v3 = {  8,  9, 10, 11 }
				m30, m31, m32, m33; //  ⎣ 0  0  0  1  ⎦  v4 = { 12, 13, 14, 15 }
#else // Row Major
			float m00, m10, m20, m30,
				m01, m11, m21, m31,
				m02, m12, m22, m32,
				m03, m13, m23, m33;
#endif
		};

		std::array<vector4, 4> m_mat4; // { { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 8, 9, 10, 11 }, { 12, 13, 14, 15 } }
		std::array<float, 16> m_array1d; //{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	};

#ifdef _WIN32
# pragma warning( pop )
	//http://en.wikipedia.org/wiki/Row-major_order
#endif


		//  | 00 04 08 12 |
		//  | 01 05 09 13 |
		//  | 02 06 10 14 |
		//  | 03 07 11 15 |

		  //    L(X)  U(Y)  F(Z)   T      <=  direction(axis)
		  //  | V1.x  V2.x  V3.x  V4.x |
		  //  | V1.y  V2.y  V3.y  V4.y |
		  //  | V1.z  V2.z  V3.z  V4.z |
		  //  | V1.w  V2.w  V3.w  V4.w |


		//  [00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15]
};

[[nodiscard]] matrix4x4 operator*(float lhs, const matrix4x4 & rhs);

