/*!***************************************************************************************
\file       MathFunctions.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include <MathConstants.hpp>
#include <Quaternion.hpp>
#include <Vector3.hpp>

bool epsilonCompare(const float lhs, const float rhs) noexcept;

constexpr float RadToDeg(const float radians) noexcept
{
	return (180.0f / math::c_Pi) * radians;
}

constexpr float DegToRad(float degrees) noexcept
{
	return (math::c_Pi / 180.0f) * degrees;
}

vector3 toEulerAngle(quaternion const& q);

quaternion toQuaternion(vector3 const& p_eular);
