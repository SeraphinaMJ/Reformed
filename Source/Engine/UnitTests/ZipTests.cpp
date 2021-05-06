/*!***************************************************************************************
\file       ZipTests.cpp
\author     Tyler Robbins
\date       10/17/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "gtest/gtest.h"
#include "Zip.hpp"

#include <vector>
#include <list>
#include <array>
#include <iterator> // std::advance
#include <type_traits>

TEST(Zip, DefaultConstructor)
{
    Zipper<std::vector<int>> zipper;
}

TEST(Zip, OneVector)
{
    std::vector<int> vec1;
    vec1.push_back(5);
    vec1.push_back(4);
    vec1.push_back(3);
    vec1.push_back(2);
    vec1.push_back(1);
    vec1.push_back(0);
    vec1.push_back(-1);

    auto zipper = zip(vec1);

    auto it = zipper.begin();
    EXPECT_EQ(std::get<0>(*(it++)), 5);
    EXPECT_EQ(std::get<0>(*(it++)), 4);
    EXPECT_EQ(std::get<0>(*(it++)), 3);
    EXPECT_EQ(std::get<0>(*(it++)), 2);
    EXPECT_EQ(std::get<0>(*(it++)), 1);
    EXPECT_EQ(std::get<0>(*(it++)), 0);
    EXPECT_EQ(std::get<0>(*(it++)), -1);
    EXPECT_EQ(it, zipper.end());
}

TEST(Zip, VectorAndList)
{
    std::vector<int> vec1;
    vec1.push_back(5);
    vec1.push_back(4);
    vec1.push_back(3);
    vec1.push_back(2);
    vec1.push_back(1);
    vec1.push_back(0);
    vec1.push_back(-1);

    std::list<float> list1;
    list1.push_front(6);
    list1.push_front(7);
    list1.push_front(8);
    list1.push_front(9);
    list1.push_front(10);
    list1.push_front(11);

    auto zipper = zip(vec1, list1);
    auto it = zipper.begin();

    int idx = 0;
    for(auto lit = list1.begin(); it != zipper.end(); ++it, ++idx, ++lit) {
        EXPECT_EQ(std::get<0>(*it), vec1.at(idx));
        EXPECT_EQ(std::get<1>(*it), *lit);
    }

    EXPECT_EQ(it, zipper.end());
}

TEST(Zip, VectorListAndArray)
{
    std::vector<int> vec1;
    vec1.push_back(5);
    vec1.push_back(4);
    vec1.push_back(3);
    vec1.push_back(2);
    vec1.push_back(1);
    vec1.push_back(0);
    vec1.push_back(-1);

    std::list<float> list1;
    list1.push_front(6);
    list1.push_front(7);
    list1.push_front(8);
    list1.push_front(9);
    list1.push_front(10);
    list1.push_front(11);

    std::array<std::string, 5> array1;
    array1[0] = "foo";
    array1[1] = "spam";
    array1[2] = "bar";
    array1[3] = "eggs";
    array1[4] = "aaaaaaaaaaaaaaaaaa";

    auto zipper = zip(vec1, list1, array1);
    auto it = zipper.begin();

    int idx = 0;
    for(auto lit = list1.begin(); it != zipper.end(); ++it, ++idx, ++lit) {
        EXPECT_EQ(std::get<0>(*it), vec1.at(idx));
        EXPECT_EQ(std::get<1>(*it), *lit);
        EXPECT_EQ(std::get<2>(*it), array1.at(idx));
    }

    EXPECT_EQ(it, zipper.end());
}

TEST(Zip, BigAndSmall)
{
    std::vector<int> vec1;
    vec1.push_back(5);
    vec1.push_back(4);
    vec1.push_back(3);
    vec1.push_back(2);
    vec1.push_back(1);
    vec1.push_back(0);
    vec1.push_back(-1);

    std::list<float> list1;
    list1.push_front(6);
    list1.push_front(7);
    list1.push_front(8);
    list1.push_front(9);
    list1.push_front(10);
    list1.push_front(11);

    std::array<std::string, 5> array1;
    array1[0] = "foo";
    array1[1] = "spam";
    array1[2] = "bar";
    array1[3] = "eggs";
    array1[4] = "aaaaaaaaaaaaaaaaaa";

    auto zipper = zip(vec1, list1, array1);
    auto it = zipper.begin();

    int idx = 0;
    for(; it != zipper.end(); ++it, ++idx);

    EXPECT_EQ(idx, array1.size());
}

TEST(Zip, RangeBasedFor)
{
    std::vector<int> vec1;
    vec1.push_back(5);
    vec1.push_back(4);
    vec1.push_back(3);
    vec1.push_back(2);
    vec1.push_back(1);
    vec1.push_back(0);
    vec1.push_back(-1);

    std::array<std::string, 5> array1;
    array1[0] = "foo";
    array1[1] = "spam";
    array1[2] = "bar";
    array1[3] = "eggs";
    array1[4] = "aaaaaaaaaaaaaaaaaa";

    auto zipper = zip(vec1, array1);

    int idx = 0;
    for(auto&& tup : zipper) {
        EXPECT_EQ(std::get<0>(tup), vec1.at(idx));
        EXPECT_EQ(std::get<1>(tup), array1.at(idx));
        ++idx;
    }
}

TEST(Zip, STL_Foreach)
{
    std::vector<int> vec1;
    vec1.push_back(5);
    vec1.push_back(4);
    vec1.push_back(3);
    vec1.push_back(2);
    vec1.push_back(1);
    vec1.push_back(0);
    vec1.push_back(-1);

    std::array<std::string, 5> array1;
    array1[0] = "foo";
    array1[1] = "spam";
    array1[2] = "bar";
    array1[3] = "eggs";
    array1[4] = "aaaaaaaaaaaaaaaaaa";

    auto zipper = zip(vec1, array1);

    int idx = 0;
    std::for_each(zipper.begin(), zipper.end(),
            [&idx, &vec1, &array1](auto&& tuple) {
                EXPECT_EQ(std::get<0>(tuple), vec1.at(idx));
                EXPECT_EQ(std::get<1>(tuple), array1.at(idx));
                ++idx;
            });
}

TEST(Zip, StructuredBinding)
{
    std::vector<int> vec1;
    vec1.push_back(5);
    vec1.push_back(4);
    vec1.push_back(3);
    vec1.push_back(2);
    vec1.push_back(1);
    vec1.push_back(0);
    vec1.push_back(-1);

    std::array<std::string, 5> array1;
    array1[0] = "foo";
    array1[1] = "spam";
    array1[2] = "bar";
    array1[3] = "eggs";
    array1[4] = "aaaaaaaaaaaaaaaaaa";

    int idx = 0;
    for(auto&& [velem, aelem] : zip(vec1, array1)) {
        EXPECT_EQ(velem, vec1.at(idx));
        EXPECT_EQ(aelem, array1.at(idx));
        ++idx;
    }
}

TEST(Zip, ContainerModification)
{
    std::vector<int> vec1;
    vec1.push_back(5);
    vec1.push_back(4);
    vec1.push_back(3);
    vec1.push_back(2);
    vec1.push_back(1);
    vec1.push_back(0);
    vec1.push_back(-1);

    auto zipper = zip(vec1);

    for(auto&& it = zipper.begin(); it != zipper.end(); ++it) {
        ++(std::get<0>(*it));
    }

    auto it = vec1.begin();
    EXPECT_EQ(*(it++), 6);
    EXPECT_EQ(*(it++), 5);
    EXPECT_EQ(*(it++), 4);
    EXPECT_EQ(*(it++), 3);
    EXPECT_EQ(*(it++), 2);
    EXPECT_EQ(*(it++), 1);
    EXPECT_EQ(*(it++), 0);
}

TEST(Zip, ConstTest)
{
    std::vector<int> vec1;
    vec1.push_back(5);
    vec1.push_back(4);
    vec1.push_back(3);
    vec1.push_back(2);
    vec1.push_back(1);
    vec1.push_back(0);
    vec1.push_back(-1);

    const std::vector<int>& vec2 = vec1;

    auto zipper = zip(vec2);
    if constexpr(!std::is_same_v<decltype(std::get<0>(*zipper.begin())), int const &>) {
        FAIL() << "Expected 'int const&', got " << typeid(decltype(std::get<0>(*zipper.begin()))).name() << std::endl;
    }
}

