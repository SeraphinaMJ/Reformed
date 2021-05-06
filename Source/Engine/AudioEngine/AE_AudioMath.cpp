/*!***************************************************************************************
\file       AE_AudioMath.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "AE_AudioMath.hpp"

#include <cmath>
#include <random>
#include <functional>

double dBToGain(double p_dB)
{
	return std::pow(10, p_dB / 20.0);
}

float dBToGain(float dB_p)
{
	return static_cast<float>(std::pow(10.0f, dB_p / 20.0f));
}

float dBToGain(int dB_p)
{
	return static_cast<float>(std::pow(10.0f, (float)dB_p / 20.0f));
}

double gainTodB(double p_gain)
{
	return 20.0 * std::log10(p_gain);
}

float gainTodB(float gain_p)
{
	return 20.0f * std::log10(gain_p);
}

float gainTodB(int gain_p)
{
	return 20.0f * std::log10((float)gain_p);
}

float interpolate(float p_start, float p_end, float p_factor)
{
	return (p_end - p_start) * p_factor + p_start;
}

double pitchToSpeedFactor(double p_pitchChange)
{
	double speedupFactor = std::pow(2.0, p_pitchChange / 1200);

	return speedupFactor;
}

float pitchToSpeedFactor(float pitchChange_p)
{
	float speedFactor = std::pow(2.0f, pitchChange_p / 1200.0f);
	return speedFactor;
}

double speedFactorToCents(double speedFactor_p)
{
	double cents = 1200 * std::log2(speedFactor_p);
	return cents;
}

float speedFactorToCents(float speedFactor_p)
{
	float cents = 1200.0f * std::log2f(speedFactor_p);
	return cents;
}

RandomParameter::RandomParameter(int min, int max) : minVal(min), maxVal(max) { }

// TODO: Look at a less garbage way to do this
int RandomParameter::operator()(void)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> range(minVal, maxVal);

	return range(generator);
}
