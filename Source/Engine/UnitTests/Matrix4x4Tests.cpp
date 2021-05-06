/*!***************************************************************************************
\file       Matrix4x4Tests.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include <gtest/gtest.h>
#include <Matrix4x4.hpp>
#include <glm/glm.hpp>
#include <Vector4.hpp>
#include <Vector3.hpp>
#include "PrintHelpers.hpp"
#include <MathFunctions.hpp>
#include <Quaternion.hpp>

TEST(Matrix4x4, Construction)
{
	matrix4x4 l_mat;
	EXPECT_EQ(l_mat[0], vector4(1, 0, 0, 0));
	EXPECT_EQ(l_mat[1], vector4(0, 1, 0, 0));
	EXPECT_EQ(l_mat[2], vector4(0, 0, 1, 0));
	EXPECT_EQ(l_mat[3], vector4(0, 0, 0, 1));

	const matrix4x4 & l_cmat = l_mat;
	EXPECT_EQ(l_cmat[0], vector4(1, 0, 0, 0));
	EXPECT_EQ(l_cmat[1], vector4(0, 1, 0, 0));
	EXPECT_EQ(l_cmat[2], vector4(0, 0, 1, 0));
	EXPECT_EQ(l_cmat[3], vector4(0, 0, 0, 1));

	matrix4x4 l_mat2(vector3(9, 10, 11), vector3(5, 6, 7), vector3(1, 2, 3));
	EXPECT_EQ(l_mat2.right(), vector3(1, 2, 3));
	EXPECT_EQ(l_mat2.up(), vector3(5, 6, 7));
	EXPECT_EQ(l_mat2.forward(), vector3(9, 10, 11));
	EXPECT_EQ(l_mat2.position(), vector3(0, 0, 0));
	EXPECT_EQ(l_mat2[0][3], 0);
	EXPECT_EQ(l_mat2[1][3], 0);
	EXPECT_EQ(l_mat2[2][3], 0);
	EXPECT_EQ(l_mat2[3][3], 1);
}

TEST(Matrix4x4, CopyConstructor)
{
	const float l_elements[] ={
		1, 2, 3, 4,  5, 6, 7, 8,   9, 10, 11, 12,   13, 14, 15, 16,
	};
	matrix4x4 l_mat(l_elements);
	matrix4x4 l_mat2(l_mat);

	EXPECT_EQ(l_mat2[0], vector4(1, 2, 3, 4));
	EXPECT_EQ(l_mat2[1], vector4(5, 6, 7, 8));
	EXPECT_EQ(l_mat2[2], vector4(9, 10, 11, 12));
	EXPECT_EQ(l_mat2[3], vector4(13, 14, 15, 16));
}

TEST(Matrix4x4, floatarrayConversion)
{
	const float l_arr[16] =
	{
		1, 2, 3, 4,  5, 6, 7, 8,
		11, 12, 13, 14,  15, 16, 17, 18
	};

	//matrix4x4 l_mat(l_arr);
	//EXPECT_EQ(l_mat[0][0], 1);
	//EXPECT_EQ(l_mat[0][1], 2);
	//EXPECT_EQ(l_mat[0][2], 3);
	//EXPECT_EQ(l_mat[0][3], 4);

	//EXPECT_EQ(l_mat[1][0], 5);
	//EXPECT_EQ(l_mat[1][1], 6);
	//EXPECT_EQ(l_mat[1][2], 7);
	//EXPECT_EQ(l_mat[1][3], 8);

	//EXPECT_EQ(l_mat[2][0], 11);
	//EXPECT_EQ(l_mat[2][1], 12);
	//EXPECT_EQ(l_mat[2][2], 13);
	//EXPECT_EQ(l_mat[2][3], 14);

	//EXPECT_EQ(l_mat[3][0], 15);
	//EXPECT_EQ(l_mat[3][1], 16);
	//EXPECT_EQ(l_mat[3][2], 17);
	//EXPECT_EQ(l_mat[3][3], 18);

#ifdef ColumnBasis
	matrix4x4 l_mat2(l_arr);
	EXPECT_EQ(l_mat2[0][0], 1);
	EXPECT_EQ(l_mat2[0][1], 2);
	EXPECT_EQ(l_mat2[0][2], 3);
	EXPECT_EQ(l_mat2[0][3], 4);

	EXPECT_EQ(l_mat2[1][0], 5);
	EXPECT_EQ(l_mat2[1][1], 6);
	EXPECT_EQ(l_mat2[1][2], 7);
	EXPECT_EQ(l_mat2[1][3], 8);

	EXPECT_EQ(l_mat2[2][0], 11);
	EXPECT_EQ(l_mat2[2][1], 12);
	EXPECT_EQ(l_mat2[2][2], 13);
	EXPECT_EQ(l_mat2[2][3], 14);

	EXPECT_EQ(l_mat2[3][0], 15);
	EXPECT_EQ(l_mat2[3][1], 16);
	EXPECT_EQ(l_mat2[3][2], 17);
	EXPECT_EQ(l_mat2[3][3], 18);

#else
	matrix4x4 l_mat3(l_arr);
	EXPECT_EQ(l_mat3[0][0], 1);
	EXPECT_EQ(l_mat3[0][1], 5);
	EXPECT_EQ(l_mat3[0][2], 11);
	EXPECT_EQ(l_mat3[0][3], 15);

	EXPECT_EQ(l_mat3[1][0], 2);
	EXPECT_EQ(l_mat3[1][1], 6);
	EXPECT_EQ(l_mat3[1][2], 12);
	EXPECT_EQ(l_mat3[1][3], 16);

	EXPECT_EQ(l_mat3[2][0], 3);
	EXPECT_EQ(l_mat3[2][1], 7);
	EXPECT_EQ(l_mat3[2][2], 13);
	EXPECT_EQ(l_mat3[2][3], 17);

	EXPECT_EQ(l_mat3[3][0], 4);
	EXPECT_EQ(l_mat3[3][1], 8);
	EXPECT_EQ(l_mat3[3][2], 14);
	EXPECT_EQ(l_mat3[3][3], 18);
#endif

}

//TEST(Matrix4x4, PlusEqualsOperator)
//{
//	const float l_elements1[] = { 1,2,3,4,  5,6,7,8,  11, 12,13,14,  15,16,17,18 };
//	const float l_elements2[] = { 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0 };
//	matrix4x4 l_mat2(l_elements1, false);
//	matrix4x4 l_mat(l_elements2, false);
//
//	l_mat += l_mat2;
//
//	EXPECT_EQ(l_mat[0][0], 0 + 1);
//	EXPECT_EQ(l_mat[0][1], 0 + 2);
//	EXPECT_EQ(l_mat[0][2], 0 + 3);
//	EXPECT_EQ(l_mat[0][3], 0 + 4);
//
//	EXPECT_EQ(l_mat[1][0], 0 + 5);
//	EXPECT_EQ(l_mat[1][1], 0 + 6);
//	EXPECT_EQ(l_mat[1][2], 0 + 7);
//	EXPECT_EQ(l_mat[1][3], 0 + 8);
//
//	EXPECT_EQ(l_mat[2][0], 0 + 11);
//	EXPECT_EQ(l_mat[2][1], 0 + 12);
//	EXPECT_EQ(l_mat[2][2], 0 + 13);
//	EXPECT_EQ(l_mat[2][3], 0 + 14);
//
//	EXPECT_EQ(l_mat[3][0], 0 + 15);
//	EXPECT_EQ(l_mat[3][1], 0 + 16);
//	EXPECT_EQ(l_mat[3][2], 0 + 17);
//	EXPECT_EQ(l_mat[3][3], 0 + 18);
//
//	l_mat += 11.0f;
//
//	EXPECT_EQ(l_mat[0][0], 0 + 1 + 11.0f);
//	EXPECT_EQ(l_mat[0][1], 0 + 2 + 11.0f);
//	EXPECT_EQ(l_mat[0][2], 0 + 3 + 11.0f);
//	EXPECT_EQ(l_mat[0][3], 0 + 4 + 11.0f);
//
//	EXPECT_EQ(l_mat[1][0], 0 + 5 + 11.0f);
//	EXPECT_EQ(l_mat[1][1], 0 + 6 + 11.0f);
//	EXPECT_EQ(l_mat[1][2], 0 + 7 + 11.0f);
//	EXPECT_EQ(l_mat[1][3], 0 + 8 + 11.0f);
//
//	EXPECT_EQ(l_mat[2][0], 0 + 11 + 11.0f);
//	EXPECT_EQ(l_mat[2][1], 0 + 12 + 11.0f);
//	EXPECT_EQ(l_mat[2][2], 0 + 13 + 11.0f);
//	EXPECT_EQ(l_mat[2][3], 0 + 14 + 11.0f);
//
//	EXPECT_EQ(l_mat[3][0], 0 + 15 + 11.0f);
//	EXPECT_EQ(l_mat[3][1], 0 + 16 + 11.0f);
//	EXPECT_EQ(l_mat[3][2], 0 + 17 + 11.0f);
//	EXPECT_EQ(l_mat[3][3], 0 + 18 + 11.0f);
//
//}

//TEST(Matrix4x4, MinusEqualsOperator)
//{
//	const float l_elements1[] = { 1,2,3,4,  5,6,7,8,  11, 12,13,14,  15,16,17,18 };
//	const float l_elements2[] = { 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0 };
//	matrix4x4 l_mat2(l_elements1, false);
//	matrix4x4 l_mat(l_elements2, false);
//
//	l_mat -= l_mat2;
//
//	EXPECT_EQ(l_mat[0][0], 0 - 1);
//	EXPECT_EQ(l_mat[0][1], 0 - 2);
//	EXPECT_EQ(l_mat[0][2], 0 - 3);
//	EXPECT_EQ(l_mat[0][3], 0 - 4);
//
//	EXPECT_EQ(l_mat[1][0], 0 - 5);
//	EXPECT_EQ(l_mat[1][1], 0 - 6);
//	EXPECT_EQ(l_mat[1][2], 0 - 7);
//	EXPECT_EQ(l_mat[1][3], 0 - 8);
//
//	EXPECT_EQ(l_mat[2][0], 0 - 11);
//	EXPECT_EQ(l_mat[2][1], 0 - 12);
//	EXPECT_EQ(l_mat[2][2], 0 - 13);
//	EXPECT_EQ(l_mat[2][3], 0 - 14);
//
//	EXPECT_EQ(l_mat[3][0], 0 - 15);
//	EXPECT_EQ(l_mat[3][1], 0 - 16);
//	EXPECT_EQ(l_mat[3][2], 0 - 17);
//	EXPECT_EQ(l_mat[3][3], 0 - 18);
//
//	l_mat -= 11.0f;
//
//	EXPECT_EQ(l_mat[0][0], 0 - 1 - 11.0f);
//	EXPECT_EQ(l_mat[0][1], 0 - 2 - 11.0f);
//	EXPECT_EQ(l_mat[0][2], 0 - 3 - 11.0f);
//	EXPECT_EQ(l_mat[0][3], 0 - 4 - 11.0f);
//
//	EXPECT_EQ(l_mat[1][0], 0 - 5 - 11.0f);
//	EXPECT_EQ(l_mat[1][1], 0 - 6 - 11.0f);
//	EXPECT_EQ(l_mat[1][2], 0 - 7 - 11.0f);
//	EXPECT_EQ(l_mat[1][3], 0 - 8 - 11.0f);
//
//	EXPECT_EQ(l_mat[2][0], 0 - 11 - 11.0f);
//	EXPECT_EQ(l_mat[2][1], 0 - 12 - 11.0f);
//	EXPECT_EQ(l_mat[2][2], 0 - 13 - 11.0f);
//	EXPECT_EQ(l_mat[2][3], 0 - 14 - 11.0f);
//
//	EXPECT_EQ(l_mat[3][0], 0 - 15 - 11.0f);
//	EXPECT_EQ(l_mat[3][1], 0 - 16 - 11.0f);
//	EXPECT_EQ(l_mat[3][2], 0 - 17 - 11.0f);
//	EXPECT_EQ(l_mat[3][3], 0 - 18 - 11.0f);
//}

TEST(Matrix4x4, MultiplyEqualsOperator)
{
	const float l_elements1[] = {
		2,11,23,43,
		3,13,29,47,
		5,17,31,51,
		7,19,37,53 };
	const float l_elements2[] = {
		1,5,11,15,
		2,6,12,16,
		3,7,13,17,
		4,8,14,18 };
	matrix4x4 l_mat(l_elements1);
	matrix4x4 l_mat2(l_elements2);

	l_mat *= l_mat2;

	EXPECT_EQ(l_mat[0], vector4(265, 581, 1055, 1371));
	EXPECT_EQ(l_mat[1], vector4(304, 672, 1224, 1592));
	EXPECT_EQ(l_mat[2], vector4(336, 752, 1376, 1792));
	EXPECT_EQ(l_mat[3], vector4(368, 832, 1528, 1992));

	l_mat *= 11.0f;

	EXPECT_EQ(l_mat[0][0], 265 * 11.0f);
	EXPECT_EQ(l_mat[1][0], 304 * 11.0f);
	EXPECT_EQ(l_mat[2][0], 336 * 11.0f);
	EXPECT_EQ(l_mat[3][0], 368 * 11.0f);

	EXPECT_EQ(l_mat[0][1], 581 * 11.0f);
	EXPECT_EQ(l_mat[1][1], 672 * 11.0f);
	EXPECT_EQ(l_mat[2][1], 752 * 11.0f);
	EXPECT_EQ(l_mat[3][1], 832 * 11.0f);

	EXPECT_EQ(l_mat[0][2], 1055 * 11.0f);
	EXPECT_EQ(l_mat[1][2], 1224 * 11.0f);
	EXPECT_EQ(l_mat[2][2], 1376 * 11.0f);
	EXPECT_EQ(l_mat[3][2], 1528 * 11.0f);

	EXPECT_EQ(l_mat[0][3], 1371 * 11.0f);
	EXPECT_EQ(l_mat[1][3], 1592 * 11.0f);
	EXPECT_EQ(l_mat[2][3], 1792 * 11.0f);
	EXPECT_EQ(l_mat[3][3], 1992 * 11.0f);
}

//TEST(Matrix4x4, DivideEqualsOperator)
//{
//    const float l_elements1[] = {
//        -0.441771590604f, 0.729946828901f, -0.521550975194f, 0,
//        -0.219642632741f, -0.65166670042f, -0.726008023205f, 0,
//        -0.869824665428f, -0.206174888003f, 0.448215253753f, 0,
//        0, 0, 0, 1
//    };
//	const float l_elements2[] = { 2,3,4,5,  6,7,8,9,  12,13,14,15,  16,17,18,19 };
//	matrix4x4 l_mat2(l_elements1, false);
//	matrix4x4 l_mat(l_elements2, false);
//
//	l_mat /= l_mat2;
//
//	EXPECT_EQ(l_mat[0][0], -0.779906595283f);
//	EXPECT_EQ(l_mat[0][1], -5.29831745956f);
//	EXPECT_EQ(l_mat[0][2], -0.565312979854f);
//	EXPECT_EQ(l_mat[0][3], 5.0f);
//
//	EXPECT_EQ(l_mat[1][0], -1.71340954287f);
//	EXPECT_EQ(l_mat[1][1], -11.687586885f);
//	EXPECT_EQ(l_mat[1][2], -3.07645017857f);
//	EXPECT_EQ(l_mat[1][3], 9.0f);
//
//	EXPECT_EQ(l_mat[2][0], -3.11366396426f);
//	EXPECT_EQ(l_mat[2][1], -21.2714910232f);
//	EXPECT_EQ(l_mat[2][2], -6.84315597663f);
//	EXPECT_EQ(l_mat[2][3], 15.0f);
//
//	EXPECT_EQ(l_mat[3][0], -4.04716691185f);
//	EXPECT_EQ(l_mat[3][1], -27.6607604487f);
//	EXPECT_EQ(l_mat[3][2], -9.35429317535f);
//	EXPECT_EQ(l_mat[3][3], 19.0f);
//}

//TEST(Matrix4x4, PlusOperator)
//{
//	float l_elements1[] = {
//		1, 2, 3, 4,
//		5, 6, 7, 8,
//		9, 10, 11, 12,
//		13, 14, 15, 16,
//	};
//	float l_elements2[] = {
//		101, 102, 103, 104,
//		105, 106, 107, 108,
//		109, 110, 111, 112,
//		113, 114, 115, 116,
//	};
//
//	matrix4x4 l_mat1(l_elements1);
//	matrix4x4 l_mat2(l_elements2);
//
//	matrix4x4 l_mat = l_mat1 + l_mat2;
//
//	ASSERT_FLOAT_EQ(l_mat[0][0], 1 + 101);
//	ASSERT_FLOAT_EQ(l_mat[0][1], 2 + 102);
//	ASSERT_FLOAT_EQ(l_mat[0][2], 3 + 103);
//	ASSERT_FLOAT_EQ(l_mat[0][3], 4 + 104);
//
//	ASSERT_FLOAT_EQ(l_mat[1][0], 5 + 105);
//	ASSERT_FLOAT_EQ(l_mat[1][1], 6 + 106);
//	ASSERT_FLOAT_EQ(l_mat[1][2], 7 + 107);
//	ASSERT_FLOAT_EQ(l_mat[1][3], 8 + 108);
//
//	ASSERT_FLOAT_EQ(l_mat[2][0], 9 + 109);
//	ASSERT_FLOAT_EQ(l_mat[2][1], 10 + 110);
//	ASSERT_FLOAT_EQ(l_mat[2][2], 11 + 111);
//	ASSERT_FLOAT_EQ(l_mat[2][3], 12 + 112);
//
//	ASSERT_FLOAT_EQ(l_mat[3][0], 13 + 113);
//	ASSERT_FLOAT_EQ(l_mat[3][1], 14 + 114);
//	ASSERT_FLOAT_EQ(l_mat[3][2], 15 + 115);
//	ASSERT_FLOAT_EQ(l_mat[3][3], 16 + 116);
//
//	l_mat = l_mat + 33.0f;
//
//	ASSERT_FLOAT_EQ(l_mat[0][0], 1 + 101 + 33);
//	ASSERT_FLOAT_EQ(l_mat[0][1], 2 + 102 + 33);
//	ASSERT_FLOAT_EQ(l_mat[0][2], 3 + 103 + 33);
//	ASSERT_FLOAT_EQ(l_mat[0][3], 4 + 104 + 33);
//
//	ASSERT_FLOAT_EQ(l_mat[1][0], 5 + 105 + 33);
//	ASSERT_FLOAT_EQ(l_mat[1][1], 6 + 106 + 33);
//	ASSERT_FLOAT_EQ(l_mat[1][2], 7 + 107 + 33);
//	ASSERT_FLOAT_EQ(l_mat[1][3], 8 + 108 + 33);
//
//	ASSERT_FLOAT_EQ(l_mat[2][0], 9 + 109 + 33);
//	ASSERT_FLOAT_EQ(l_mat[2][1], 10 + 110 + 33);
//	ASSERT_FLOAT_EQ(l_mat[2][2], 11 + 111 + 33);
//	ASSERT_FLOAT_EQ(l_mat[2][3], 12 + 112 + 33);
//
//	ASSERT_FLOAT_EQ(l_mat[3][0], 13 + 113 + 33);
//	ASSERT_FLOAT_EQ(l_mat[3][1], 14 + 114 + 33);
//	ASSERT_FLOAT_EQ(l_mat[3][2], 15 + 115 + 33);
//	ASSERT_FLOAT_EQ(l_mat[3][3], 16 + 116 + 33);
//
//}

//TEST(Matrix4x4, MinusOperator)
//{
//	float l_elements1[] = {
//		1, 2, 3, 4,
//		5, 6, 7, 8,
//		9, 10, 11, 12,
//		13, 14, 15, 16,
//	};
//	float l_elements2[] = {
//		101, 102, 103, 104,
//		105, 106, 107, 108,
//		109, 110, 111, 112,
//		113, 114, 115, 116,
//	};
//
//	matrix4x4 l_mat1(l_elements1);
//	matrix4x4 l_mat2(l_elements2);
//
//	matrix4x4 l_mat = l_mat1 - l_mat2;
//
//	EXPECT_FLOAT_EQ(l_mat[0][0], 1 - 101);
//	EXPECT_FLOAT_EQ(l_mat[0][1], 2 - 102);
//	EXPECT_FLOAT_EQ(l_mat[0][2], 3 - 103);
//	EXPECT_FLOAT_EQ(l_mat[0][3], 4 - 104);
//
//	EXPECT_FLOAT_EQ(l_mat[1][0], 5 - 105);
//	EXPECT_FLOAT_EQ(l_mat[1][1], 6 - 106);
//	EXPECT_FLOAT_EQ(l_mat[1][2], 7 - 107);
//	EXPECT_FLOAT_EQ(l_mat[1][3], 8 - 108);
//
//	EXPECT_FLOAT_EQ(l_mat[2][0], 9 - 109);
//	EXPECT_FLOAT_EQ(l_mat[2][1], 10 - 110);
//	EXPECT_FLOAT_EQ(l_mat[2][2], 11 - 111);
//	EXPECT_FLOAT_EQ(l_mat[2][3], 12 - 112);
//
//	EXPECT_FLOAT_EQ(l_mat[3][0], 13 - 113);
//	EXPECT_FLOAT_EQ(l_mat[3][1], 14 - 114);
//	EXPECT_FLOAT_EQ(l_mat[3][2], 15 - 115);
//	EXPECT_FLOAT_EQ(l_mat[3][3], 16 - 116);
//
//	l_mat = l_mat - 77.0f;
//
//	EXPECT_FLOAT_EQ(l_mat[0][0], 1 - 101 - 77);
//	EXPECT_FLOAT_EQ(l_mat[0][1], 2 - 102 - 77);
//	EXPECT_FLOAT_EQ(l_mat[0][2], 3 - 103 - 77);
//	EXPECT_FLOAT_EQ(l_mat[0][3], 4 - 104 - 77);
//
//	EXPECT_FLOAT_EQ(l_mat[1][0], 5 - 105 - 77);
//	EXPECT_FLOAT_EQ(l_mat[1][1], 6 - 106 - 77);
//	EXPECT_FLOAT_EQ(l_mat[1][2], 7 - 107 - 77);
//	EXPECT_FLOAT_EQ(l_mat[1][3], 8 - 108 - 77);
//
//	EXPECT_FLOAT_EQ(l_mat[2][0], 9 - 109 - 77);
//	EXPECT_FLOAT_EQ(l_mat[2][1], 10 - 110 - 77);
//	EXPECT_FLOAT_EQ(l_mat[2][2], 11 - 111 - 77);
//	EXPECT_FLOAT_EQ(l_mat[2][3], 12 - 112 - 77);
//
//	EXPECT_FLOAT_EQ(l_mat[3][0], 13 - 113 - 77);
//	EXPECT_FLOAT_EQ(l_mat[3][1], 14 - 114 - 77);
//	EXPECT_FLOAT_EQ(l_mat[3][2], 15 - 115 - 77);
//	EXPECT_FLOAT_EQ(l_mat[3][3], 16 - 116 - 77);
//
//}

TEST(Matrix4x4, MultiplyOperator)
{
	float l_elements1[] = {
		1, 5, 9, 13,
		2, 6, 10, 14,
		3, 7, 11, 15,
		4, 8, 12, 16,
	};
	float l_elements2[] = {
		101, 105, 109, 113,
		102, 106, 110, 114,
		103, 107, 111, 115,
		104, 108, 112, 116,
	};

	matrix4x4 l_mat1(l_elements1);
	matrix4x4 l_mat2(l_elements2);

	matrix4x4 l_mat = l_mat2 * l_mat1;

	EXPECT_FLOAT_EQ(l_mat[0][0], 1090);
	EXPECT_FLOAT_EQ(l_mat[0][1], 2802);
	EXPECT_FLOAT_EQ(l_mat[0][2], 4514);
	EXPECT_FLOAT_EQ(l_mat[0][3], 6226);

	EXPECT_FLOAT_EQ(l_mat[1][0], 1100);
	EXPECT_FLOAT_EQ(l_mat[1][1], 2828);
	EXPECT_FLOAT_EQ(l_mat[1][2], 4556);
	EXPECT_FLOAT_EQ(l_mat[1][3], 6284);

	EXPECT_FLOAT_EQ(l_mat[2][0], 1110);
	EXPECT_FLOAT_EQ(l_mat[2][1], 2854);
	EXPECT_FLOAT_EQ(l_mat[2][2], 4598);
	EXPECT_FLOAT_EQ(l_mat[2][3], 6342);

	EXPECT_FLOAT_EQ(l_mat[3][0], 1120);
	EXPECT_FLOAT_EQ(l_mat[3][1], 2880);
	EXPECT_FLOAT_EQ(l_mat[3][2], 4640);
	EXPECT_FLOAT_EQ(l_mat[3][3], 6400);

	l_mat = l_mat * 17;

	EXPECT_FLOAT_EQ(l_mat[0][0], 1090 * 17);
	EXPECT_FLOAT_EQ(l_mat[0][1], 2802 * 17);
	EXPECT_FLOAT_EQ(l_mat[0][2], 4514 * 17);
	EXPECT_FLOAT_EQ(l_mat[0][3], 6226 * 17);

	EXPECT_FLOAT_EQ(l_mat[1][0], 1100 * 17);
	EXPECT_FLOAT_EQ(l_mat[1][1], 2828 * 17);
	EXPECT_FLOAT_EQ(l_mat[1][2], 4556 * 17);
	EXPECT_FLOAT_EQ(l_mat[1][3], 6284 * 17);

	EXPECT_FLOAT_EQ(l_mat[2][0], 1110 * 17);
	EXPECT_FLOAT_EQ(l_mat[2][1], 2854 * 17);
	EXPECT_FLOAT_EQ(l_mat[2][2], 4598 * 17);
	EXPECT_FLOAT_EQ(l_mat[2][3], 6342 * 17);

	EXPECT_FLOAT_EQ(l_mat[3][0], 1120 * 17);
	EXPECT_FLOAT_EQ(l_mat[3][1], 2880 * 17);
	EXPECT_FLOAT_EQ(l_mat[3][2], 4640 * 17);
	EXPECT_FLOAT_EQ(l_mat[3][3], 6400 * 17);
}

TEST(Matrix4x4, MultiplyOperatorVector)
{
	float l_elements[] = {
		1, 2, 3, 4,		5, 6, 7, 8,   9,	10, 11, 12,   13, 14, 15, 16,
	};

	matrix4x4 l_mat(l_elements);
	vector4 l_vec(12, 23, 34, 45);

	vector4 l_result = l_mat * l_vec;

	EXPECT_FLOAT_EQ(l_result.x, 1 * 12 + 2 * 23 + 3 * 34 + 4 * 45);
	EXPECT_FLOAT_EQ(l_result.y, 5 * 12 + 6 * 23 + 7 * 34 + 8 * 45);
	EXPECT_FLOAT_EQ(l_result.z, 9 * 12 + 10 * 23 + 11 * 34 + 12 * 45);
	EXPECT_FLOAT_EQ(l_result.w, 13 * 12 + 14 * 23 + 15 * 34 + 16 * 45);
}


TEST(Matrix4x4, Decompose)
{
    float l_elements[] =
    {
		17.042537788872835f,-37.54389958533337f, 79.19539916473506f, 12.0f,
	  40.149702969556785f, 72.61556520841978f, 23.6995169319019f, 23.0f,
	  -64.66493065929326f, 35.1914093691648f, 35.58681846320036f, 34.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };

	matrix4x4 l_mat(l_elements);
	vector3 l_pos, l_scale;
  quaternion l_rot;
	l_mat.decompose(l_pos, l_rot, l_scale);

    EXPECT_FLOAT_EQ(l_pos.x, 12.0f);
    EXPECT_FLOAT_EQ(l_pos.y, 23.0f);
    EXPECT_FLOAT_EQ(l_pos.z, 34.0f);


    quaternion q = toQuaternion({ 45, 56, 67 });
    EXPECT_FLOAT_EQ(l_rot.x, q.x);
    EXPECT_FLOAT_EQ(l_rot.y, q.y);
    EXPECT_FLOAT_EQ(l_rot.z, q.z);
    EXPECT_FLOAT_EQ(l_rot.w, q.w);

    EXPECT_FLOAT_EQ(l_scale.x, 78.0f);
    EXPECT_FLOAT_EQ(l_scale.y, 89.0f);
    EXPECT_FLOAT_EQ(l_scale.z, 90.0f);
}

TEST(Matrix4x4, DecomposeTranslation)
{
	// TODO
	ASSERT_EQ(true, true);
}

TEST(Matrix4x4, Build)
{

	matrix4x4 l_mat = matrix4x4::build(
        vector3(12, 23, 34), toQuaternion({ 45, 56, 67 }), vector3(78, 89, 90));

	EXPECT_FLOAT_EQ(l_mat[0][0], 17.0425398f);
	EXPECT_FLOAT_EQ(l_mat[0][1],-37.5439024f);
	EXPECT_FLOAT_EQ(l_mat[0][2], 79.195401f);
	EXPECT_FLOAT_EQ(l_mat[0][3], 12.0f);

	EXPECT_FLOAT_EQ(l_mat[1][0], 40.1497044f);
	EXPECT_FLOAT_EQ(l_mat[1][1], 72.6155628f);
	EXPECT_FLOAT_EQ(l_mat[1][2], 23.699511f);
	EXPECT_FLOAT_EQ(l_mat[1][3], 23.0f);

	EXPECT_FLOAT_EQ(l_mat[2][0], -64.664925f);
	EXPECT_FLOAT_EQ(l_mat[2][1], 35.1914099f);
	EXPECT_FLOAT_EQ(l_mat[2][2], 35.586819f);
	EXPECT_FLOAT_EQ(l_mat[2][3], 34.0f);

	EXPECT_FLOAT_EQ(l_mat[3][0], 0.0f);
	EXPECT_FLOAT_EQ(l_mat[3][1], 0.0f);
	EXPECT_FLOAT_EQ(l_mat[3][2], 0.0f);
	EXPECT_FLOAT_EQ(l_mat[3][3], 1.0f);
	EXPECT_FLOAT_EQ(l_mat[3][3], 1.0f);
}

TEST(Matrix4x4, Translation)
{
	matrix4x4 l_mat = matrix4x4::translation(vector3(12, 23, 34));

	EXPECT_EQ(l_mat[0], vector4(1, 0, 0, 12));
	EXPECT_EQ(l_mat[1], vector4(0, 1, 0, 23));
	EXPECT_EQ(l_mat[2], vector4(0, 0, 1, 34));
	EXPECT_EQ(l_mat[3], vector4(0, 0, 0, 1));
}

TEST(Matrix4x4, RotationAxis)
{
	matrix4x4 l_mat = matrix4x4::rotationAxis(vector3(12, 23, 34), 45);

	EXPECT_FLOAT_EQ(l_mat[0][0], 0.5626941231043601f);
	EXPECT_FLOAT_EQ(l_mat[0][1], -0.6048460658910783f);
	EXPECT_FLOAT_EQ(l_mat[0][2], 0.5635038246541906f);
	EXPECT_FLOAT_EQ(l_mat[0][3], 0.0f);

	EXPECT_FLOAT_EQ(l_mat[1][0], 0.748105913989828f);
	EXPECT_FLOAT_EQ(l_mat[1][1], 0.6626126765790316f);
	EXPECT_FLOAT_EQ(l_mat[1][2], -0.0358047802704606f);
	EXPECT_FLOAT_EQ(l_mat[1][3], 0.0f);

	EXPECT_FLOAT_EQ(l_mat[2][0],-0.3517283970299519f);
	EXPECT_FLOAT_EQ(l_mat[2][1],0.4417076832169181f	);
	EXPECT_FLOAT_EQ(l_mat[2][2],0.8253371779520678f	);
	EXPECT_FLOAT_EQ(l_mat[2][3],0.0f								);

	EXPECT_FLOAT_EQ(l_mat[3][0], 0.0f);
	EXPECT_FLOAT_EQ(l_mat[3][1], 0.0f);
	EXPECT_FLOAT_EQ(l_mat[3][2], 0.0f);
	EXPECT_FLOAT_EQ(l_mat[3][3], 1.0f);
}

TEST(Matrix4x4, ComputeProjectionMatrix)
{
	float l_angle = glm::radians(33.0f);
	float l_aspect = 1.23f;
	float l_near = 0.84f;
	float l_far = 10.5f;
	matrix4x4 l_mat = matrix4x4::computeProjMatrix(l_angle, l_aspect, l_near, l_far, false);

	// equations from CS250 Notes #04 NDC slide 19
	EXPECT_FLOAT_EQ(l_mat[0][0], 1.0f / tan(l_angle / 2.0f));
	EXPECT_FLOAT_EQ(l_mat[0][1], 0);
	EXPECT_FLOAT_EQ(l_mat[0][2], 0);
	EXPECT_FLOAT_EQ(l_mat[0][3], 0);

	EXPECT_FLOAT_EQ(l_mat[1][0], 0);
	EXPECT_FLOAT_EQ(l_mat[1][1], l_aspect * 1.0f / tan(l_angle / 2.0f));
	EXPECT_FLOAT_EQ(l_mat[1][2], 0);
	EXPECT_FLOAT_EQ(l_mat[1][3], 0);

	EXPECT_FLOAT_EQ(l_mat[2][0], 0);
	EXPECT_FLOAT_EQ(l_mat[2][1], 0);
	EXPECT_FLOAT_EQ(l_mat[2][2], (l_near + l_far) / (l_near - l_far));
	EXPECT_FLOAT_EQ(l_mat[2][3], 2 * l_near * l_far / (l_near - l_far));

	EXPECT_FLOAT_EQ(l_mat[3][0], 0);
	EXPECT_FLOAT_EQ(l_mat[3][1], 0);
	EXPECT_FLOAT_EQ(l_mat[3][2], -1.0f);
	EXPECT_FLOAT_EQ(l_mat[3][3], 0);
}

TEST(Matrix4x4, Scale)
{
	matrix4x4 l_mat = matrix4x4::scale(vector3(12, 23, 34));

	ASSERT_EQ(l_mat[0], vector4(12.0f, 0, 0, 0));
	ASSERT_EQ(l_mat[1], vector4(0, 23.0f, 0, 0));
	ASSERT_EQ(l_mat[2], vector4(0, 0, 34.0f, 0));
	ASSERT_EQ(l_mat[3], vector4(0,0,0,1));

	l_mat = matrix4x4::scale(vector3(0, 0, 0));

	ASSERT_EQ(l_mat[0], vector4(1.0f, 0, 0, 0));
	ASSERT_EQ(l_mat[1], vector4(0, 1.0f, 0, 0));
	ASSERT_EQ(l_mat[2], vector4(0, 0, 1.0f, 0));
	ASSERT_EQ(l_mat[3], vector4(0, 0, 0, 1.0f));

	matrix4x4 l_mat2 = matrix4x4::scale(vector3(2, 3, 4));
	float l_arr[] = { 1.0f, 1.0f, 1.0f, 1.0f,
					  2.0f, 2.0f, 2.0f, 2.0f,
					  3.0f, 3.0f, 3.0f, 3.0f,
					  4.0f, 4.0f, 4.0f, 4.0f };
	matrix4x4 l_mat3 = matrix4x4(l_arr);
	matrix4x4 l_mat4 = l_mat2 * l_mat3;
	ASSERT_EQ(l_mat4[0], vector4(2.0f, 2.0f, 2.0f, 2.0f));
	ASSERT_EQ(l_mat4[1], vector4(6.0f, 6.0f, 6.0f, 6.0f));
	ASSERT_EQ(l_mat4[2], vector4(12.0f, 12.0f, 12.0f, 12.0f));
	ASSERT_EQ(l_mat4[3], vector4(4.0f, 4.0f, 4.0f, 4.0f));
}

TEST(Matrix4x4, Inverse)
{
	const float l_elements[] = {
		1, 2, -3, 4,
		-4, 5, 6, 8,
		3, -5, 7, 34,
		-4, -13, -834, 1,
	};
	const matrix4x4 l_mat(l_elements);

	const matrix4x4 l_mat2 = l_mat.inverse();

	EXPECT_FLOAT_EQ(l_mat2[0][0], 0.3684524999255784f);
	EXPECT_FLOAT_EQ(l_mat2[0][1], -0.1596469442518935f);
	EXPECT_FLOAT_EQ(l_mat2[0][2],-0.0057091946739699f);
	EXPECT_FLOAT_EQ(l_mat2[0][3], -0.0025218267721893f);

	EXPECT_FLOAT_EQ(l_mat2[1][0], 0.2850940050266429f);
	EXPECT_FLOAT_EQ(l_mat2[1][1], 0.0401536060421779f);
	EXPECT_FLOAT_EQ(l_mat2[1][2], -0.0429561083067188f);
	EXPECT_FLOAT_EQ(l_mat2[1][3], -0.0010971860155562f);

	EXPECT_FLOAT_EQ(l_mat2[2][0], -0.0061982504560977f);
	EXPECT_FLOAT_EQ(l_mat2[2][1],0.0001637273705384f);
	EXPECT_FLOAT_EQ(l_mat2[2][2], 0.0007250783552416f);
	EXPECT_FLOAT_EQ(l_mat2[2][3], -0.0011694812181316f);

	EXPECT_FLOAT_EQ(l_mat2[3][0], 0.0106911846632107f);
	EXPECT_FLOAT_EQ(l_mat2[3][1], 0.0199577285697883f);
	EXPECT_FLOAT_EQ(l_mat2[3][2], 0.0234491615882831f);
	EXPECT_FLOAT_EQ(l_mat2[3][3], 0.0003019387872267f);

	const matrix4x4 l_ident = l_mat * l_mat2;

	EXPECT_NEAR(l_ident[0][0], 1.0f, 0.000001f);
	EXPECT_NEAR(l_ident[0][1], 0.0f, 0.000001f);
	EXPECT_NEAR(l_ident[0][2], 0.0f, 0.000001f);
	EXPECT_NEAR(l_ident[0][3], 0.0f, 0.000001f);

	EXPECT_NEAR(l_ident[1][0], 0.0f, 0.000001f);
	EXPECT_NEAR(l_ident[1][1], 1.0f, 0.000001f);
	EXPECT_NEAR(l_ident[1][2], 0.0f, 0.000001f);
	EXPECT_NEAR(l_ident[1][3], 0.0f, 0.000001f);

	EXPECT_NEAR(l_ident[2][0], 0.0f, 0.000001f);
	EXPECT_NEAR(l_ident[2][1], 0.0f, 0.000001f);
	EXPECT_NEAR(l_ident[2][2], 1.0f, 0.000001f);
	EXPECT_NEAR(l_ident[2][3], 0.0f, 0.000001f);

	EXPECT_NEAR(l_ident[3][0], 0.0f, 0.000001f);
	EXPECT_NEAR(l_ident[3][1], 0.0f, 0.000001f);
	EXPECT_NEAR(l_ident[3][2], 0.0f, 0.000001f);
	EXPECT_NEAR(l_ident[3][3], 1.0f, 0.000001f);

}

TEST(Matrix4x4, ArrayConversion)
{
	float l_elements[] =
	{
		12,23,34,45,
		56,67,78,89,
		112,123,134,145,
		156,167,178,189,
	};

	matrix4x4 l_mat(l_elements);
	float * l_ptr = l_mat.toFloatPtr();
	for(int i = 0; i < 16; ++i)
	{
		EXPECT_EQ(l_elements[i], l_ptr[i]);
	}

	EXPECT_EQ(l_mat[0][0], 12);
	EXPECT_EQ(l_mat[1][0], 23);
	EXPECT_EQ(l_mat[2][0], 34);
	EXPECT_EQ(l_mat[3][0], 45);

	EXPECT_EQ(l_mat[0][1], 56);
	EXPECT_EQ(l_mat[1][1], 67);
	EXPECT_EQ(l_mat[2][1], 78);
	EXPECT_EQ(l_mat[3][1], 89);

	EXPECT_EQ(l_mat[0][2], 112);
	EXPECT_EQ(l_mat[1][2], 123);
	EXPECT_EQ(l_mat[2][2], 134);
	EXPECT_EQ(l_mat[3][2], 145);

	EXPECT_EQ(l_mat[0][3], 156);
	EXPECT_EQ(l_mat[1][3], 167);
	EXPECT_EQ(l_mat[2][3], 178);
	EXPECT_EQ(l_mat[3][3], 189);

}

TEST(Matrix4x4, StringConversion)
{
	float l_elements[] =
	{
		12,23,34,45, 56,67,78,89,  112,123,134,145,  156,167,178,189,
	};

	matrix4x4 l_mat(l_elements);
	std::string l_str = l_mat.toStringPtr();

	ASSERT_STREQ(l_str.c_str(), "|12 23 34 45|\n|56 67 78 89|\n|112 123 134 145|\n|156 167 178 189|");
}

TEST(Matrix4x4, IdentityMatrix)
{
	matrix4x4 l_mat = matrix4x4().GetIdentitiyMatrix();

	ASSERT_EQ(l_mat[0], vector4(1, 0, 0, 0));
	ASSERT_EQ(l_mat[1], vector4(0, 1, 0, 0));
	ASSERT_EQ(l_mat[2], vector4(0, 0, 1, 0));
	ASSERT_EQ(l_mat[3], vector4(0, 0, 0, 1));
}

TEST(Matrix4x4, toQuaternion)
{
    float l_elements[] = {
    0.2184941f, 0.5147398f, -0.8290375f, 0,
    -0.4218416f, 0.8159052f, 0.3954091f, 0,
    0.8799489f, 0.2633280f, 0.3954091f, 0,
    0, 0, 0, 1,
    };
    matrix4x4 l_mat(l_elements);

   quaternion q =  l_mat.toQuaternion();

   EXPECT_FLOAT_EQ(q.x, -0.042366702869941174f);
   EXPECT_FLOAT_EQ(q.y, -0.5481792551513459f);
   EXPECT_FLOAT_EQ(q.z, -0.30042046808599804f);
   EXPECT_FLOAT_EQ(q.w, 0.7793921349359384f);


   const matrix4x4 l_mat1 = matrix4x4::rotationAxis(vector3(1, 0, 0), glm::radians(-35.0f));
   quaternion l_q1 = l_mat1.toQuaternion();
   EXPECT_FLOAT_EQ(l_q1.x, 0.3007058f);
   EXPECT_FLOAT_EQ(l_q1.y, 0.0f);
   EXPECT_FLOAT_EQ(l_q1.z, 0.0f);
   EXPECT_FLOAT_EQ(l_q1.w, 0.953717f);

   const matrix4x4 l_mat2 = matrix4x4::rotationAxis(vector3(0, 1, 0), glm::radians(-35.0f));
   quaternion l_q2 = l_mat2.toQuaternion();
   EXPECT_FLOAT_EQ(l_q2.x, 0.0f);
   EXPECT_FLOAT_EQ(l_q2.y, 0.3007058f);
   EXPECT_FLOAT_EQ(l_q2.z, 0.0f);
   EXPECT_FLOAT_EQ(l_q2.w, 0.953717f);

   const matrix4x4 l_mat3 = matrix4x4::rotationAxis(vector3(0, 0, 1), glm::radians(-35.0f));
   quaternion l_q3 = l_mat3.toQuaternion();
   EXPECT_FLOAT_EQ(l_q3.x, 0.0f);
   EXPECT_FLOAT_EQ(l_q3.y, 0.0f);
   EXPECT_FLOAT_EQ(l_q3.z, 0.3007058f);
   EXPECT_FLOAT_EQ(l_q3.w, 0.953717f);

}

TEST(Matrix4x4, Transpose)
{
	float l_elements1[] ={
		1, 2, 3, 4,  5, 6, 7, 8,  9, 10, 11, 12,  13, 14, 15, 16,
	};
	float l_elements2[] = {
		1, 5, 9, 13,  2, 6, 10, 14,  3, 7, 11, 15,  4, 8, 12, 16,
	};

	matrix4x4 l_mat1(l_elements1);
	matrix4x4 l_mat2(l_elements2);

	EXPECT_EQ(l_mat1.transpose(), l_mat2);
	EXPECT_EQ(l_mat2.transpose(), l_mat1);

	EXPECT_EQ(l_mat1.transpose().transpose(), l_mat1);
	EXPECT_EQ(l_mat2.transpose().transpose(), l_mat2);
}

TEST(Matrix4x4, ComponentVectors)
{
	float l_elements[] = {
		1, 2, 3, 4,  5, 6, 7, 8,  9, 10, 11, 12,   13, 14, 15, 16,
	};

	matrix4x4 l_mat(l_elements);

	EXPECT_EQ(l_mat.right(), vector3(1, 2, 3));
	EXPECT_EQ(l_mat.up(), vector3(5, 6, 7));
	EXPECT_EQ(l_mat.forward(), vector3(9, 10, 11));
	EXPECT_EQ(l_mat.position(), vector3(13, 14, 15));
}

TEST(Matrix4x4, Indexers)
{
	float l_elements[] = {
		1, 2, 3, 4,   5, 6, 7, 8,  10, 11, 12, 13,  14, 15, 16, 17,
	};
	matrix4x4 l_mat(l_elements);

	EXPECT_EQ(l_mat[0], vector4(1, 2, 3, 4));
	EXPECT_EQ(l_mat[1], vector4(5, 6, 7, 8));
	EXPECT_EQ(l_mat[2], vector4(10, 11, 12, 13));
	EXPECT_EQ(l_mat[3], vector4(14, 15, 16, 17));

	const matrix4x4 l_cmat(l_elements);

	EXPECT_EQ(l_cmat[0], vector4(1, 2, 3, 4));
	EXPECT_EQ(l_cmat[1], vector4(5, 6, 7, 8));
	EXPECT_EQ(l_cmat[2], vector4(10, 11, 12, 13));
	EXPECT_EQ(l_cmat[3], vector4(14, 15, 16, 17));
}

TEST(Matrix4x4, EqualityOperator)
{
	const float l_elements[]={
		1, 2, 3, 4,   5, 6, 7, 8,   9, 10, 11, 12,   13, 14, 15, 16,
	};
	matrix4x4 l_mat1(l_elements);
	matrix4x4 l_mat2(l_elements);

	EXPECT_TRUE(l_mat1 == l_mat2);
	EXPECT_FALSE(l_mat1 == l_mat1.transpose());
	EXPECT_TRUE(l_mat1.transpose() == l_mat2.transpose());
	EXPECT_TRUE(matrix4x4() == matrix4x4());
	EXPECT_TRUE(l_mat1 == l_mat1);
}

TEST(Matrix4x4, InequalityOperator)
{
	const float l_elements[]={
		1,2,3,4, 5,6,7,8,  9,10,11,12,  13,14,15,16,
	};
	matrix4x4 l_mat1(l_elements);
	matrix4x4 l_mat2(l_elements);

	EXPECT_FALSE(l_mat1 != l_mat2);
	EXPECT_TRUE(l_mat1.transpose() != l_mat2);
	EXPECT_TRUE(l_mat1 != matrix4x4());
}

