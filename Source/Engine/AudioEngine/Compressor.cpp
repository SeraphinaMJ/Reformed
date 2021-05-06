/*!***************************************************************************************
\file       Compressor.cpp
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "Compressor.h"
#include "DisplayPrimer.h"
#include "Serializer.h"

namespace Audio
{

	std::string Compressor::DSPTypeName = "Compressor";

	// Public Functions //
	Compressor::Compressor(int ratio_p, float threshold_p, float attackTime_p,
						   float releaseTime_p, float sampleRate_p)
		:  sampleRate(sampleRate_p), ratio(ratio_p), threshold(threshold_p),
		  attackTime(attackTime_p), releaseTime(releaseTime_p)
	{
	}

	Compressor::~Compressor()
	{

	}


	void Compressor::editDSP(DisplayPrimer* displayClass_p)
	{
		if (displayClass_p)
		{
			displayClass_p->editCompressor(this);
		}
	}

	void Compressor::dispatchSerialize(Serializer* serializer_p)
	{
		serializer_p->writeCompressor(this);
	}

	// This is called by the audio thread as part of the signal processing loop
	void Compressor::process(float* /*dataBuffer_p*/, int /*frameCount_p*/)
	{

	}

	// Getters and Setters
	void Compressor::setCompressionRatio(int /*ratio_p*/)
	{

	}

	int Compressor::getCompressionRatio() const
	{
		return ratio;
	}

	void Compressor::setThreshold(float /*threshold_p*/)
	{

	}

	float Compressor::getThreshold() const
	{
		return threshold;
	}

	void Compressor::setAttackReleaseTimes(float /*attackTime_p*/, float /*releaseTime_p*/)
	{

	}

	float Compressor::getAttackTime() const
	{
		return attackTime;
	}

	float Compressor::getReleaseTime() const
	{
		return releaseTime;
	}

	// Utility functions needed to manage DSP's
	const std::string& Compressor::getDSPName() const
	{
		return DSPTypeName;
	}

	// Private Functions //

	void Compressor::recalculate()
	{

	}


} // namespace Audio
