/*!***************************************************************************************
\file       MathFunctions.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "MathFunctions.hpp"
#include <cmath>

vector3 toEulerAngle(const quaternion& p_q)
{
	//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

    // roll (x-axis rotation)
	const auto getRoll = [&p_q]() noexcept -> float
	{
		const float sinr_cosp = +2.0f * (p_q.w * p_q.x + p_q.y * p_q.z);
		const float cosr_cosp = +1.0f - 2.0f * (p_q.x * p_q.x + p_q.y * p_q.y);
		return atan2(sinr_cosp, cosr_cosp);
	};

    // pitch (y-axis rotation)
	const auto getPitch = [&p_q]() noexcept -> float
	{
		const float sinp = +2.0f * (p_q.w * p_q.y - p_q.z * p_q.x);
		
		if (std::abs(sinp) >= 1.0f)
			return std::copysign(math::c_Pi / 2.0f, sinp);

		return std::asin(sinp);
	};

    // yaw (z-axis rotation)
	const auto getYaw = [&p_q]() noexcept -> float
	{
		const float siny_cosp = +2.0f * (p_q.w * p_q.z + p_q.x * p_q.y);
		const float cosy_cosp = +1.0f - 2.0f * (p_q.y * p_q.y + p_q.z * p_q.z);
		return std::atan2(siny_cosp, cosy_cosp);
	};

	return vector3{
		RadToDeg(getRoll()),
		RadToDeg(getPitch()),
		RadToDeg(getYaw())
	};
}

quaternion toQuaternion(vector3 const& p_eular)  // yaw (Z), pitch (Y), roll (X)
{
    //https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

    // Abbreviations for the various angular functions
    const float cy = std::cos(DegToRad(p_eular.z) * 0.5f);
    const float sy = std::sin(DegToRad(p_eular.z) * 0.5f);
    const float cp = std::cos(DegToRad(p_eular.y) * 0.5f);
    const float sp = std::sin(DegToRad(p_eular.y) * 0.5f);
    const float cr = std::cos(DegToRad(p_eular.x) * 0.5f);
    const float sr = std::sin(DegToRad(p_eular.x) * 0.5f);

	return quaternion{
		cy * cp * sr - sy * sp * cr, // X
		sy * cp * sr + cy * sp * cr, // Y
		sy * cp * cr - cy * sp * sr, // Z
		cy * cp * cr + sy * sp * sr, // W
	};
}

bool epsilonCompare(const float lhs, const float rhs) noexcept
{
	const float difference = std::fabs(rhs - lhs);
	return (std::fabs(difference) <= std::fabs(lhs + rhs) * std::numeric_limits<float>::epsilon())
		| (lhs == rhs) // comparing 0
		| difference <= std::numeric_limits<float>::min(); // difference is denormalized
}



