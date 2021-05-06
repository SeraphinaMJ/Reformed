/*!***************************************************************************************
\file       Envelopes.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

namespace Audio {

	enum envelopeState {
		attackState,
		decayState,
		loopingSustainState,
		sustainState,
		releaseState, 
		doneState
	};

	class TrapezoidEnvelope
	{
	public:
		TrapezoidEnvelope();
		TrapezoidEnvelope(float fadeInTime_p, float maxGain_p, float fadeOutTime_p, 
						  const unsigned int& sampleRate, const float& signalTime, 
			              bool looping = false);

		TrapezoidEnvelope(const TrapezoidEnvelope& source);
		TrapezoidEnvelope& operator=(const TrapezoidEnvelope& rhs);

		void initialize(float fadeInTime_p, float maxGain_p, float fadeOutTime_p,
						const float& signalTime, const unsigned int& sampleRate = 48000,
						bool looping = false);

		bool process(float* buffer, unsigned int bufferSize);

		float getMaxGain() const { return maxGain; }
		float getFadeInTime() const { return fadeInTime; }
		float getFadeOutTime() const { return fadeOutTime; }
		float getSignalTime() const { return signalTime; }
		unsigned int getSampleRate() const { return sampleRate; }

		__inline bool getLooping() const { return looping; }

		// be careful with multithreading, what happens if portaudio reads values as we update it?
		// TODO: Make atomics to handle the chance of parallel read/write ops
		__inline void setFadeTimes(float fadeInTime_p, float fadeOutTime_p) {
			fadeInTime = fadeInTime_p;
			fadeOutTime = fadeOutTime_p;
			recalculate();
		}

		__inline void setMaxGain(float maxGain_p) {
			maxGain = maxGain_p; 
			recalculate();
		}

		__inline void setSignalInformation(unsigned int sampleRate_p, float signalTime_p) {
			sampleRate = sampleRate_p;
			signalTime = signalTime_p;
			recalculate();
		}

		__inline void triggerRelease() { 
			currentState = releaseState; 
		} 

		__inline void resetEnvelope() {
			currentState = attackState;
			currentSample = 0;
			currentGain = 0.0f;
		}

		__inline void setLooping(bool looping_p) {
			looping = looping_p;
		}

	protected: // variables to be seen and manipulated through editor build
		float fadeInTime, fadeOutTime;
		float maxGain;

		float signalTime;
		bool looping;
		envelopeState currentState;

		// randomization variables go here
		//bool randomize;
		//float minimumRand, maximumRand;

	private:
		__inline void recalculate() {
			//                gainPerSample =       delta gain /              delta samples
			if(fadeInTime) fadeInIncrement = (maxGain - 0.0f) / (fadeInTime  * sampleRate);
			else fadeInIncrement = 0.0f;

			if(fadeOutTime) fadeOutIncrement = (0.0f - maxGain) / (fadeOutTime * sampleRate);
			else fadeOutIncrement = 0.0f;

			float tempStart = (signalTime - fadeOutTime) * static_cast<float>(sampleRate);
			fadeOutStartSample = static_cast<unsigned int>(tempStart);
		}

		unsigned int sampleRate;


		float currentGain;
		float fadeInIncrement, fadeOutIncrement;
		unsigned int fadeOutStartSample;
		unsigned int currentSample;
	};

} // namespace Audio