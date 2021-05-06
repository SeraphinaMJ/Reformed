/*!***************************************************************************************
\file       AE_AudioMath.hpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#define MAX_VAL(bits) (float((1 << (bits - 1)) - 1))

double dBToGain(double p_dB);
float dBToGain(float dB_p);

double gainTodB(double p_gain);
float gainTodB(float gain_p);

// requires T to support basic multiplication, addition, and subtraction
//template<typename T>
float interpolate(float p_start, float p_end, float p_factor);

// pitch should be passed as cents
// NOTE: 1200 cents is one octave
double pitchToSpeedFactor(double p_pitchChange);
float pitchToSpeedFactor(float pitchChange_p);

double speedFactorToCents(double speedFactor_p);
float speedFactorToCents(float speedFactor_p);

template <typename T>
float* reinterpretAudioData(char* audioData, int numSamples)
{
	float* floatData = new float[numSamples];
	T* rawData = reinterpret_cast<T*>(audioData);
#ifdef _WIN32
# pragma warning(push)
# pragma warning(disable : 4307)
#endif
	float maximumValues = MAX_VAL(sizeof(T) * 8);
#ifdef _WIN32
# pragma warning(pop)
#endif
	for (int i = 0; i < numSamples; ++i)
	{
		floatData[i] = *rawData / maximumValues;
		rawData++;
	}

	return floatData;
}

template <typename T>
T* reinterpretAudioData(float* audioData, int numSamples)
{
	T* integerData = new T[numSamples];
#ifdef _WIN32
# pragma warning(push)
# pragma warning(disable : 4307)
#endif
	float maxValues = MAX_VAL(sizeof(T) * 8);
#ifdef _WIN32
# pragma warning(pop)
#endif
	for (int i = 0; i < numSamples; ++i)
		integerData[i] = static_cast<T>(*(audioData + i) * maxValues);

	return integerData;
}

template <typename T>
const T* reinterpretAudioData(const float* audioData, int numSamples)
{
	T* integerData = new T[numSamples];
#ifdef _WIN32
# pragma warning(push)
# pragma warning(disable : 4307)
#endif
	float maxValues = MAX_VAL(sizeof(T) * 8);
#ifdef _WIN32
#pragma warning(pop)
#endif
	for (int i = 0; i < numSamples; ++i)
		integerData[i] = static_cast<T>(*(audioData + i) * maxValues);

	return integerData;
}

class RandomParameter
{
public:
	RandomParameter(int min, int max);

	void setMin(int min) { minVal = min; }
	void setMax(int max) { maxVal = max; }

	int operator()();

private:

	int minVal;
	int maxVal;
};
