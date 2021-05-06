/*!***************************************************************************************
\file       QuaternionTests.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "gtest/gtest.h"
#include <Quaternion.hpp>
#include <Matrix4x4.hpp>
#include <MathFunctions.hpp>

TEST(Quaternion, DefaultConstructor)
{
	quaternion l_quat;

	EXPECT_FLOAT_EQ(l_quat.getX(), 0);
	EXPECT_FLOAT_EQ(l_quat.getY(), 0);
	EXPECT_FLOAT_EQ(l_quat.getZ(), 0);
	EXPECT_FLOAT_EQ(l_quat.getW(), 1);
}

TEST(Quaternion, ToMatrix)
{
		quaternion l_xonlyquat(vector3(1, 0, 0), 15);
		matrix4x4 l_xonlymat = l_xonlyquat.toMatrix4x4();

		EXPECT_FLOAT_EQ(l_xonlymat[0][0], 1.0f);
		EXPECT_FLOAT_EQ(l_xonlymat[0][1], 0.0f);
		EXPECT_FLOAT_EQ(l_xonlymat[0][2], 0.0f);
		EXPECT_FLOAT_EQ(l_xonlymat[0][3], 0.0f);

		EXPECT_FLOAT_EQ(l_xonlymat[1][0], 0.0f);
		EXPECT_FLOAT_EQ(l_xonlymat[1][1], -0.7596879f);
		EXPECT_FLOAT_EQ(l_xonlymat[1][2], -0.6502879f);
		EXPECT_FLOAT_EQ(l_xonlymat[1][3], 0.0f);

		EXPECT_FLOAT_EQ(l_xonlymat[2][0], 0.0f);
		EXPECT_FLOAT_EQ(l_xonlymat[2][1], 0.6502879f);
		EXPECT_FLOAT_EQ(l_xonlymat[2][2], -0.7596879f);
		EXPECT_FLOAT_EQ(l_xonlymat[2][3], 0.0f);

		EXPECT_FLOAT_EQ(l_xonlymat[3][0], 0.0f);
		EXPECT_FLOAT_EQ(l_xonlymat[3][1], 0.0f);
		EXPECT_FLOAT_EQ(l_xonlymat[3][2], 0.0f);
		EXPECT_FLOAT_EQ(l_xonlymat[3][3], 1.0f);

    quaternion q(0.0423667f, 0.5481793f, 0.3004205f, 0.7793921f);

    matrix4x4 l_mat = q.toMatrix4x4();

    EXPECT_FLOAT_EQ(l_mat[0][0], 0.21849396004184873f);
    EXPECT_FLOAT_EQ(l_mat[0][1], -0.4218416309254284f);
    EXPECT_FLOAT_EQ(l_mat[0][2], 0.8799488779715592f);
    EXPECT_FLOAT_EQ(l_mat[0][3], 0.0f);
												 
    EXPECT_FLOAT_EQ(l_mat[1][0], 0.5147398222971911f);
    EXPECT_FLOAT_EQ(l_mat[1][1], 0.8159051726648817f);
    EXPECT_FLOAT_EQ(l_mat[1][2], 0.263328055019107f);
    EXPECT_FLOAT_EQ(l_mat[1][3], 0.0f);

    EXPECT_FLOAT_EQ(l_mat[2][0], -0.8290375774153349f);
    EXPECT_FLOAT_EQ(l_mat[2][1], 0.39540913954645035f);
    EXPECT_FLOAT_EQ(l_mat[2][2], 0.39540903833429064f);
    EXPECT_FLOAT_EQ(l_mat[2][3], 0.0f);

    EXPECT_FLOAT_EQ(l_mat[3][0], 0.0f);
    EXPECT_FLOAT_EQ(l_mat[3][1], 0.0f);
    EXPECT_FLOAT_EQ(l_mat[3][2], 0.0f);
    EXPECT_FLOAT_EQ(l_mat[3][3], 1.0f);
}

TEST(Quaternion, AxisAngle)
{
	const quaternion l_xonlyquat(vector3(1, 0, 0), 1);
	EXPECT_FLOAT_EQ(l_xonlyquat.getX(), 0.4794255f);
	EXPECT_FLOAT_EQ(l_xonlyquat.getY(), 0.0f);
	EXPECT_FLOAT_EQ(l_xonlyquat.getZ(), 0.0f);
	EXPECT_FLOAT_EQ(l_xonlyquat.getW(), 0.8775826f);

	const quaternion l_yonlyquat(vector3(0, 1, 0), 1);
	EXPECT_FLOAT_EQ(l_yonlyquat.getX(), 0.0f);
	EXPECT_FLOAT_EQ(l_yonlyquat.getY(), 0.4794255f);
	EXPECT_FLOAT_EQ(l_yonlyquat.getZ(), 0.0f);
	EXPECT_FLOAT_EQ(l_yonlyquat.getW(), 0.8775826f);

	const quaternion l_zonlyquat(vector3(0, 0, 1), 1);
	EXPECT_FLOAT_EQ(l_zonlyquat.getX(), 0.0f);
	EXPECT_FLOAT_EQ(l_zonlyquat.getY(), 0.0f);
	EXPECT_FLOAT_EQ(l_zonlyquat.getZ(), 0.4794255f);
	EXPECT_FLOAT_EQ(l_zonlyquat.getW(), 0.8775826f);


	const vector3 l_axis(12, 23, 34);
	const float l_angle = 56;
	quaternion l_quat(l_axis, l_angle);

	EXPECT_FLOAT_EQ(l_quat.getX(), 0.0760138f);
	EXPECT_FLOAT_EQ(l_quat.getY(), 0.1456932f);
	EXPECT_FLOAT_EQ(l_quat.getZ(), 0.2153725f);
	EXPECT_FLOAT_EQ(l_quat.getW(), -0.9626059f);
}

TEST(Quaternion, Concatenation)
{
	const quaternion l_initial = toQuaternion({ 12, 23, 34 });
	const quaternion l_offset = toQuaternion({ 100, 100, 100 });

	const quaternion l_expected(0.16882254291303378f, 0.82141781651091506f, 0.091512535249119425f, 0.53702623363770252f);

	const quaternion l_actual = l_offset * l_initial;

	EXPECT_FLOAT_EQ(l_actual.x, l_expected.x);
	EXPECT_FLOAT_EQ(l_actual.y, l_expected.y);
	EXPECT_FLOAT_EQ(l_actual.z, l_expected.z);
	EXPECT_FLOAT_EQ(l_actual.w, l_expected.w);
}
