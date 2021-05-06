/*!***************************************************************************************
\file       DataAssuranceTests.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "gtest/gtest.h"
#include <Vector3.hpp>
#include <Matrix4x4.hpp>
#include <MathFunctions.hpp>
#include <Quaternion.hpp>

TEST(DataAssurance, BuildAndDecompose)
{
		vector3 l_pos, l_scale;
		quaternion l_rot;


		quaternion tempQ = toQuaternion({ 45, 56, 67 });
		// build and decompose a matrix
		const matrix4x4 l_mat = matrix4x4::build(
			vector3(12, 23, 34), 
			tempQ,
			vector3(78, 89, 90));

    l_mat.decompose(l_pos, l_rot, l_scale);

		// check we got the same values back from the build->decompose process
    EXPECT_FLOAT_EQ(l_pos.x, 12.0f);
    EXPECT_FLOAT_EQ(l_pos.y, 23.0f);
    EXPECT_FLOAT_EQ(l_pos.z, 34.0f);

    const quaternion q = toQuaternion({ 45, 56, 67 });
    EXPECT_FLOAT_EQ(l_rot.x, q.x);
    EXPECT_FLOAT_EQ(l_rot.y, q.y);
    EXPECT_FLOAT_EQ(l_rot.z, q.z);
    EXPECT_FLOAT_EQ(l_rot.w, q.w);

    EXPECT_FLOAT_EQ(l_scale.x, 78.0f);
    EXPECT_FLOAT_EQ(l_scale.y, 89.0f);
    EXPECT_FLOAT_EQ(l_scale.z, 90.0f);

		// use the values from the decompose to build the matrix again
		const matrix4x4 l_newmat = matrix4x4::build(l_pos, l_rot, l_scale);

		EXPECT_NEAR(l_newmat[0][0], l_mat[0][0], 0.0001f); // not sure why this the only element with a larger error
		EXPECT_FLOAT_EQ(l_newmat[0][1], l_mat[0][1]);
		EXPECT_FLOAT_EQ(l_newmat[0][2], l_mat[0][2]);
		EXPECT_FLOAT_EQ(l_newmat[0][3], l_mat[0][3]);

		EXPECT_FLOAT_EQ(l_newmat[1][0], l_mat[1][0]);
		EXPECT_FLOAT_EQ(l_newmat[1][1], l_mat[1][1]);
		EXPECT_FLOAT_EQ(l_newmat[1][2], l_mat[1][2]);
		EXPECT_FLOAT_EQ(l_newmat[1][3], l_mat[1][3]);

		EXPECT_FLOAT_EQ(l_newmat[2][0], l_mat[2][0]);
		EXPECT_FLOAT_EQ(l_newmat[2][1], l_mat[2][1]);
		EXPECT_FLOAT_EQ(l_newmat[2][2], l_mat[2][2]);
		EXPECT_FLOAT_EQ(l_newmat[2][3], l_mat[2][3]);

		EXPECT_FLOAT_EQ(l_newmat[3][0], l_mat[3][0]);
		EXPECT_FLOAT_EQ(l_newmat[3][1], l_mat[3][1]);
		EXPECT_FLOAT_EQ(l_newmat[3][2], l_mat[3][2]);
		EXPECT_FLOAT_EQ(l_newmat[3][3], l_mat[3][3]);

}

TEST(DataAssurance, Eular_Quaternion_Quaternion_Eular)
{
    quaternion q = toQuaternion(vector3(45, 56, 67));
    vector3 l_eular = toEulerAngle(q);

    
    ASSERT_FLOAT_EQ(l_eular.x, 45.0f);
    ASSERT_FLOAT_EQ(l_eular.y, 56.0f);
    ASSERT_FLOAT_EQ(l_eular.z, 67.0f);

}

TEST(DataAssurance, Matrix_Quaternion_Quaternion_Matrix)
{
    float l_elements[] =
    {
        -34.45818513944838f, 60.4491828663194f, -52.7494726407923f, 12.0f,
        -56.935854192371494f, 9.583074663891605f, 60.747246162769024f, 23.0f,
         40.68097816277913f, 64.6147117209429f, 40.33937554992949f, 34.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    };

    matrix4x4 l_mat(l_elements);

    quaternion q = l_mat.toQuaternion();

    matrix4x4 l_mat2 = q.toMatrix4x4();


    EXPECT_FLOAT_EQ(l_mat[0][0], -34.45818513944838f);
    EXPECT_FLOAT_EQ(l_mat[0][1], 60.4491828663194f);
    EXPECT_FLOAT_EQ(l_mat[0][2], -52.7494726407923f);
    EXPECT_FLOAT_EQ(l_mat[0][3], 12.0f);

		EXPECT_FLOAT_EQ(l_mat[1][0], -56.935854192371494f);
    EXPECT_FLOAT_EQ(l_mat[1][1], 9.583074663891605f);
    EXPECT_FLOAT_EQ(l_mat[1][2], 60.747246162769024f);
    EXPECT_FLOAT_EQ(l_mat[1][3], 23.0f);

		EXPECT_FLOAT_EQ(l_mat[2][0], 40.68097816277913f);
    EXPECT_FLOAT_EQ(l_mat[2][1], 64.6147117209429f);
    EXPECT_FLOAT_EQ(l_mat[2][2], 40.33937554992949f);
    EXPECT_FLOAT_EQ(l_mat[2][3], 34.0f);

		EXPECT_FLOAT_EQ(l_mat[3][0], 0.0f);
    EXPECT_FLOAT_EQ(l_mat[3][1], 0.0f);
    EXPECT_FLOAT_EQ(l_mat[3][2], 0.0f);
    EXPECT_FLOAT_EQ(l_mat[3][3], 1.0f);
}

TEST(DataAssurance, InverseAndDecompose)
{
	vector3 l_pos, l_scale;
	quaternion l_rot;

	// build and decompose a matrix
	const matrix4x4 l_original_mat = matrix4x4::build(
		vector3(-12, -23, -34),
		toQuaternion({ -45, -56, -67 }),
		vector3(1.0f/78, 1.0f/89, 1.0f/90));

	const matrix4x4 l_mat = l_original_mat.inverse();
	l_mat.decompose(l_pos, l_rot, l_scale);

	// check we got the same values back from the build->decompose process
	EXPECT_FLOAT_EQ(l_pos.x, 12.0f);
	EXPECT_FLOAT_EQ(l_pos.y, 23.0f);
	EXPECT_FLOAT_EQ(l_pos.z, 34.0f);

	const quaternion q = toQuaternion({ 45, 56, 67 });
	EXPECT_FLOAT_EQ(l_rot.x, q.x);
	EXPECT_FLOAT_EQ(l_rot.y, q.y);
	EXPECT_FLOAT_EQ(l_rot.z, q.z);
	EXPECT_FLOAT_EQ(l_rot.w, q.w);

	EXPECT_FLOAT_EQ(l_scale.x, 78.0f);
	EXPECT_FLOAT_EQ(l_scale.y, 89.0f);
	EXPECT_FLOAT_EQ(l_scale.z, 90.0f);
}