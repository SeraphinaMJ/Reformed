/*!***************************************************************************************
\file       Matrix2x2.h
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
struct vector3;
#include "../Math/Vector2.hpp"

struct matrix2x2
{

public:
    //======================================================================================//
    //       Operators                                                                      //
    //======================================================================================//

    /*!***************************************************************************************
    \brief  Gets vec4 from [column]
    \param  p_index - The column you want
    \return ref to the vec4
    *****************************************************************************************/
    vector2 & operator[](unsigned p_index) { return vect[p_index]; }

    /*!***************************************************************************************
    \brief  Gets vec4 from [column] (const)
    \param  p_index - The column you want
    \return Const ref to the vec4
    *****************************************************************************************/
    const vector2& operator[](unsigned p_index) const { return vect[p_index]; }

    /*!***************************************************************************************
    \brief  Adds right-hand matrix to this matrix
    \param  p_rhs - Right-hand matrix
    *****************************************************************************************/
    void operator+=(const matrix2x2 & p_rhs);

    /*!***************************************************************************************
    \brief  Subtracts right hand matrix to this matrix
    \param  p_rhs - Right-hand matrix
    *****************************************************************************************/
    void operator-=(const matrix2x2 & p_rhs);

    /*!***************************************************************************************
    \brief  Multiples right-hand matrix to this matrix
    \param  p_rhs - Right hand float
    *****************************************************************************************/
    void operator*=(const matrix2x2 & p_rhs);

    /*!***************************************************************************************
    \brief  Divides right-hand matrix to this matrix
    \param  p_rhs - Right-hand matrix
    *****************************************************************************************/
    void operator/=(const matrix2x2 & p_rhs);

    /*!***************************************************************************************
    \brief  Adds right-hand float to all elements of this matrix
    \param  p_rhs - Right-hand float
    *****************************************************************************************/
    void operator+=(const float & p_rhs);

    /*!***************************************************************************************
    \brief  Subtracts right-hand float to all elements of this matrix
    \param  p_rhs - Right-hand float
    *****************************************************************************************/
    void operator-=(const float & p_rhs);

    /*!***************************************************************************************
    \brief  Multiples right-hand float to all elements of this matrix
    \param  p_rhs - Right-hand float
    *****************************************************************************************/
    void operator*=(const float & p_rhs);

    /*!***************************************************************************************
    \brief  Divides right-hand float to all elements of this matrix
    \param  p_rhs - Right-hand float
    *****************************************************************************************/
    void operator/=(const float & p_rhs);

    /*!***************************************************************************************
    \brief  Makes temp copy of matrix and += right-hand matrix to this matrix
    \param  p_rhs - Right-hand matrix
    \return copy of temp matrix
    *****************************************************************************************/
    matrix2x2 operator+(const matrix2x2 & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of matrix and -= right-hand matrix to this matrix
    \param  p_rhs - Right-hand matrix
    \return copy of temp matrix
    *****************************************************************************************/
    matrix2x2 operator-(const matrix2x2 & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of matrix and *= right-hand matrix to this matrix
    \param  p_rhs - Right-hand matrix
    \return copy of temp matrix
    *****************************************************************************************/
    matrix2x2 operator*(const matrix2x2 & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of matrix and /= right-hand matrix to this matrix
    \param  p_rhs - Right-hand matrix
    \return copy of temp matrix
    *****************************************************************************************/
    matrix2x2 operator/(const matrix2x2 & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of matrix and += right-hand float to this matrix
    \param  p_rhs - Right-hand matrix
    \return copy of temp matrix
    *****************************************************************************************/
    matrix2x2 operator+(const float & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of matrix and -= right-hand float to this matrix
    \param  p_rhs - Right-hand float
    \return copy of temp matrix
    *****************************************************************************************/
    matrix2x2 operator-(const float & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of matrix and *= right-hand float to this matrix
    \param  p_rhs - Right-hand float
    \return copy of temp matrix
    *****************************************************************************************/
    matrix2x2 operator*(const float & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of matrix and /= right-hand float to this matrix
    \param  p_rhs - Right-hand float
    \return copy of temp matrix
    *****************************************************************************************/
    matrix2x2 operator/(const float & p_rhs) const;


    /*!***************************************************************************************
    \brief  Static funciton that build a translation matrix
    \param  p_position - position of the object
    \return Copy of built translation matrix
    *****************************************************************************************/
    static matrix2x2 translation(const vector2 & p_position);

    /*!***************************************************************************************
    \brief  Static function that builds a rotation matrix
    \param  p_axis  - Axis your rotating around
    \param  p_angle - The angle in which you are rotating
    \return copy of the built rotation matrix
    *****************************************************************************************/
    static matrix2x2 rotationAxis(const vector3 & p_axis, float p_angle);



    /*!***************************************************************************************
    \brief  Static function that builds a scale matrix
    \param  p_scale - vec3 for x,y z scale
    \return Copy of the built scale matrix
    *****************************************************************************************/
    static matrix2x2 scale(const vector2 & p_scale);

    float determinate();

    matrix2x2 Concat(matrix2x2 & rhs) const;

    /*!***************************************************************************************
    \brief  Gets a pointer to the data
    \return A float pointer to data
    *****************************************************************************************/
    const float * toFloatPtr();

    /*!***************************************************************************************
    \brief  Make string for output
    \return Output string
    *****************************************************************************************/
    std::string toStringPtr();

    //======================================================================================//
    //        Constructor                                                                   //
    //======================================================================================//

    /*!***************************************************************************************
    \brief  Constructor
    *****************************************************************************************/
    matrix2x2();

    matrix2x2(float * arr);

    /*!***************************************************************************************
    \brief  Copy Constructor
    \param  p_other - other mat4
    *****************************************************************************************/
    matrix2x2(const matrix2x2 & p_other);

    /*!***************************************************************************************
    \brief  defult destructor
    *****************************************************************************************/
    virtual ~matrix2x2() = default;

    /*!***************************************************************************************
    \brief  Inverse function
    \return Copy of the invere function
    *****************************************************************************************/
    matrix2x2 inverse();

    //==================================================================================//
    //        Getters & Setters                                                         //
    //==================================================================================//

    /*!***************************************************************************************
    \brief  Gets the Identity Matrix
    \return the identity matrix
    *****************************************************************************************/
    matrix2x2 GetIdentitiyMatrix();

    ///////============================================================================///////
    ///////   Private                                                                  ///////
    ///////============================================================================///////


    //////============================================================================//////
    //////     Data                                                                   //////
    //////============================================================================//////
#ifdef _WIN32
# pragma warning( push )
# pragma warning( disable : 4201)
#endif

    union
    {
        struct
        {
            float m00, m01,
                m10, m11;
        };
        struct
        {
            vector2 vect[2];
        };

        float array[4];
    };

#ifdef _WIN32
# pragma warning( pop )
#endif

};
