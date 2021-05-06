/*!***************************************************************************************
\file       Quaternion.hpp
\author     Cody Cannell
\date       8/21/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definition of the quaternion class. ( column-major order )
*****************************************************************************************/
#pragma once

#include "Vector3.hpp"
#include <string>
class matrix4x4;
struct vector4;

struct quaternion
{
    quaternion();
    quaternion(float x, float y, float z, float w);
    quaternion(vector3 axis, float angle);
    matrix4x4 toMatrix4x4() const;
    std::string toStringPtr() const;
    void normalize();

    
    

    float& operator[](unsigned index);
    float operator[](unsigned index) const;

    void operator+=(const quaternion & rhs);
    quaternion operator+(const quaternion & rhs) const;
	quaternion operator-(const quaternion & rhs) const;

    quaternion operator*(quaternion quat) const;
    quaternion operator*(const float rhs) const;
    quaternion operator*(const vector3 rhs) const;

    void operator*=(const	quaternion rhs);
    void operator*=(const float rhs);
    bool operator==(const quaternion& p_other) const;

    vector4& toVector4();
    const vector4& toVector4() const;

  
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

    void setW(float p_new_w) { w = p_new_w; }

    /*vector3 getForward() const { return vector3(0,0,1).rotate(*this); }

    vector3 getUp() const { return vector3(0,1,0).rotate(*this); }

    vector3 getRight() const { return vector3(1,0,0).rotate(*this); }*/

	vector3 getForward() const { return vector3(2 * (x*z + w * y), 2 * (y*z - w * x), 1 - 2 * (x*x + y * y)); }

	vector3 getUp() const { return vector3(2 * (x*y - w * z), 1 - 2 * (x*x + z * z), 2 * (y*z + w * x)); }

	vector3 getRight() const { return vector3(1 - 2 * (y*y + z * z), 2 * (x*y + w * z), 2 * (x*z - w * y)); }


    /*!***************************************************************************************
       \brief  sets x component
       \param p_new_x - The new x element to set
       *****************************************************************************************/
    float getX() const { return x; }

    /*!***************************************************************************************
    \brief  sets y component
    \param p_new_y - The new y element to set
    *****************************************************************************************/
    float getY() const { return y; }

    /*!***************************************************************************************
    \brief  sets z component
    \param p_new_z - The new z element to set
    *****************************************************************************************/
    float getZ() const { return z; }

    float getW() const { return w; }
			
    /*!***************************************************************************************
    \brief  sets offset for x component
    \param p_offset - Of set to change x by
    *****************************************************************************************/
    void offsetX(vector3 axis, float p_offset) { *this *= quaternion(axis, p_offset); }

    /*!***************************************************************************************
    \brief  sets offset for y component
    \param p_offset - Of set to change y by
    *****************************************************************************************/
    void offsetY(vector3 axis, float p_offset) { *this *=  quaternion(axis, p_offset); }

    /*!***************************************************************************************
    \brief  sets offset for z component
    \param p_offset - Of set to change z by
    *****************************************************************************************/
    void offsetZ(vector3 axis, float p_offset) { *this *= quaternion(axis, p_offset); }

    float* toFloatPtr()
    {
        return reinterpret_cast<float*>(this);
    }

	float const* toFloatPtr() const
	{
		return reinterpret_cast<float const*>(this);
	}

    void Conjugate();
    quaternion Conjugated() const;	

    float x = 0, y = 0, z = 0, w = 0;
};


quaternion Lerp(const quaternion & start, const quaternion & end, float tValue);
quaternion Slerp(const quaternion & start, const quaternion & end, float tValue);
