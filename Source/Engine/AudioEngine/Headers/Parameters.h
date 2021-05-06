/*!***************************************************************************************
\file       Parameters.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include "AE_AudioMath.hpp"
#include <string>
#include <cmath>

namespace Audio {

	class pitchParameter
	{
	public:
		pitchParameter(float value = 0.0f);
		pitchParameter(const pitchParameter& rhs);

		pitchParameter& operator=(const pitchParameter& rhs);

		const std::string& getName() const { return parameterName; }

		void setCurrent(float newValue);
		float getCurrent() const;

		__inline int getBufferFrames(const int& requestedFrames) const
		{
			return ((int)std::floor(speedupFactor * (float)(requestedFrames)) + 1);
		}

		void process(float* inputBuffer, float* outputBuffer, const int& frames, const int& channels);

	protected:
		const std::string parameterName;
		float current;
		float speedupFactor;
		float lastFractional;
		const float min, max;
		bool randomize;

	private:

		__inline void recalculate() { speedupFactor = std::pow(2.0f, current / 1200.0f); }
	};
} // namespace Audio
