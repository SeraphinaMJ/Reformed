/*!***************************************************************************************
\file       MathFunctionsTest.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "gtest/gtest.h"
#include <Vector3.hpp>
#include <MathFunctions.hpp>
#include <Quaternion.hpp>


TEST(MathFunctions, Eular_ZYX_ToQuaternion)
{

		// eular is in a zyx ordering
    
    quaternion q = toQuaternion(vector3(45, 56, 67));

    ASSERT_FLOAT_EQ(q.x, 0.0423667f);
    ASSERT_FLOAT_EQ(q.y, 0.5481793f);
    ASSERT_FLOAT_EQ(q.z, 0.3004205f);
    ASSERT_FLOAT_EQ(q.w, 0.7793921f);
}