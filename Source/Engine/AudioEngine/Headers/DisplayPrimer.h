/*!***************************************************************************************
\file       DisplayPrimer.h
\author     Morgen Hyde
\date       1/22/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Contains a virtual interface that essentially does nothing. This class is
			meant to be inherited from by the authoring tool to implement the second
			dispatch in the double dispatch pattern to decouple the manipulation of audio
			objects by the authoring tool from the rest of the audio engine.
*****************************************************************************************/
#pragma once

#include "DSPBase.h"

#define VOID_CAST(x) (void)x

namespace Audio 
{
	// Forward declare the classes we want to display //

	// general mixing
	class TrapezoidEnvelope;
	class pitchParameter;

	// filters
	class LowPass;
	class HighPass;
	class BandPass;

	// DSP objects
	class Compressor;
	

	// An abstract class to facilitate any GUI plug ins that want to manipulate the engines data
	class DisplayPrimer
	{
	public:
		DisplayPrimer() = default;
		~DisplayPrimer() = default;

		// General Mixing data functions
		virtual void editEnvelope(TrapezoidEnvelope* tEnvelope_p) { VOID_CAST(tEnvelope_p); }
		virtual void editPitch(pitchParameter* pitch_p) { VOID_CAST(pitch_p); }

		// Filter edit functions 
		virtual void editLowPass(LowPass* lpFilter_p)   { (void)lpFilter_p; }
		virtual void editHighPass(HighPass* hpFilter_p) { (void)hpFilter_p; }
		virtual void editBandPass(BandPass* bpFilter_p) { (void)bpFilter_p; }

		// DSP edit functions
		virtual void editCompressor(Compressor* compressor_p) { (void)compressor_p; }

	private:
		// ??

	};


}