/*!***************************************************************************************
\file       Filter.h
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Contains all of the filter definitions used throughout the audio engine as well
			as the base class all filters should inherit from
*****************************************************************************************/
#pragma once

#include <string>
#include <vector>
#include "FilterFactory.h"

namespace Audio
{
	typedef char FilterID;

	class DisplayPrimer;
	class Serializer;

	class FilterBase
	{
	public:
		virtual ~FilterBase() = default;

		virtual void initFromArray(char *initData) = 0;

		// It's assumed that these will take interleaved stereo data
		virtual void process(float* buffer_p, int frames_p) = 0;

		virtual void set(float valueOne) = 0;
		virtual float get() const = 0;

		virtual void editFilter(DisplayPrimer* displayClass) = 0;
		virtual int getSerializeInfo(char* writableData_p, int arraySize_p) const = 0;


		virtual const std::string& getFilterName() const = 0;
	};

	template <typename T>
	class FilterTemplate : public FilterBase
	{
	public:
		virtual ~FilterTemplate() = default;

		virtual void initFromArray(char *initData) = 0;

		virtual void process(float* buffer_p, int frames_p) = 0;

		virtual void set(float valueOne) = 0;
		virtual float get() const = 0;

		virtual void editFilter(DisplayPrimer* displayClass) = 0;
		virtual int getSerializeInfo(char* writableData_p, int arraySize_p) const = 0;

		virtual const std::string& getFilterName() const = 0;
		virtual bool isRegistered() const = 0;

	protected:
		static bool registered;
	};

	template <typename T>
	bool FilterTemplate<T>::registered = FilterFactory::getInstance().registerFilter(T::getName(), &createFilterType<T>);

	// Currently implemented as a 1st order Butterworth lowpass
	class LowPass : public FilterTemplate<LowPass>
	{
	public:
		LowPass(float cutoffFrequency_p = 440.f, float sampleRate_p = 48000.f);
		~LowPass() override;

		virtual void initFromArray(char *initData) override;

		// Assumes dataBuffer is stereo interleaved data
		virtual void process(float* dataBuffer, const int frames_p) override;

		virtual void set(float cutoffFreq) override;
		virtual float get() const override;
		virtual const std::string& getFilterName() const override;

		virtual void editFilter(DisplayPrimer* displayClass) override;
		virtual int getSerializeInfo(char* writableData_p, int arraySize_p) const override;

		virtual bool isRegistered() const override { return registered; }
		static std::string getName() { return "LowPass"; }

	private:
		void recalculate();

		float cutoffFrequency;
		float sampleRate;

		static std::string filterType;

		// Filter coefficients needed for filter equation
		float inputCoeff, outputCoeff;
		// Delay lines needed **per channel**
		float inputDelayL, inputDelayR, outputDelayL, outputDelayR;
	};

	// Implemented as 1st order Butterworth highpass
	class HighPass : public FilterBase
	{
	public:


	private:

	};

	class BandPass : public FilterBase
	{
	public:

	private:

	};

} // namespace Audio