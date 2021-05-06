/*!***************************************************************************************
\file       AudioEngine.hpp
\author     Morgen Hyde
\date       9/11/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Definition of the Audio Engine's interface.
*****************************************************************************************/
#pragma once

#include "AudioStorage.hpp"
#include "AE_Events.hpp"
#include "Voice.h"
#include "BusInterface.h"
#include "DoubleBuffer.h"

#include <thread>
#include <atomic>
#include <map>
#include <filesystem>

//#define AE_DEBUG 1

typedef void PaStream;

namespace Audio
{
	typedef Voice** VoiceArray;
	typedef VoiceGroup** VGroups;
	typedef MixBus** BusArray;
	typedef AudioNode** NodeArray;
	
	class ResourceManager;

	void SendAudioEvent(EventBase* p_incoming_event);

	class AudioEngine : public BusInterface
	{
	public:

		AudioEngine();
		AudioEngine(const std::filesystem::path& loadPath);
        virtual ~AudioEngine() = default;

		void onInitialize();

		// Should go through and update the voice object's FSM
		void onUpdate();

		void onShutdown();
		
		void AudioCallback(float* pa_output, unsigned long& p_frameCount);

		void postEvent(EventBase* audioEvent) { EManager->QueueEvent(audioEvent); }

		// Interface functions //
		
		// Bus interface
		float getBusGain(AudioBusType busID_p) override;
		void setBusGain(int busID_p, float gain_p) override;

		float getBusSpeedupFactor(int busID_p) override;
		void setBusSpeedupFactor(int busID_p, float speedFactor_p) override;

		int getBusPanning(int busID_p) override;
		void setBusPanning(int busID_p, int panningAmount_p) override;

		// 

	private:
		void allocateData();

		void PlayCallback(PlayEvent* p_event_type);
		void PauseCallback(PauseEvent* p_event_type);
		void StopCallback(StopEvent* p_event_type);
		void LoadCallback(LoadEvent* p_event_type);
		void UnloadCallback(UnloadEvent* p_event_type);

		// For authoring tool use only, shouldn't be used during gameplay
		void SetIOTargetCallback(IOTargetSwapEvent* swapEvent_p);
		
		PaStream* outStream;
		float* vBufferData;

		BusArray m_AudioBuses;

		// For that one thing in MUS where assets could share stuff like envelopes and LFOs
		// Envelope Array
		// LFO Array

		std::atomic<size_t> atomicVoiceCount;
		std::atomic<size_t> atomicVGroupCount;

		DoubleBufferOutput* secondaryOutput;

		EventManager* EManager;
		ResourceManager* RManager;
	};

}// namespace Audio
