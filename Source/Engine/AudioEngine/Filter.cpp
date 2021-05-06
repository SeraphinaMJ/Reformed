/*!***************************************************************************************
\file       Filter.cpp
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Implementation of all default filter objects
*****************************************************************************************/
#include "Filter.h"
#include "DisplayPrimer.h"
#include "Serializer.h"

#include <cmath>

// Create a constant for PI used by filter coefficient calculations
double PI = 4.0 * std::atan(1.0);

namespace Audio
{
	// Static Members
	std::string LowPass::filterType = "LowPass";

	// Class implementations //

	LowPass::LowPass(float cutoffFrequency_p, float sampleRate_p)
		: cutoffFrequency(cutoffFrequency_p), sampleRate(sampleRate_p)
	{
		recalculate();
	}

	// Default deconstructor no memory allocated by this class
	LowPass::~LowPass() = default;

	void LowPass::initFromArray(char *initData)
	{
		cutoffFrequency = *reinterpret_cast<float*>(initData);
		recalculate();
	}

	// Run the provided buffer through the filter inplace
	void LowPass::process(float* dataBuffer_p, int frames_p)
	{
		for (int i = 0; i < frames_p; ++i)
		{
			float output = inputCoeff * (*dataBuffer_p + inputDelayL);
			inputDelayL = *dataBuffer_p;
			*dataBuffer_p = output + outputCoeff * outputDelayL;
			outputDelayL = *dataBuffer_p++;

			output = inputCoeff * (*dataBuffer_p + inputDelayR);
			inputDelayR = *dataBuffer_p;
			*dataBuffer_p = output + outputCoeff * outputDelayR;
			outputDelayR = *dataBuffer_p++;
		}
	}

	// Set a new cutoff frequency for the filter and recalculate the coefficients
	void LowPass::set(float cutoffFrequency_p)
	{
		cutoffFrequency = cutoffFrequency_p;
		recalculate();
	}

	// Grab the current cutoff frequency for the filter
	float LowPass::get() const
	{
		return cutoffFrequency;
	}

	// Called by the authoring tool to display and edit the filter. In normal use this won't get
	// called and even if it does it won't do anything.
	void LowPass::editFilter(DisplayPrimer* displayClass)
	{
		// Pass this instance as a derived class back to the display class to
		// call the appropriate audio rendering functions
		displayClass->editLowPass(this);
	}

	// Fill the given character array with the set of values this object needs to be recreated correctly.
	// Don't fill past the arraySize_p mark and return the bytes used
	int LowPass::getSerializeInfo(char* writableData_p, const int /*arraySize_p*/) const
	{
		*reinterpret_cast<float*>(writableData_p) = cutoffFrequency;

		return sizeof(float);
	}

	const std::string& LowPass::getFilterName() const
	{
		return filterType;
	}

	// Recalculate the filter coefficients based on current cutoff and sampleRate information
	void LowPass::recalculate()
	{
		float theta = static_cast<float>(std::tan((PI * cutoffFrequency) / sampleRate));

		inputCoeff = theta / (1.0f + theta);
		outputCoeff = (1.0f - theta) / (1.0f + theta);

		inputDelayL  = inputDelayR = 0.0f;
		outputDelayL = inputDelayR = 0.0f;
	}


} // namespace Audio
