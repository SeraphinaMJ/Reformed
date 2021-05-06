/*!***************************************************************************************
\file       Vector4Tests.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "gtest/gtest.h"
#include <Vector4.hpp>
#include <Vector3.hpp>
#include <Vector4Serialization.hpp>

TEST(Vector4, Construction)
{
	vector4 * l_vec = new vector4();
	ASSERT_EQ(l_vec->getX(), 0);
	ASSERT_EQ(l_vec->getY(), 0);
	ASSERT_EQ(l_vec->getZ(), 0);
	ASSERT_EQ(l_vec->getW(), 0);
	delete l_vec;

	vector4 l_vec2(1, 2, 3, 4);
	ASSERT_EQ(l_vec2.getX(), 1);
	ASSERT_EQ(l_vec2.getY(), 2);
	ASSERT_EQ(l_vec2.getZ(), 3);
	ASSERT_EQ(l_vec2.getW(), 4);
}

TEST(Vector4, SetAndRead)
{
	vector4 l_vec;
	l_vec.setX(12);
	l_vec.setY(23);
	l_vec.setZ(34);
	l_vec.setW(45);

	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
	ASSERT_EQ(l_vec.getZ(), 34);
	ASSERT_EQ(l_vec.getW(), 45);
}

//TEST(Vector4, Offset)
//{
//	vector4 l_vec(12, 23, 34, 45);
//
//	l_vec.offsetX(10);
//	l_vec.offsetY(10);
//	l_vec.offsetZ(10);
//	l_vec.offsetW(10);
//
//	ASSERT_EQ(l_vec.getX(), 12 + 10);
//	ASSERT_EQ(l_vec.getY(), 23 + 10);
//	ASSERT_EQ(l_vec.getZ(), 34 + 10);
//	ASSERT_EQ(l_vec.getW(), 45 + 10);
//
//	l_vec.offsetX(-20);
//	l_vec.offsetY(-20);
//	l_vec.offsetZ(-20);
//	l_vec.offsetW(-20);
//
//	ASSERT_EQ(l_vec.getX(), 12 + 10 - 20);
//	ASSERT_EQ(l_vec.getY(), 23 + 10 - 20);
//	ASSERT_EQ(l_vec.getZ(), 34 + 10 - 20);
//	ASSERT_EQ(l_vec.getW(), 45 + 10 - 20);
//}

TEST(Vector4, TypeRTConversion)
{
	vector4 l_vec(12, 23, 34, 45);
	typeRT l_rt = ::toTypeRT(l_vec);

	ASSERT_EQ(l_rt.getTypeName(), "vector4");
	ASSERT_EQ(l_rt.getVariableName(), "vector4");
	ASSERT_EQ(l_rt.members().size(), 4);
	ASSERT_EQ(l_rt.member("x").getFloat(), 12);
	ASSERT_EQ(l_rt.member("y").getFloat(), 23);
	ASSERT_EQ(l_rt.member("z").getFloat(), 34);
	ASSERT_EQ(l_rt.member("w").getFloat(), 45);
}

TEST(Vector4, TypeRTConstruction)
{
	typeRT l_rt("vector4");
	l_rt.setTypeName("vector4");
	l_rt.insertMember(typeRT("x", 12.0f));
	l_rt.insertMember(typeRT("y", 23.0f));
	l_rt.insertMember(typeRT("z", 34.0f));
	l_rt.insertMember(typeRT("w", 45.0f));

	vector4 l_vec = ::toVector4(l_rt);
	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
	ASSERT_EQ(l_vec.getZ(), 34);
	ASSERT_EQ(l_vec.getW(), 45);
}

//TEST(Vector4, glmvec4Conversion)
//{
//	glm::vec4 l_glm_vec(12, 23, 34, 45);
//
//	vector4 l_vec(l_glm_vec);
//	ASSERT_EQ(l_vec.getX(), 12);
//	ASSERT_EQ(l_vec.getY(), 23);
//	ASSERT_EQ(l_vec.getZ(), 34);
//}

TEST(Vector4, vector3Conversion)
{
	vector4 l_vec(vector3(12, 23, 34), 45);

	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
	ASSERT_EQ(l_vec.getZ(), 34);
	ASSERT_EQ(l_vec.getW(), 45);
}

TEST(Vector4, glmvec4Assignment)
{
	vector4 l_glm_vec(12, 23, 34, 45);

	vector4 l_vec(0, 0, 0, 0);
	l_vec = l_glm_vec;
	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
	ASSERT_EQ(l_vec.getZ(), 34);
	ASSERT_EQ(l_vec.getW(), 45);
}

TEST(Vector4, Indexers)
{
	vector4 l_vec(12, 23, 34, 45);

	ASSERT_EQ(l_vec[0], 12);
	ASSERT_EQ(l_vec[1], 23);
	ASSERT_EQ(l_vec[2], 34);
	ASSERT_EQ(l_vec[3], 45);

	const vector4 & l_cvec = l_vec;

	ASSERT_EQ(l_cvec[0], 12);
	ASSERT_EQ(l_cvec[1], 23);
	ASSERT_EQ(l_cvec[2], 34);
	ASSERT_EQ(l_cvec[3], 45);
}

TEST(Vector4, Equality)
{
	vector4 l_vec(12.34f, 23.45f, 34.56f, 45.67f);
	vector4 l_vec2(12.34f, 23.45f, 34.56f, 45.67f);

	ASSERT_EQ(l_vec == l_vec, true);
	ASSERT_EQ(l_vec == l_vec2, true);

	ASSERT_EQ(l_vec == vector4(0, 0, 0, 0), false);
	ASSERT_EQ(vector4(0, 0, 0, 0) == vector4(), true);
	ASSERT_EQ(vector4(1, 0, 0, 0) == vector4(1, 0, 0, 0), true);
	ASSERT_EQ(vector4(0, 1, 0, 0) == vector4(0, 1, 0, 0), true);
	ASSERT_EQ(vector4(0, 0, 1, 0) == vector4(0, 0, 1, 0), true);
	ASSERT_EQ(vector4(0, 0, 0, 1) == vector4(0, 0, 0, 1), true);
}

TEST(Vector4, Inequality)
{
	vector4 l_vec(12.34f, 23.45f, 34.56f, 45.67f);
	vector4 l_vec2(12.34f, 23.45f, 34.56f, 45.67f);

	ASSERT_EQ(l_vec != l_vec, false);
	ASSERT_EQ(l_vec != l_vec2, false);

	ASSERT_EQ(l_vec != vector4(0, 0, 0, 0), true);
	ASSERT_EQ(vector4(0, 0, 0, 0) != vector4(), false);
	ASSERT_EQ(vector4(0, 1, 1, 1) != vector4(0, 1, 1, 1), false);
	ASSERT_EQ(vector4(1, 0, 1, 1) != vector4(1, 0, 1, 1), false);
	ASSERT_EQ(vector4(1, 1, 0, 1) != vector4(1, 1, 0, 1), false);
	ASSERT_EQ(vector4(1, 1, 1, 0) != vector4(1, 1, 1, 0), false);
}

TEST(Vector4, Negate)
{
	vector4 l_vec(12, 23, 34, 45);
	vector4 l_vec2 = -l_vec;

	ASSERT_EQ(l_vec2[0], -1 * 12);
	ASSERT_EQ(l_vec2[1], -1 * 23);
	ASSERT_EQ(l_vec2[2], -1 * 34);
	ASSERT_EQ(l_vec2[3], -1 * 45);
}

TEST(Vector4, PlusEqualsOperator)
{
	vector4 l_vec(12, 23, 34, 45);

	l_vec += vector4(10, 20, 30, 40);

	ASSERT_EQ(l_vec[0], 12 + 10);
	ASSERT_EQ(l_vec[1], 23 + 20);
	ASSERT_EQ(l_vec[2], 34 + 30);
	ASSERT_EQ(l_vec[3], 45 + 40);

	l_vec += 7;

	ASSERT_EQ(l_vec[0], 12 + 10 + 7);
	ASSERT_EQ(l_vec[1], 23 + 20 + 7);
	ASSERT_EQ(l_vec[2], 34 + 30 + 7);
	ASSERT_EQ(l_vec[3], 45 + 40 + 7);
}

TEST(Vector4, MinusEqualsOperator)
{
	vector4 l_vec(12, 23, 34, 45);

	l_vec -= vector4(10, 20, 30, 40);

	ASSERT_EQ(l_vec[0], 12 - 10);
	ASSERT_EQ(l_vec[1], 23 - 20);
	ASSERT_EQ(l_vec[2], 34 - 30);
	ASSERT_EQ(l_vec[3], 45 - 40);

	l_vec -= 7;

	ASSERT_EQ(l_vec[0], 12 - 10 - 7);
	ASSERT_EQ(l_vec[1], 23 - 20 - 7);
	ASSERT_EQ(l_vec[2], 34 - 30 - 7);
	ASSERT_EQ(l_vec[3], 45 - 40 - 7);
}

TEST(Vector4, MultiplyEqualsOperator)
{
	vector4 l_vec(12, 23, 34, 45);

	l_vec *= vector4(10, 20, 30, 40);

	ASSERT_EQ(l_vec[0], 12 * 10);
	ASSERT_EQ(l_vec[1], 23 * 20);
	ASSERT_EQ(l_vec[2], 34 * 30);
	ASSERT_EQ(l_vec[3], 45 * 40);

	l_vec *= 7;

	ASSERT_EQ(l_vec[0], 12 * 10 * 7);
	ASSERT_EQ(l_vec[1], 23 * 20 * 7);
	ASSERT_EQ(l_vec[2], 34 * 30 * 7);
	ASSERT_EQ(l_vec[3], 45 * 40 * 7);
}

TEST(Vector4, DivideEqualsOperator)
{
	vector4 l_vec(12, 23, 34, 45);

	l_vec /= vector4(11, 13, 17, 19);

	ASSERT_EQ(l_vec[0], 12 / 11.0f);
	ASSERT_EQ(l_vec[1], 23 / 13.0f);
	ASSERT_EQ(l_vec[2], 34 / 17.0f);
	ASSERT_EQ(l_vec[3], 45 / 19.0f);

	l_vec /= 7;

	ASSERT_EQ(l_vec[0], 12 / 11.0f / 7.0f);
	ASSERT_EQ(l_vec[1], 23 / 13.0f / 7.0f);
	ASSERT_EQ(l_vec[2], 34 / 17.0f / 7.0f);
	ASSERT_EQ(l_vec[3], 45 / 19.0f / 7.0f);
}

TEST(Vector4, PlusOperator)
{
	vector4 l_vec = vector4(12, 23, 34, 45) + vector4(10, 20, 30, 40);

	ASSERT_EQ(l_vec[0], 12 + 10);
	ASSERT_EQ(l_vec[1], 23 + 20);
	ASSERT_EQ(l_vec[2], 34 + 30);
	ASSERT_EQ(l_vec[3], 45 + 40);

	l_vec = vector4(12, 23, 34, 45) + 17.0f;

	ASSERT_EQ(l_vec[0], 12 + 17);
	ASSERT_EQ(l_vec[1], 23 + 17);
	ASSERT_EQ(l_vec[2], 34 + 17);
	ASSERT_EQ(l_vec[3], 45 + 17);
}

TEST(Vector4, MinusOperator)
{
	vector4 l_vec = vector4(12, 23, 34, 45) - vector4(10, 20, 30, 40);

	ASSERT_EQ(l_vec[0], 12 - 10);
	ASSERT_EQ(l_vec[1], 23 - 20);
	ASSERT_EQ(l_vec[2], 34 - 30);
	ASSERT_EQ(l_vec[3], 45 - 40);

	l_vec = vector4(12, 23, 34, 45) - 17.0f;

	ASSERT_EQ(l_vec[0], 12 - 17);
	ASSERT_EQ(l_vec[1], 23 - 17);
	ASSERT_EQ(l_vec[2], 34 - 17);
	ASSERT_EQ(l_vec[3], 45 - 17);
}

TEST(Vector4, MultiplyOperator)
{
	vector4 l_vec = vector4(12, 23, 34, 45) * vector4(10, 20, 30, 40);

	ASSERT_EQ(l_vec[0], 12 * 10);
	ASSERT_EQ(l_vec[1], 23 * 20);
	ASSERT_EQ(l_vec[2], 34 * 30);
	ASSERT_EQ(l_vec[3], 45 * 40);

	l_vec = vector4(12, 23, 34, 45) * 17.0f;

	ASSERT_EQ(l_vec[0], 12 * 17);
	ASSERT_EQ(l_vec[1], 23 * 17);
	ASSERT_EQ(l_vec[2], 34 * 17);
	ASSERT_EQ(l_vec[3], 45 * 17);
}

TEST(Vector4, DivideOperator)
{
	vector4 l_vec = vector4(12, 23, 34, 45) / vector4(10, 20, 30, 40);

	ASSERT_EQ(l_vec[0], 12 / 10.0f);
	ASSERT_EQ(l_vec[1], 23 / 20.0f);
	ASSERT_EQ(l_vec[2], 34 / 30.0f);
	ASSERT_EQ(l_vec[3], 45 / 40.0f);

	l_vec = vector4(12, 23, 34, 45) / 17.0f;

	ASSERT_EQ(l_vec[0], 12 / 17.0f);
	ASSERT_EQ(l_vec[1], 23 / 17.0f);
	ASSERT_EQ(l_vec[2], 34 / 17.0f);
	ASSERT_EQ(l_vec[3], 45 / 17.0f);
}

//TEST(Vector4, Distance)
//{
//	vector4 l_vec(12, 23, 34, 45);
//
//	ASSERT_EQ(l_vec.distance(), 62.0805927807);
//}
//
//TEST(Vector4, DistanceSquared)
//{
//	vector4 l_vec(12, 23, 34, 45);
//
//	ASSERT_EQ(l_vec.distanceSquared(), 3854.0f);
//}

//TEST(Vector4, DotProduct)
//{
//	vector4 l_vec1(12, 23, 34, 45);
//
//	float l_result = vector4(-10, -20, -30, -40).dotP(l_vec1);
//
//	ASSERT_EQ(l_result, -3400.0f);
//}

TEST(Vector4, Normalize)
{
	vector4 l_vec(12, 23, 34, 45);

	l_vec = l_vec.normalize();
	ASSERT_FLOAT_EQ(l_vec[0], 0.193297123344f);
	ASSERT_FLOAT_EQ(l_vec[1], 0.370486153076f);
	ASSERT_FLOAT_EQ(l_vec[2], 0.547675182808f);
	ASSERT_FLOAT_EQ(l_vec[3], 0.72486421254f);
}

TEST(Vector4, StringConversion)
{
	vector4 l_vec(12, 23, 34, 45);

	std::string l_str = l_vec.toStringPtr();

	ASSERT_EQ(l_str, "(12, 23, 34, 45)");
}

TEST(Vector4, DotProduct)
{
	const vector4 l_vec1(2, 3, 5, 7);
	const vector4 l_vec2(11, 13, 17, 19);

	//EXPECT_EQ(vector4::dotP(l_vec1, l_vec2), 279.0f);
	//EXPECT_EQ(vector4::dotP(l_vec2, l_vec1), 279.0f);
}