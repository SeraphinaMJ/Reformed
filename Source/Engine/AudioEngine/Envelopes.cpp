/*!***************************************************************************************
\file       Envelopes.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "Envelopes.h"

#include <utility>
#include <cstring>

namespace Audio {

#define GAIN_EPSILON 0.001f

	TrapezoidEnvelope::TrapezoidEnvelope()
		: fadeInTime(0.0f), fadeOutTime(0.0f), maxGain(1.0f), signalTime(1.0f),
		  looping(false), currentState(attackState),
		  sampleRate(48000), currentGain(0.0f), currentSample(0)
	{
		recalculate();
	}

	TrapezoidEnvelope::TrapezoidEnvelope(float fadeInTime_p, float maxGain_p, float fadeOutTime_p,
		                                 const unsigned int& sampleRate_p, const float& signalTime_p,
		                                 bool looping_p)
		: fadeInTime(fadeInTime_p), fadeOutTime(fadeOutTime_p), maxGain(maxGain_p), signalTime(signalTime_p),
		  looping(looping_p), currentState(attackState),
		  sampleRate(sampleRate_p), currentGain(0.0f), currentSample(0)
	{
		recalculate();
	}

	TrapezoidEnvelope::TrapezoidEnvelope(const TrapezoidEnvelope& source)
		: fadeInTime(source.fadeInTime), fadeOutTime(source.fadeOutTime),
		  maxGain(source.maxGain), signalTime(source.signalTime),
		  looping(source.looping), sampleRate(source.sampleRate)
	{
		resetEnvelope();
		recalculate();
	}

	TrapezoidEnvelope& TrapezoidEnvelope::operator=(const TrapezoidEnvelope& rhs)
	{
		fadeInTime = rhs.fadeInTime;
		fadeOutTime = rhs.fadeOutTime;
		maxGain = rhs.maxGain;
		looping = rhs.looping;
		sampleRate = rhs.sampleRate;
		signalTime = rhs.signalTime;

		resetEnvelope();
		recalculate();

		return *this;
	}

	void TrapezoidEnvelope::initialize(float fadeInTime_p, float maxGain_p, float fadeOutTime_p,
								       const float& signalTime_p, const unsigned int& sampleRate_p,
								       bool looping_p)
	{
		fadeInTime = fadeInTime_p;
		fadeOutTime = fadeOutTime_p;
		maxGain = maxGain_p;
		signalTime = signalTime_p;
		sampleRate = sampleRate_p;
		looping = looping_p;

		recalculate();
	}

	// processes a buffer of audio data according to setup parameters using one large function
	bool TrapezoidEnvelope::process(float* buffer, unsigned int bufferSize)
	{
		unsigned int bufferCounter = 0;

		if (currentState == doneState)
		{
			std::memset(buffer, 0, bufferSize * sizeof(float));
			return true;
		}

		if (currentState == attackState)
		{
			if (fadeInIncrement == 0.0f)
				currentGain = maxGain;

			for (; bufferCounter < bufferSize; ++bufferCounter, ++currentSample)
			{
				// Apply gain and do the necessesary increases to the currentGain
				buffer[bufferCounter] *= currentGain;
				currentGain += fadeInIncrement;
				if (currentGain >= maxGain)
				{
					// Determine what the next state should be
					if (looping) currentState = loopingSustainState;
					else currentState = sustainState;

					break; // move to the next state
				}
			}

			// early termination for ending buffer boundry
			if (bufferCounter == bufferSize)
				return false;
		}

		if (currentState == loopingSustainState)
		{
			currentGain = maxGain; // ensure the correct gain is set before applying it
			for (; bufferCounter < bufferSize; ++bufferCounter)
				buffer[bufferCounter] *= currentGain;
		}
		else if (currentState == sustainState)
		{
			currentGain = maxGain;
			for (; bufferCounter < bufferSize; ++bufferCounter, ++currentSample)
			{
				buffer[bufferCounter] *= currentGain;
				if (currentSample == fadeOutStartSample)
				{
					currentState = releaseState;
					break;
				}
			}
		}

		if (currentState == releaseState)
		{
			if (fadeOutIncrement == 0.0f)
				currentGain = 0.0f;

			for (; bufferCounter < bufferSize; ++bufferCounter)
			{
				buffer[bufferCounter] *= currentGain;
				currentGain += fadeOutIncrement;
				if (currentGain < GAIN_EPSILON)
				{
					currentState = doneState;
					std::memset(buffer + bufferCounter, 0, (bufferSize - bufferCounter) * sizeof(float));
					return true;
				}
			}
		}

		return false;
	}

} // namespace Audio
