/*!***************************************************************************************
\file       Vector3.hpp
\author     Cody Cannell
\date       5/17/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definition of the Vector3 class.
*****************************************************************************************/
#pragma once

#include <string>
/////////========================================================================/////////
/////////  Class                                                                 /////////
/////////========================================================================/////////
#pragma pack(push, 1)
struct quaternion;
struct vector4;
struct vector2;

/*!*************************************************************************************
\par class: Vector3
\brief   Class for managing 3 float values
***************************************************************************************/
struct vector3
{


	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
	friend class matrix4x4;  //!< for life

	//////======================================================================//////
	//////    Static                                                            //////
	//////======================================================================//////
	static const vector3 UnitX;  //!< unit in x direction
	static const vector3 UnitY;  //!< unit in y direction
	static const vector3 UnitZ;  //!< unit in z direction
	static const vector3 Zero;

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


	vector3 rotate(quaternion const& rotation) const;

	void splat(float p_value);

	//======================================================================================//
	//        Constructor                                                                   //
	//======================================================================================//

	/*!***************************************************************************************
	\brief  Constructor
	*****************************************************************************************/
	vector3() noexcept = default;

	/*!***************************************************************************************
	\brief  Constructor built from scalar multiplied by (UnitX + UnitY + UnitZ)
	\param p_scalar scalar to fill vector with
	*****************************************************************************************/
	vector3(float p_scalar) noexcept;

	/*!***************************************************************************************
	\brief  Constructor built from xyz
	\param p_x - The x element
	\param p_y - The y element
	\param p_z - The z element
	*****************************************************************************************/
	vector3(float p_x, float p_y, float p_z) noexcept;

	vector3(vector2 const& p_xy, float p_z) noexcept;

	/*!***************************************************************************************
	\brief  Copy Constructor
	\param  p_other - other vec3
	*****************************************************************************************/
	vector3(const vector3 & p_other) noexcept = default;

	/*!***************************************************************************************
	\brief  conversion constructor
	\param  p_other - other vec4. truncates to xyz from xyzw
	*****************************************************************************************/
	explicit vector3(const vector4 & p_other) noexcept;
	/*!***************************************************************************************
	\brief  default destructor
	*****************************************************************************************/
	~vector3() noexcept = default;


	//======================================================================================//
	//       Operators                                                                      //
	//======================================================================================//

	vector3& operator=(const vector3 & rhs) noexcept = default;

	/*!***************************************************************************************
	\brief  Gets unsigned element from vec3
	\param  p_index - The element you want
	\return ref to the element
	*****************************************************************************************/
	[[nodiscard]] float& operator[](const unsigned & p_index);

	/*!***************************************************************************************
	\brief  Gets unsigned element from vec3
	\param  p_index - The element you want
	\return copy of the element
	*****************************************************************************************/
	[[nodiscard]] float operator[](const unsigned & p_index) const;//TODO (cody): change to return ref

	/*!***************************************************************************************
	\brief  Compares this vector with another for equality
	\param  p_other - The other vector to compare with
	\return bool - true if all properties are equivalent, false otherwise
	*****************************************************************************************/
	[[nodiscard]] bool operator==(const vector3 & p_other) const;
	/*!***************************************************************************************
	\brief  Compares this vector with another for inequality
	\param  p_other - The other vector to compare with
	\return bool - false if all properties are equivalent, true otherwise
	*****************************************************************************************/
	[[nodiscard]] bool operator!=(const vector3 & p_other) const;

	/*!***************************************************************************************
	\brief  Unary Operator
	\return Negated vec3
	*****************************************************************************************/
	[[nodiscard]] vector3 operator-() const;

	/*!***************************************************************************************
	\brief  Adds right-hand vec3 to this vec3
	\param  p_rhs - Right-hand vec3
	*****************************************************************************************/
	vector3& operator+=(const vector3 & p_rhs);

	/*!***************************************************************************************
	\brief  Subtracts right hand vec3 to this vec3
	\param  p_rhs - Right-hand vec3
	*****************************************************************************************/
	vector3& operator-=(const vector3 & p_rhs);

	/*!***************************************************************************************
	\brief  Multiples right-hand vec3 to this vec3
	\param  p_rhs - Right hand vec3
	*****************************************************************************************/
	vector3& operator*=(const vector3 & p_rhs);

	/*!***************************************************************************************
	\brief  Divides right-hand vec3 to this vec3
	\param  p_rhs - Right-hand vec3
	*****************************************************************************************/
	vector3& operator/=(const vector3 & p_rhs);

	/*!***************************************************************************************
	\brief  Adds right-hand float to all elements of this vec3
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	vector3& operator+=(float p_rhs);

	/*!***************************************************************************************
	\brief  Subtracts right-hand float to all elements of this vec3
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	vector3& operator-=(float p_rhs);

	/*!***************************************************************************************
	\brief  Multiples right-hand float to all elements of this vec3
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	vector3& operator*=(float p_rhs);

	/*!***************************************************************************************
	\brief  Divides right-hand float to all elements of this vec3
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	vector3& operator/=(float p_rhs);

	/*!***************************************************************************************
	\brief  Makes temp copy of vec3 and += right-hand vec3 to this vec3
	\param  p_rhs - Right-hand vec3
	\return copy of temp vec3
	*****************************************************************************************/
	[[nodiscard]] vector3 operator+(const vector3 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec3 and -= right-hand vec3 to this vec3
	\param  p_rhs - Right-hand vec3
	\return copy of temp vec3
	*****************************************************************************************/
	[[nodiscard]] vector3 operator-(const vector3 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec3 and *= right-hand vec3 to this vec3
	\param  p_rhs - Right-hand vec3
	\return copy of temp vec3
	*****************************************************************************************/
	[[nodiscard]] vector3 operator*(const vector3 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec3 and /= right-hand vec3 to this vec3
	\param  p_rhs - Right-hand vec3
	\return copy of temp vec3
	*****************************************************************************************/
	[[nodiscard]] vector3 operator/(const vector3 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec3 and += right-hand float to this vec3
	\param  p_rhs - Right-hand vec3
	\return copy of temp vec3
	*****************************************************************************************/
	[[nodiscard]] vector3 operator+(float p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec3 and -= right-hand float to this vec3
	\param  p_rhs - Right-hand float
	\return copy of temp vec3
	*****************************************************************************************/
	[[nodiscard]] vector3 operator-(float p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec3 and *= right-hand float to this vec3
	\param  p_rhs - Right-hand float
	\return copy of temp vec3
	*****************************************************************************************/
	[[nodiscard]] vector3 operator*(float p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec3 and /= right-hand float to this vec3
	\param  p_rhs - Right-hand float
	\return copy of temp vec3
	*****************************************************************************************/
	[[nodiscard]] vector3 operator/(float p_rhs) const;

	friend vector3 operator*(float lhs, const vector3 & rhs);
	friend vector3 operator+(float lhs, const vector3 & rhs);

	[[nodiscard]] bool operator==(const vector3& p_rhs);

	/*!***************************************************************************************
	\brief  Gets the dist=ance of the vec3 using distance formula.  WARNING: slow operation
	\return Distance
	*****************************************************************************************/
	[[nodiscard]] float distance() const;

	/*!***************************************************************************************
	\brief  Gets the distance squared of the vec3 using distance formula (faster than distance())
	\return Distance squared
	*****************************************************************************************/
	[[nodiscard]] float distanceSquared() const;

	/*!***************************************************************************************
	\brief  Gets the dot product of two vectors
	\param  p_rhs - Right-hand vec3
	\return dot product
	*****************************************************************************************/
	[[nodiscard]] float dotP(const vector3 p_rhs) const;

	/*!***************************************************************************************
	\brief  Gets the Cross product of two vectors
	\param  p_rhs - Right-hand vec3
	\return dot product
	*****************************************************************************************/
	[[nodiscard]] vector3 crossP(const vector3 p_rhs) const;

	/*!***************************************************************************************
	\brief  normalizes the vec3 by changing the data
	\return normalized vector
	*****************************************************************************************/
	[[nodiscard]] vector3 normalize() const;

	/*!***************************************************************************************
	\brief  normalizes the vec3 by send back a normalized copy
	\return normalized vector
	*****************************************************************************************/
	void normalized();

	friend vector3 floor(vector3 const& vec3);
	friend vector3 ceil(vector3 const& vec3);

	/*!***************************************************************************************
	\brief  Gets a pointer to the start of the data memory
	\return A float pointer to data
	*****************************************************************************************/
	[[nodiscard]] const float * toFloatPtr() const;

	[[nodiscard]] float * toFloatPtr();

	/*!***************************************************************************************
	\brief  Make string for output
	\return Output string
	*****************************************************************************************/
	[[nodiscard]] std::string toStringPtr() const;


	//////============================================================================//////
	//////     Data                                                                   //////
	//////============================================================================//////
	float x = 0, y = 0, z = 0;
	//glm::vec3 m_data;  //!< GLM vec3

};
#pragma pack(pop)

/*!***************************************************************************************
\brief  For google Test pretty printing
\param p_obj - vector to be printed
\param p_os - stream to print to
*****************************************************************************************/
void PrintTo(const vector3& p_obj, std::ostream* p_os);

float distance(const vector3& p_obj1, const vector3& p_obj2);