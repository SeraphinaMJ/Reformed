/*!***************************************************************************************
\file       Matrix2x2.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definition of the matrix2x2 class. ( column-major order )
*****************************************************************************************/

//========Self Include==================================================================//
//========1st Party Includes============================================================//
#include "../Math/Matrix2x2.h"
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

    ///////========================================================================///////
    ///////   Public                                                               ///////
    ///////========================================================================///////

        //==============================================================================//
        //        Constructor                                                           //
        //==============================================================================//


matrix2x2::matrix2x2(const matrix2x2 & p_other)
{
    m00 = p_other.m00;
    m01 = p_other.m01;
    m10 = p_other.m10;
    m11 = p_other.m11;
}

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//
void matrix2x2::operator+=(const matrix2x2& p_rhs)
{
    *this += p_rhs;
}

void matrix2x2::operator-=(const matrix2x2& p_rhs)
{
    *this -= p_rhs;
}

void matrix2x2::operator*=(const matrix2x2& p_rhs)
{
    vect[0] *= p_rhs;
}

void matrix2x2::operator/=(const matrix2x2& rhs)
{
    *this /= rhs;
}

void matrix2x2::operator+=(const float& p_rhs)
{
    *this += p_rhs;
}

void matrix2x2::operator-=(const float& p_rhs)
{
    *this -= p_rhs;
}

void matrix2x2::operator*=(const float& p_rhs)
{
    *this *= p_rhs;
}

void matrix2x2::operator/=(const float& p_rhs)
{
    *this /= p_rhs;
}

matrix2x2 matrix2x2::operator+(const matrix2x2& p_rhs) const
{
    matrix2x2 temp = *this;
    temp += p_rhs;
    return temp;
}

matrix2x2 matrix2x2::operator-(const matrix2x2& p_rhs) const
{
    matrix2x2 temp = *this;
    temp -= p_rhs;
    return temp;
}

matrix2x2 matrix2x2::operator*(const matrix2x2& p_rhs) const
{
    matrix2x2 temp = *this;
    temp *= p_rhs;
    return temp;
}

matrix2x2 matrix2x2::operator/(const matrix2x2& p_rhs) const
{
    matrix2x2 temp = *this;
    temp /= p_rhs;
    return temp;
}

matrix2x2 matrix2x2::operator+(const float& p_rhs) const
{
    matrix2x2 temp = *this;
    temp += p_rhs;
    return temp;
}

matrix2x2 matrix2x2::operator-(const float& p_rhs) const
{
    matrix2x2 temp = *this;
    temp -= p_rhs;
    return temp;
}

matrix2x2 matrix2x2::operator*(const float& p_rhs) const
{
    matrix2x2 temp = *this;
    temp *= p_rhs;
    return temp;
}

matrix2x2 matrix2x2::operator/(const float& p_rhs) const
{
    matrix2x2 temp = *this;
    temp /= p_rhs;
    return temp;
}

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////



matrix2x2 matrix2x2::scale(const vector2 & p_scale)
{
    matrix2x2 l_result;
    l_result[0][0] = (p_scale.x) ? p_scale.x : 1;
    l_result[1][1] = (p_scale.y) ? p_scale.y : 1;

    return l_result;
}

float matrix2x2::determinate()
{
    return m00 * m11 - m01 * m10;
}

matrix2x2 matrix2x2::Concat(matrix2x2 & rhs) const
{
    matrix2x2 ret;

    ret.m00 = vect[0].dotP(vector2(rhs.m00, rhs.m10));
    ret.m01 = vect[0].dotP(vector2(rhs.m01, rhs.m11));

    ret.m10 = vect[1].dotP(vector2(rhs.m00, rhs.m10));
    ret.m11 = vect[1].dotP(vector2(rhs.m01, rhs.m11));

    return ret;
}

matrix2x2 matrix2x2::inverse()
{
    matrix2x2 temp;

    temp.m00 = m11;
    temp.m01 = -m10;
    temp.m10 = -m01;
    temp.m11 = m00;
    temp *= 1 /  determinate();

    return temp;
}


const float* matrix2x2::toFloatPtr()
{
    return reinterpret_cast<float*>(this);
}


matrix2x2 matrix2x2::GetIdentitiyMatrix()
{
    return matrix2x2();
}
