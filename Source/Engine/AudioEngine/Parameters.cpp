/*!***************************************************************************************
\file       Parameters.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "AE_AudioMath.hpp"
#include "Parameters.h"


#include <algorithm>

namespace Audio
{

	pitchParameter::pitchParameter(float value)
		: parameterName("Pitch"), current(0.0f), lastFractional(0.0f),
		  min(-1200.0f), max(1200.0f), randomize(false)
	{
		current = (value > max ? max : (value < min ? min : value));
		recalculate();
	}

	pitchParameter::pitchParameter(const pitchParameter& rhs)
		: parameterName("Name"), current(rhs.current),
		  lastFractional(rhs.lastFractional), min(-1200.0f), max(1200.0f),
		  randomize(rhs.randomize)
	{
		recalculate();
	}

	pitchParameter& pitchParameter::operator=(const pitchParameter& rhs)
	{
		current = rhs.current;
		randomize = rhs.randomize;
		lastFractional = rhs.lastFractional;

		recalculate();

		return *this;
	}

	void pitchParameter::setCurrent(float newValue)
	{
		// sets the new pitch value, clips it to be within the allowed range
		current = (newValue > max ? max : (newValue < min ? min : newValue));
		recalculate();
	}

	float pitchParameter::getCurrent() const
	{
		return current;
	}

	// This function takes the target number of frames. It expects that the input and output buffers are
	// the appropriate sizes. Use the getBufferFrames function to find the appropriate number of input samples
	void pitchParameter::process(float* inputBuffer, float* outputBuffer, const int& frames, const int& channels)
	{
		float fractionalFrameNum = 0.0f;

		for (int frameNum = 0; frameNum < frames; ++frameNum)
		{
			int currentFrame = (int)std::floor(fractionalFrameNum);
			int nextFrame = currentFrame + 1;
			float factor = fractionalFrameNum - static_cast<float>(currentFrame);

			for (int channelNum = 0; channelNum < channels; ++channelNum) // unroll me
			{
				int currentIndex = (currentFrame * channels) + channelNum;
				int nextIndex = (nextFrame * channels) + channelNum;
				float outputVal = interpolate(inputBuffer[currentIndex], inputBuffer[nextIndex], factor);
				outputBuffer[(frameNum * channels) + channelNum] = outputVal;
			}

			fractionalFrameNum += speedupFactor;
		}

		//lastFractional = fractionalFrameNum - std::floor(fractionalFrameNum);
	}

} // namespace Audio
