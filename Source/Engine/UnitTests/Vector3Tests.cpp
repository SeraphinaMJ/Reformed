/*!***************************************************************************************
\file       Vector3Tests.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "gtest/gtest.h"
#include <Vector3.hpp>
#include <Vector3Serialization.hpp>

TEST(Vector3, Construction)
{
	vector3 * l_vec = new vector3();
	ASSERT_EQ(l_vec->getX(), 0);
	ASSERT_EQ(l_vec->getY(), 0);
	ASSERT_EQ(l_vec->getZ(), 0);
	delete l_vec;

	vector3 l_vec2(1, 2, 3);
	ASSERT_EQ(l_vec2.getX(), 1);
	ASSERT_EQ(l_vec2.getY(), 2);
	ASSERT_EQ(l_vec2.getZ(), 3);
}

TEST(Vector3, SetAndRead)
{
	vector3 l_vec;
	l_vec.setX(12);
	l_vec.setY(23);
	l_vec.setZ(34);

	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
	ASSERT_EQ(l_vec.getZ(), 34);
}

TEST(Vector3, Offset)
{
	vector3 l_vec(12, 23, 34);

	l_vec.offsetX(10);
	l_vec.offsetY(10);
	l_vec.offsetZ(10);

	ASSERT_EQ(l_vec.getX(), 12 + 10);
	ASSERT_EQ(l_vec.getY(), 23 + 10);
	ASSERT_EQ(l_vec.getZ(), 34 + 10);

	l_vec.offsetX(-20);
	l_vec.offsetY(-20);
	l_vec.offsetZ(-20);

	ASSERT_EQ(l_vec.getX(), 12 + 10 - 20);
	ASSERT_EQ(l_vec.getY(), 23 + 10 - 20);
	ASSERT_EQ(l_vec.getZ(), 34 + 10 - 20);
}

TEST(Vector3, TypeRTConversion)
{
	vector3 l_vec(12, 23, 34);
	typeRT l_rt = ::toTypeRT(l_vec);

	ASSERT_EQ(l_rt.getTypeName(), "vector3");
	ASSERT_EQ(l_rt.getVariableName(), "vector3");
	ASSERT_EQ(l_rt.members().size(), 3);
	ASSERT_EQ(l_rt.member("x").getFloat(), 12);
	ASSERT_EQ(l_rt.member("y").getFloat(), 23);
	ASSERT_EQ(l_rt.member("z").getFloat(), 34);
}

TEST(Vector3, TypeRTConstruction)
{
	typeRT l_rt("vector3");
	l_rt.setTypeName("vector3");
	l_rt.insertMember(typeRT("x", 12.0f));
	l_rt.insertMember(typeRT("y", 23.0f));
	l_rt.insertMember(typeRT("z", 34.0f));

	vector3 l_vec = ::toVector3(l_rt);
	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
	ASSERT_EQ(l_vec.getZ(), 34);
}

TEST(Vector3, glmvec3Conversion)
{
  vector3 l_glm_vec(12, 23, 34);

	vector3 l_vec(l_glm_vec);
	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
	ASSERT_EQ(l_vec.getZ(), 34);
}

TEST(Vector3, glmvec3Assignment)
{
	vector3 l_glm_vec(12, 23, 34);

	vector3 l_vec(0, 0, 0);
	l_vec = l_glm_vec;
	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
	ASSERT_EQ(l_vec.getZ(), 34);
}

TEST(Vector3, Indexers)
{
	vector3 l_vec(12, 23, 34);

	ASSERT_EQ(l_vec[0], 12);
	ASSERT_EQ(l_vec[1], 23);
	ASSERT_EQ(l_vec[2], 34);

	const vector3 & l_cvec = l_vec;

	ASSERT_EQ(l_cvec[0], 12);
	ASSERT_EQ(l_cvec[1], 23);
	ASSERT_EQ(l_cvec[2], 34);
}

TEST(Vector3, Equality)
{
	vector3 l_vec(12.34f, 23.45f, 34.56f);
	vector3 l_vec2(12.34f, 23.45f, 34.56f);

	ASSERT_EQ(l_vec == l_vec, true);
	ASSERT_EQ(l_vec == l_vec2, true);

	ASSERT_EQ(l_vec == vector3(0, 0, 0), false);
	ASSERT_EQ(vector3(0, 0, 0) == vector3(), true);
	ASSERT_EQ(vector3(1, 0, 0) == vector3(1, 0, 0), true);
	ASSERT_EQ(vector3(0, 1, 0) == vector3(0, 1, 0), true);
	ASSERT_EQ(vector3(0, 0, 1) == vector3(0, 0, 1), true);
}

TEST(Vector3, Inequality)
{
	vector3 l_vec(12.34f, 23.45f, 34.56f);
	vector3 l_vec2(12.34f, 23.45f, 34.56f);

	ASSERT_EQ(l_vec != l_vec, false);
	ASSERT_EQ(l_vec != l_vec2, false);

	ASSERT_EQ(l_vec != vector3(0, 0, 0), true);
	ASSERT_EQ(vector3(0, 0, 0) != vector3(), false);
	ASSERT_EQ(vector3(0, 1, 1) != vector3(0, 1, 1), false);
	ASSERT_EQ(vector3(1, 0, 1) != vector3(1, 0, 1), false);
	ASSERT_EQ(vector3(1, 1, 0) != vector3(1, 1, 0), false);
}

TEST(Vector3, Negate)
{
	vector3 l_vec(12, 23, 34);
	vector3 l_vec2 = -l_vec;

	ASSERT_EQ(l_vec2[0], -1 * 12);
	ASSERT_EQ(l_vec2[1], -1 * 23);
	ASSERT_EQ(l_vec2[2], -1 * 34);
}

TEST(Vector3, PlusEqualsOperator)
{
	vector3 l_vec(12, 23, 34);

	l_vec += vector3(10, 20, 30);

	ASSERT_EQ(l_vec[0], 12 + 10);
	ASSERT_EQ(l_vec[1], 23 + 20);
	ASSERT_EQ(l_vec[2], 34 + 30);

	l_vec += 7;

	ASSERT_EQ(l_vec[0], 12 + 10 + 7);
	ASSERT_EQ(l_vec[1], 23 + 20 + 7);
	ASSERT_EQ(l_vec[2], 34 + 30 + 7);
}

TEST(Vector3, MinusEqualsOperator)
{
	vector3 l_vec(12, 23, 34);

	l_vec -= vector3(10, 20, 30);

	ASSERT_EQ(l_vec[0], 12 - 10);
	ASSERT_EQ(l_vec[1], 23 - 20);
	ASSERT_EQ(l_vec[2], 34 - 30);

	l_vec -= 7;

	ASSERT_EQ(l_vec[0], 12 - 10 - 7);
	ASSERT_EQ(l_vec[1], 23 - 20 - 7);
	ASSERT_EQ(l_vec[2], 34 - 30 - 7);
}

TEST(Vector3, MultiplyEqualsOperator)
{
	vector3 l_vec(12, 23, 34);

	l_vec *= vector3(10, 20, 30);

	ASSERT_EQ(l_vec[0], 12 * 10);
	ASSERT_EQ(l_vec[1], 23 * 20);
	ASSERT_EQ(l_vec[2], 34 * 30);

	l_vec *= 7;

	ASSERT_EQ(l_vec[0], 12 * 10 * 7);
	ASSERT_EQ(l_vec[1], 23 * 20 * 7);
	ASSERT_EQ(l_vec[2], 34 * 30 * 7);
}

TEST(Vector3, DivideEqualsOperator)
{
	vector3 l_vec(12, 23, 34);

	l_vec /= vector3(11, 13, 17);

	ASSERT_EQ(l_vec[0], 12 / 11.0f);
	ASSERT_EQ(l_vec[1], 23 / 13.0f);
	ASSERT_EQ(l_vec[2], 34 / 17.0f);

	l_vec /= 7;

	ASSERT_EQ(l_vec[0], 12 / 11.0f / 7.0f);
	ASSERT_EQ(l_vec[1], 23 / 13.0f / 7.0f);
	ASSERT_EQ(l_vec[2], 34 / 17.0f / 7.0f);
}

TEST(Vector3, PlusOperator)
{
	vector3 l_vec = vector3(12, 23, 34) + vector3(10, 20, 30);

	ASSERT_EQ(l_vec[0], 12 + 10);
	ASSERT_EQ(l_vec[1], 23 + 20);
	ASSERT_EQ(l_vec[2], 34 + 30);

	l_vec = vector3(12, 23, 34) + 17.0f;

	ASSERT_EQ(l_vec[0], 12 + 17);
	ASSERT_EQ(l_vec[1], 23 + 17);
	ASSERT_EQ(l_vec[2], 34 + 17);
}

TEST(Vector3, MinusOperator)
{
	vector3 l_vec = vector3(12, 23, 34) - vector3(10, 20, 30);

	ASSERT_EQ(l_vec[0], 12 - 10);
	ASSERT_EQ(l_vec[1], 23 - 20);
	ASSERT_EQ(l_vec[2], 34 - 30);

	l_vec = vector3(12, 23, 34) - 17.0f;

	ASSERT_EQ(l_vec[0], 12 - 17);
	ASSERT_EQ(l_vec[1], 23 - 17);
	ASSERT_EQ(l_vec[2], 34 - 17);
}

TEST(Vector3, MultiplyOperator)
{
	vector3 l_vec = vector3(12, 23, 34) * vector3(10, 20, 30);

	ASSERT_EQ(l_vec[0], 12 * 10);
	ASSERT_EQ(l_vec[1], 23 * 20);
	ASSERT_EQ(l_vec[2], 34 * 30);

	l_vec = vector3(12, 23, 34) * 17.0f;

	ASSERT_EQ(l_vec[0], 12 * 17);
	ASSERT_EQ(l_vec[1], 23 * 17);
	ASSERT_EQ(l_vec[2], 34 * 17);
}

TEST(Vector3, DivideOperator)
{
	vector3 l_vec = vector3(12, 23, 34) / vector3(10, 20, 30);

	ASSERT_EQ(l_vec[0], 12 / 10.0f);
	ASSERT_EQ(l_vec[1], 23 / 20.0f);
	ASSERT_EQ(l_vec[2], 34 / 30.0f);

	l_vec = vector3(12, 23, 34) / 17.0f;

	ASSERT_EQ(l_vec[0], 12 / 17.0f);
	ASSERT_EQ(l_vec[1], 23 / 17.0f);
	ASSERT_EQ(l_vec[2], 34 / 17.0f);
}

TEST(Vector3, Distance)
{
	vector3 l_vec(12, 23, 34);

	ASSERT_EQ(l_vec.distance(), 42.766809560686f);
}

TEST(Vector3, DistanceSquared)
{
	vector3 l_vec(12, 23, 34);

	ASSERT_EQ(l_vec.distanceSquared(), 1829.0f);
}

TEST(Vector3, DotProduct)
{
	vector3 l_vec1(12, 23, 34);

	float l_result = vector3(-10, -20, -30).dotP(l_vec1);

	ASSERT_EQ(l_result, -1600.0f);
}

TEST(Vector3, Normalize)
{
	vector3 l_vec(12, 23, 34);

	l_vec.normalized();
	ASSERT_FLOAT_EQ(l_vec[0], 0.280591424126f);
	ASSERT_FLOAT_EQ(l_vec[1], 0.537800229576f);
	ASSERT_FLOAT_EQ(l_vec[2], 0.795009035026f);
}

TEST(Vector3, StringConversion)
{
	vector3 l_vec(12, 23, 34);

	std::string l_str = l_vec.toStringPtr();

	ASSERT_EQ(l_str, "(12, 23, 34)");
}

TEST(Vector3, UnitVectors)
{
	ASSERT_EQ(vector3::UnitX[0], 1);
	ASSERT_EQ(vector3::UnitX[1], 0);
	ASSERT_EQ(vector3::UnitX[2], 0);

	ASSERT_EQ(vector3::UnitY[0], 0);
	ASSERT_EQ(vector3::UnitY[1], 1);
	ASSERT_EQ(vector3::UnitY[2], 0);

	ASSERT_EQ(vector3::UnitZ[0], 0);
	ASSERT_EQ(vector3::UnitZ[1], 0);
	ASSERT_EQ(vector3::UnitZ[2], 1);
}