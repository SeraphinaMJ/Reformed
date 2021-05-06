/*!***************************************************************************************
\file       ComponentTypeSetTests.cpp
\author     Aaron Damyen
\date       1/31/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a set of unit tests for the componentTypeSet data type
*****************************************************************************************/

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <gtest/gtest.h>
#include <GameObjectFiltering/ComponentTypeSet.hpp>
//======== Types =======================================================================//
using GameObjectFiltering::componentTypeSet;
using GameObjectFiltering::make_componentTypeSet;
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Unit Tests ============================================================//

TEST(ComponentTypeSet, Construction)
{
	componentTypeSet l_set;

	ASSERT_TRUE(l_set.isEmpty());
}

TEST(ComponentTypeSet, Empty)
{
	ASSERT_TRUE(componentTypeSet::Empty().isEmpty());
}

TEST(ComponentTypeSet, SetTypeAndOperatorEquals)
{
	componentTypeSet l_left, l_right;

	ASSERT_TRUE(l_left == l_right);

	l_left.setType(componentType::forConcrete<int>());

	ASSERT_TRUE(l_left != l_right);

	l_right.setType(componentType::forConcrete<int>());

	ASSERT_TRUE(l_left == l_right);

	l_right.setType(componentType::forConcrete<int>());

	ASSERT_TRUE(l_left == l_right);
}

TEST(ComponentTypeSet, IsEmpty)
{
	componentTypeSet l_set;

	ASSERT_TRUE(l_set.isEmpty());

	l_set.setType(componentType::forConcrete<int>());

	ASSERT_FALSE(l_set.isEmpty());
}

TEST(ComponentTypeSet, strongSubsetOf)
{
	componentTypeSet l_left, l_right;

	ASSERT_TRUE(l_left.isEmpty());
	ASSERT_TRUE(l_right.isEmpty());
	ASSERT_TRUE(l_left == l_right);
	ASSERT_FALSE(l_left < l_right);
	ASSERT_FALSE(l_right < l_left);

	l_right.setType(componentType::forConcrete<int>());
	ASSERT_TRUE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_FALSE(l_left == l_right);
	ASSERT_TRUE(l_left < l_right);
	ASSERT_FALSE(l_right < l_left);

	l_left.setType(componentType::forConcrete<int>());
	ASSERT_FALSE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_TRUE(l_left == l_right);
	ASSERT_FALSE(l_left < l_right);
	ASSERT_FALSE(l_right < l_left);

	l_right.setType(componentType::forConcrete<float>());
	ASSERT_FALSE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_FALSE(l_left == l_right);
	ASSERT_TRUE(l_left < l_right);
	ASSERT_FALSE(l_right < l_left);

	ASSERT_TRUE(componentTypeSet::Empty() < l_left);
	ASSERT_TRUE(componentTypeSet::Empty() < l_right);

	ASSERT_FALSE(componentTypeSet::Empty() < componentTypeSet::Empty());
	ASSERT_FALSE(l_left < l_left);
	ASSERT_FALSE(l_right < l_right);
}

TEST(ComponentTypeSet, weakSubsetOf)
{
	componentTypeSet l_left, l_right;

	ASSERT_TRUE(l_left.isEmpty());
	ASSERT_TRUE(l_right.isEmpty());
	ASSERT_TRUE(l_left == l_right);
	ASSERT_TRUE(l_left <= l_right);
	ASSERT_TRUE(l_right <= l_left);

	l_right.setType(componentType::forConcrete<int>());
	ASSERT_TRUE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_FALSE(l_left == l_right);
	ASSERT_TRUE(l_left <= l_right);
	ASSERT_FALSE(l_right <= l_left);

	l_left.setType(componentType::forConcrete<int>());
	ASSERT_FALSE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_TRUE(l_left == l_right);
	ASSERT_TRUE(l_left <= l_right);
	ASSERT_TRUE(l_right <= l_left);

	l_right.setType(componentType::forConcrete<float>());
	ASSERT_FALSE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_FALSE(l_left == l_right);
	ASSERT_TRUE(l_left <= l_right);
	ASSERT_FALSE(l_right <= l_left);

	ASSERT_TRUE(componentTypeSet::Empty() <= l_left);
	ASSERT_TRUE(componentTypeSet::Empty() <= l_right);

	ASSERT_TRUE(componentTypeSet::Empty() <= componentTypeSet::Empty());
	ASSERT_TRUE(l_left <= l_left);
	ASSERT_TRUE(l_right <= l_right);
}

TEST(ComponentTypeSet, HashValue)
{
	componentTypeSet l_set1, l_set2;

	ASSERT_EQ(l_set1.hashValue(), l_set2.hashValue());

	l_set1.setType(componentType::forConcrete<int>());
	ASSERT_FALSE(l_set1.hashValue() == l_set2.hashValue());

	l_set2.setType(componentType::forConcrete<int>());
	ASSERT_TRUE(l_set1.hashValue() == l_set2.hashValue());

	l_set2.setType(componentType::forConcrete<bool>());
	ASSERT_FALSE(l_set1.hashValue() == l_set2.hashValue());
}

TEST(ComponentTypeSet, bitwiseOr)
{
	componentTypeSet l_set1, l_set2, l_set3;

	l_set1.setType(componentType::forConcrete<int>());

	l_set2.setType(componentType::forConcrete<bool>());

	l_set3.setType(componentType::forConcrete<int>());
	l_set3.setType(componentType::forConcrete<bool>());

	ASSERT_TRUE(l_set3 == (l_set1 | l_set2));

	auto l_set4 = l_set1;
	l_set4 |= l_set3;
	ASSERT_TRUE(l_set4 == l_set3 && l_set4 == (l_set1 | l_set2));
}

#include <Transform.hpp>
#include <string>
TEST(ComponentTypeSet, StressTest)
{
	auto stressTestImpl = [&](auto const& t1, auto const& t2)
	{
		using type1 = decltype(t1);
		using type2 = decltype(t2);
		static_assert(!std::is_same_v<type1, type2>, "malformed test");
		componentTypeSet l_set1, l_set2, l_set3;

		const auto c1 = componentType::forConcrete<type1>();
		const auto c2 = componentType::forConcrete<type2>();
		l_set1.setType(c1);
		ASSERT_TRUE(l_set2 < l_set1);
		ASSERT_TRUE(l_set3 < l_set1);

		l_set2.setType(c2);
		ASSERT_TRUE(l_set2 != l_set1);

		l_set3.setType(c1);
		ASSERT_TRUE(l_set1 == l_set3);

		l_set3.setType(c2);
		ASSERT_TRUE(l_set3 == (l_set1 | l_set2));

		auto l_set4 = make_componentTypeSet(c1, c2);
		ASSERT_TRUE(l_set3 == l_set4);
		
		auto l_set5 = make_componentTypeSet();
		ASSERT_FALSE(l_set5 == l_set4);
		ASSERT_FALSE(l_set5 != l_set5);
	};

	stressTestImpl(char(), int());
	stressTestImpl(short(), int());
	stressTestImpl(bool(), int());
	stressTestImpl(std::int64_t(), int());
	stressTestImpl(std::string(), std::wstring());
	stressTestImpl(transform(), std::basic_string<char16_t>());
}

TEST(ComponentTypeSet, conversion_ctor)
{
	componentTypeSet l_set1{ componentType::forConcrete<int>() };
	componentTypeSet l_set2; l_set2.setType(componentType::forConcrete<int>());

	ASSERT_TRUE(l_set1 == l_set2);
}

TEST(ComponentTypeSet, variadic_generator)
{
	componentTypeSet l_set1 = 
		make_componentTypeSet(
			componentType::forConcrete<int>(),
			componentType::forConcrete<bool>()
		);
	componentTypeSet l_set2;
	l_set2.setType(componentType::forConcrete<int>());
	l_set2.setType(componentType::forConcrete<bool>());
}