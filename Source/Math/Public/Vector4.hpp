/*!***************************************************************************************
\file       Vector4.hpp
\author     Cody Cannell
\date       5/22/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      This is the definition of a simple Vector class of four values
*****************************************************************************************/
#pragma once

#include <string>

#include "Vector3.hpp"

/*!*************************************************************************************
\par class: Vector3
\brief   Class for managing 4 float values
***************************************************************************************/
struct vector4
{

	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	//======================================================================================//
	//        Constructor                                                                   //
	//======================================================================================//

	/*!***************************************************************************************
	\brief  Default constructor, initializes values to 0
	*****************************************************************************************/
	vector4() noexcept = default;


	/*!***************************************************************************************
	\brief  conversion constructor from float as a fill value
	\param p_scalar - the number to fill the vector with
	*****************************************************************************************/
	vector4(float p_scalar) noexcept;

	/*!***************************************************************************************
	\brief  Constructor built from xyzw
	\param p_x - The x element
	\param p_y - The y element
	\param p_z - The z element
	\param p_w - The w element
	*****************************************************************************************/
	vector4(float p_x, float p_y, float p_z, float p_w) noexcept;

	vector4(quaternion const& p_other) noexcept;


	/*!***************************************************************************************
	\brief  Augments a vector3 into a vector4 by adding a w component
	\param p_vec3 - xyz elements
	\param p_w - augmented value
	*****************************************************************************************/
	vector4(vector3 const& p_vec3, float p_w) noexcept;

	vector4(const float p_other[4]);

	vector4& operator=(vector4 const&) noexcept = default;

	/*!***************************************************************************************
	\brief  Copy Constructor
	\param  p_other - other vec4
	*****************************************************************************************/
	vector4(const vector4 & p_other) noexcept = default;

	/*!***************************************************************************************
	\brief  defult destructor
	*****************************************************************************************/
	~vector4() noexcept = default;

	//==============================================================================//
	//        Getters & Setters                                                     //
	//==============================================================================//

	/*!***************************************************************************************
	\brief  gets x component
	\return x component
	*****************************************************************************************/
	[[nodiscard]] float getX() const { return x; }

	/*!***************************************************************************************
   \brief  gets y component
   \return y component
   *****************************************************************************************/
	[[nodiscard]] float getY() const { return y; }

	/*!***************************************************************************************
	\brief  gets z component
	\return z component
	*****************************************************************************************/
	[[nodiscard]] float getZ() const { return z; }

	/*!***************************************************************************************
	\brief  gets w component
	\return w component
	*****************************************************************************************/
	[[nodiscard]] float getW() const { return w; }

	/*!***************************************************************************************
		\brief  sets x component
		\param p_new_x - The new x element to set
		*****************************************************************************************/
	void setX(float p_new_x) { x = p_new_x; }

	/*!***************************************************************************************
	\brief  sets y component
	\param p_new_y - The new y element to set
	*****************************************************************************************/
	void setY(float p_new_y) { y = p_new_y; }

	/*!***************************************************************************************
	\brief  sets z component
	\param p_new_z - The new z element to set
	*****************************************************************************************/
	void setZ(float p_new_z) { z = p_new_z; }

	/*!***************************************************************************************
	\brief  sets w component
	\param p_new_w - The new w element to set
	*****************************************************************************************/
	void setW(float p_new_w) { w = p_new_w; }

	/*!***************************************************************************************
	\brief  sets offset for x component
	\param p_offset - Of set to change x by
	*****************************************************************************************/
	void offsetX(float p_offset) { x += p_offset; }

	/*!***************************************************************************************
	\brief  sets offset for y component
	\param p_offset - Of set to change y by
	*****************************************************************************************/
	void offsetY(float p_offset) { y += p_offset; }

	/*!***************************************************************************************
	\brief  sets offset for z component
	\param p_offset - Of set to change z by
	*****************************************************************************************/
	void offsetZ(float p_offset) { z += p_offset; }

	/*!***************************************************************************************
	\brief  sets offset for z component
	\param p_offset - Of set to change z by
	*****************************************************************************************/
	void offsetW(float p_offset) { w += p_offset; }


	//======================================================================================//
	//       Operators                                                                      //
	//======================================================================================//

	// FOR IMPLEMENTATION see Private/Vector.inl

	/*!***************************************************************************************
	\brief  Gets unsigned element from vec4
	\param  p_index - The element you want
	\return ref to the element
	*****************************************************************************************/
	[[nodiscard]] float& operator[](const unsigned & p_index);

	/*!***************************************************************************************
	\brief  Gets unsigned element from vec4
	\param  p_index - The element you want
	\return copy of the element
	*****************************************************************************************/
	[[nodiscard]] float operator[](const unsigned & p_index) const;

	/*!***************************************************************************************
	\brief  Compares this vector with another for equality
	\param  p_other - The other vector to compare with
	\return bool - true if all properties are equivalent, false otherwise
	*****************************************************************************************/
	[[nodiscard]] bool operator==(const vector4 & p_other) const;

	/*!***************************************************************************************
	\brief  Compares this vector with another for inequality
	\param  p_other - The other vector to compare with
	\return bool - false if all properties are equivalent, true otherwise
	*****************************************************************************************/
	[[nodiscard]] bool operator!=(const vector4 & p_other) const;

	/*!***************************************************************************************
	\brief  Unary Operator
	\return Negated vec4
	*****************************************************************************************/
	[[nodiscard]] vector4 operator-() const;

	/*!***************************************************************************************
	\brief  Adds right-hand vec4 to this vec4
	\param  p_rhs - Right-hand vec4
	*****************************************************************************************/
	vector4& operator+=(const vector4 & p_rhs);

	/*!***************************************************************************************
	\brief  Subtracts right hand vec4 to this vec4
	\param  p_rhs - Right-hand vec4
	*****************************************************************************************/
	vector4& operator-=(const vector4 & p_rhs);

	/*!***************************************************************************************
	\brief  Multiples right-hand vec4 to this vec4
	\param  p_rhs - Right hand vec4
	*****************************************************************************************/
	vector4& operator*=(const vector4 & p_rhs);

	/*!***************************************************************************************
	\brief  Divides right-hand vec4 to this vec4
	\param  p_rhs - Right-hand vec4
	*****************************************************************************************/
	vector4& operator/=(const vector4 & p_rhs);

	/*!***************************************************************************************
	\brief  Adds right-hand float to all elements of this vec4
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	vector4& operator+=(float p_rhs);

	/*!***************************************************************************************
	\brief  Subtracts right-hand float to all elements of this vec4
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	vector4& operator-=(float p_rhs);

	/*!***************************************************************************************
	\brief  Multiples right-hand float to all elements of this vec4
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	vector4& operator*=(float p_rhs);

	/*!***************************************************************************************
	\brief  Divides right-hand float to all elements of this vec4
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	vector4& operator/=(float p_rhs);

	/*!***************************************************************************************
	\brief  Makes temp copy of vec4 and += right-hand vec4 to this vec4
	\param  p_rhs - Right-hand vec4
	\return copy of temp vec4
	*****************************************************************************************/
	[[nodiscard]] vector4 operator+(const vector4 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec4 and -= right-hand vec4 to this vec4
	\param  p_rhs - Right-hand vec4
	\return copy of temp vec4
	*****************************************************************************************/
	[[nodiscard]] vector4 operator-(const vector4 & p_rhs) const;

	/*!***************************************************************************************
   \brief  Makes temp copy of vec4 and *= right-hand vec4 to this vec4
   \param  p_rhs - Right-hand vec4
   \return copy of temp vec4
   *****************************************************************************************/
	[[nodiscard]] vector4 operator*(const vector4 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec4 and /= right-hand vec4 to this vec4
	\param  p_rhs - Right-hand vec4
	\return copy of temp vec4
	*****************************************************************************************/
	[[nodiscard]] vector4 operator/(const vector4 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec4 and += right-hand float to this vec4
	\param  p_rhs - Right-hand vec4
	\return copy of temp vec4
	*****************************************************************************************/
	[[nodiscard]] vector4 operator+(float p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec4 and -= right-hand float to this vec4
	\param  p_rhs - Right-hand float
	\return copy of temp vec4
	*****************************************************************************************/
	[[nodiscard]] vector4 operator-(float p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec4 and *= right-hand float to this vec4
	\param  p_rhs - Right-hand float
	\return copy of temp vec4
	*****************************************************************************************/
	[[nodiscard]] vector4 operator*(float p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec4 and /= right-hand float to this vec4
	\param  p_rhs - Right-hand float
	\return copy of temp vec4
	*****************************************************************************************/
	[[nodiscard]] vector4 operator/(float p_rhs) const;

	/*!***************************************************************************************
		\brief  Gets the dot product of two vectors
		\param  p_rhs - Right-hand vec4
		\return dot product
		*****************************************************************************************/
	[[nodiscard]] float dotP(const vector4& p_rhs) const;
	// external commutative operations
	friend vector4 operator+(float lhs, vector4 const& rhs);
	friend vector4 operator*(float lhs, vector4 const& rhs);

	/*!***************************************************************************************
	\brief  Gets the Cross product of two vectors
	\param  p_rhs - Right-hand vec4
	\return dot product
	*****************************************************************************************/
	[[nodiscard]] vector4 crossP(const vector4& p_rhs) const;

	/*!***************************************************************************************
	\brief  sets the magnitude of the vector to 1
	\return normalized vector
	*****************************************************************************************/
	[[nodiscard]] vector4 normalize() const;

	/*!***************************************************************************************
	\brief  inplace normalization
	*****************************************************************************************/
	void normalized();

	[[nodiscard]] float distance() const;
	[[nodiscard]] float distanceSquared() const;

	/*!***************************************************************************************
	\brief  Gets a pointer to the start of the data memory
	\return A float pointer to data
	*****************************************************************************************/
	[[nodiscard]] float * toFloatPtr();
	[[nodiscard]] const float* toFloatPtr() const;

	/*!***************************************************************************************
	\brief  Make string for output
	\return Output string
	*****************************************************************************************/
	[[nodiscard]] std::string toStringPtr() const;

	[[nodiscard]] vector3 toVector3() const noexcept;

	//////============================================================================//////
	//////     Data                                                                   //////
	//////============================================================================//////

	float x = 0, y = 0, z = 0, w = 0;
};

/*!***************************************************************************************
\brief  For google Test pretty printing
\param p_obj - vector to be printed
\param p_os - stream to print to
*****************************************************************************************/
void PrintTo(const vector4& p_obj, std::ostream* p_os);

float dotP(const vector4 & p_left, const vector4 & p_right);


