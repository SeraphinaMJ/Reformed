/*!***************************************************************************************
\file       Vector2Tests.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "gtest/gtest.h"
#include <Vector2.hpp>
#include <Vector2Serialization.hpp>

TEST(Vector2, Construction)
{
	vector2 * l_vec = new vector2();
	ASSERT_EQ(l_vec->getX(), 0);
	ASSERT_EQ(l_vec->getY(), 0);
	delete l_vec;

	vector2 l_vec2(1, 2);
	ASSERT_EQ(l_vec2.getX(), 1);
	ASSERT_EQ(l_vec2.getY(), 2);
}

/*TEST(Vector2, GLMVec2Conversion)
{
	vector2 l_vec(glm::vec2(2,3));
	
	ASSERT_EQ(l_vec.getX(), 2);
	ASSERT_EQ(l_vec.getY(), 3);
}*/

TEST(Vector2, TypeRTConversion)
{
	vector2 l_vec(12, 23);
	typeRT l_rt = ::toTypeRT(l_vec);

	ASSERT_EQ(l_rt.getTypeName(), "vector2");
	ASSERT_EQ(l_rt.getVariableName(), "vector2");
	ASSERT_EQ(l_rt.members().size(), 2);
	ASSERT_EQ(l_rt.member("x").getFloat(), 12);
	ASSERT_EQ(l_rt.member("y").getFloat(), 23);
}

TEST(Vector2, TypeRTConstruction)
{
	typeRT l_rt("vector2");
	l_rt.setTypeName("vector2");
	l_rt.insertMember(typeRT("x", 12.0f));
	l_rt.insertMember(typeRT("y", 23.0f));

	vector2 l_vec = ::toVector2(l_rt);
	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
}

TEST(Vector2, SetAndRead)
{
	vector2 l_vec;
	l_vec.setX(12);
	l_vec.setY(23);

	ASSERT_EQ(l_vec.getX(), 12);
	ASSERT_EQ(l_vec.getY(), 23);
}

TEST(Vector2, Offset)
{
	vector2 l_vec(12, 23);

	l_vec.offsetX(10);
	l_vec.offsetY(20);

	ASSERT_EQ(l_vec.getX(), 12 + 10);
	ASSERT_EQ(l_vec.getY(), 23 + 20);

	l_vec.offsetX(-30);
	l_vec.offsetY(-40);

	ASSERT_EQ(l_vec.getX(), 12 + 10 - 30);
	ASSERT_EQ(l_vec.getY(), 23 + 20 - 40);
}

TEST(Vector2, Indexers)
{
	vector2 l_vec(12, 23);

	ASSERT_EQ(l_vec[0], 12);
	ASSERT_EQ(l_vec[1], 23);

	const vector2 & l_cvec = l_vec;

	ASSERT_EQ(l_cvec[0], 12);
	ASSERT_EQ(l_cvec[1], 23);
}

TEST(Vector2, Negate)
{
	vector2 l_vec(12, 23);
	vector2 l_vec2 = -l_vec;

	ASSERT_EQ(l_vec2[0], -1 * 12);
	ASSERT_EQ(l_vec2[1], -1 * 23);
}

TEST(Vector2, PlusEqualsOperator)
{
	vector2 l_vec(12, 23);

	l_vec += vector2(10, 20);

	ASSERT_EQ(l_vec[0], 12 + 10);
	ASSERT_EQ(l_vec[1], 23 + 20);

	l_vec += 7;

	ASSERT_EQ(l_vec[0], 12 + 10 + 7);
	ASSERT_EQ(l_vec[1], 23 + 20 + 7);
}

TEST(Vector2, MinusEqualsOperator)
{
	vector2 l_vec(12, 23);

	l_vec -= vector2(10, 20);

	ASSERT_EQ(l_vec[0], 12 - 10);
	ASSERT_EQ(l_vec[1], 23 - 20);

	l_vec -= 7;

	ASSERT_EQ(l_vec[0], 12 - 10 - 7);
	ASSERT_EQ(l_vec[1], 23 - 20 - 7);
}

TEST(Vector2, MultiplyEqualsOperator)
{
	vector2 l_vec(12, 23);

	l_vec *= vector2(10, 20);

	ASSERT_EQ(l_vec[0], 12 * 10);
	ASSERT_EQ(l_vec[1], 23 * 20);

	l_vec *= 7;

	ASSERT_EQ(l_vec[0], 12 * 10 * 7);
	ASSERT_EQ(l_vec[1], 23 * 20 * 7);
}

TEST(Vector2, DivideEqualsOperator)
{
	vector2 l_vec(12, 23);

	l_vec /= vector2(11, 13);

	ASSERT_EQ(l_vec[0], 12 / 11.0f);
	ASSERT_EQ(l_vec[1], 23 / 13.0f);

	l_vec /= 7;

	ASSERT_EQ(l_vec[0], 12 / 11.0f / 7.0f);
	ASSERT_EQ(l_vec[1], 23 / 13.0f / 7.0f);
}

TEST(Vector2, PlusOperator)
{
	vector2 l_vec = vector2(12, 23) + vector2(10, 20);

	ASSERT_EQ(l_vec[0], 12 + 10);
	ASSERT_EQ(l_vec[1], 23 + 20);

	l_vec = vector2(12, 23) + 17.0f;

	ASSERT_EQ(l_vec[0], 12 + 17);
	ASSERT_EQ(l_vec[1], 23 + 17);
}

TEST(Vector2, MinusOperator)
{
	vector2 l_vec = vector2(12, 23) - vector2(10, 20);

	ASSERT_EQ(l_vec[0], 12 - 10);
	ASSERT_EQ(l_vec[1], 23 - 20);

	l_vec = vector2(12, 23) - 17.0f;

	ASSERT_EQ(l_vec[0], 12 - 17);
	ASSERT_EQ(l_vec[1], 23 - 17);
}

TEST(Vector2, MultiplyOperator)
{
	vector2 l_vec = vector2(12, 23) * vector2(10, 20);

	ASSERT_EQ(l_vec[0], 12 * 10);
	ASSERT_EQ(l_vec[1], 23 * 20);

	l_vec = vector2(12, 23) * 17.0f;

	ASSERT_EQ(l_vec[0], 12 * 17);
	ASSERT_EQ(l_vec[1], 23 * 17);
}

TEST(Vector2, DivideOperator)
{
	vector2 l_vec = vector2(12, 23) / vector2(10, 20);

	ASSERT_EQ(l_vec[0], 12 / 10.0f);
	ASSERT_EQ(l_vec[1], 23 / 20.0f);

	l_vec = vector2(12, 23) / 17.0f;

	ASSERT_EQ(l_vec[0], 12 / 17.0f);
	ASSERT_EQ(l_vec[1], 23 / 17.0f);
}

TEST(Vector2, Distance)
{
	vector2 l_vec(12, 23);

	ASSERT_EQ(l_vec.distance(), 25.942243542146f);
}

TEST(Vector2, DistanceSquared)
{
	vector2 l_vec(12, 23);

	ASSERT_EQ(l_vec.distanceSquared(), 673.0f);
}

TEST(Vector2, DotProduct)
{
	vector2 l_vec1(12, 23);

	float l_result = vector2(-10, -20).dotP(l_vec1);

	ASSERT_EQ(l_result, -580.0f);
}

TEST(Vector2, Normalize)
{
	vector2 l_vec(12, 23);

	l_vec.normalized();
	ASSERT_FLOAT_EQ(l_vec[0], 0.46256600669501f);
	ASSERT_FLOAT_EQ(l_vec[1], 0.88658484616544f);
}

TEST(Vector2, StringConversion)
{
	vector2 l_vec(12, 23);

	std::string l_str = l_vec.toStringPtr();

	ASSERT_EQ(l_str, "(12, 23)");
}

TEST(Vector2, UnitVectors)
{
	ASSERT_EQ(vector2::UnitX[0], 1);
	ASSERT_EQ(vector2::UnitX[1], 0);

	ASSERT_EQ(vector2::UnitY[0], 0);
	ASSERT_EQ(vector2::UnitY[1], 1);
}
