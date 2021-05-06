/*!***************************************************************************************
\file       Compressor.h
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		
*****************************************************************************************/
#pragma once

#include "DSPBase.h"
#include <string>

namespace Audio
{
	class DisplayPrimer;

	// TODO: Check this for thread safety once fully implemented
	
	// TODO: Handle the three problem children:
	//		 - default constructor on this class
	//       - static getName method isn't required anywhere and will just break

	/* What I current require of you (as of 1/28/2019) when you implement a DSP derived class
	 *		- a default constructor
	 *      - a static getName method that returns a string
	 *		- probably a class that takes/gives a DSP initializer structure to handle serialization code
	 */

	class Compressor : public DSPTemplate<Compressor>
	{
	public:
		Compressor() = default;
		Compressor(int ratio_p, float threshold_p, float attackTime_p, float releaseTime_p, float sampleRate = 48000.0f);
		~Compressor();

		virtual void editDSP(DisplayPrimer* displayClass_p) override;
		virtual void dispatchSerialize(Serializer* serializer_p) override;
		virtual void process(float* dataBuffer_p, int frameCount_p) override; // Will be called by audio thread

		// Getters and Setters
		void setCompressionRatio(int ratio_p);
		int getCompressionRatio() const;

		void setThreshold(float threashold_p);
		float getThreshold() const;

		void setAttackReleaseTimes(float attackTime_p, float releaseTime_p);
		float getAttackTime() const;
		float getReleaseTime() const;

		// Utility functions needed to manage DSP's
		virtual const std::string& getDSPName() const override;

		// This is needed by the CRTP class to prevent the compiler from optimizing out a static variable
		// that allows the class to be added to a list before main is called
		virtual bool isRegistered() override { return registered; }
		// Also required by the template base class to give a valid key to the factory
		static const std::string getName() { return "Compressor"; }

	private:
		void recalculate();

		enum CompressorState
		{
			none,
			attack,
			compress,
			release,
		};

		static std::string DSPTypeName;
		float sampleRate;
	
		char currentState;

		int ratio;
		float threshold;

		float attackTime, releaseTime;

	};

} // namespace Audio
