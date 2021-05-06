/*!***************************************************************************************
\file       AudioEngine.cpp
\author     Morgen Hyde
\date       9/11/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the Audio Engine's interface.
*****************************************************************************************/
#include "AudioEngine.hpp"
#include "AudioAssetManager.h"
#include "portaudio.h"

#include <iostream>
#include <cstring> // std::memset, std::memcpy

#define FRAMES 256
#define CHANNELS 2
#define SAMPLES (FRAMES * CHANNELS)
#define SAMPLERATE 48000
#define MAX_VOICES 16

namespace Audio
{
	EventManager* eventManager;

	int PACallbackFunc(const void* p_input, void* p_output, unsigned long p_frameCount,
		const PaStreamCallbackTimeInfo* p_timeInfo,
		PaStreamCallbackFlags p_statusFlags, void* p_userData);

	void SendAudioEvent(EventBase* p_incoming_event)
	{
		eventManager->QueueEvent(p_incoming_event);
	}

	AudioEngine::AudioEngine()
		: secondaryOutput(nullptr), EManager(new EventManager()), RManager(new ResourceManager())
	{
		eventManager = EManager;
		allocateData();
	}

	AudioEngine::AudioEngine(const std::filesystem::path &loadPath)
		: secondaryOutput(nullptr), EManager(new EventManager()), RManager(new ResourceManager(loadPath.generic_string()))
	{
		eventManager = EManager;

		allocateData();
	}

	void AudioEngine::onInitialize()
	{
		PaError error = Pa_Initialize();
		//if (error != paNoError)
		//	std::cout << Pa_GetErrorText(error) << std::endl;

		// setup the audio output stream
		PaStreamParameters streamParam;
		streamParam.device = Pa_GetDefaultOutputDevice();
		streamParam.channelCount = CHANNELS;
		streamParam.sampleFormat = paFloat32;
		streamParam.suggestedLatency = Pa_GetDeviceInfo(streamParam.device)->defaultHighOutputLatency;
		//std::cout << streamParam.suggestedLatency << std::endl;
		streamParam.hostApiSpecificStreamInfo = 0;

		Pa_OpenStream(&outStream, 0, &streamParam, SAMPLERATE,
			FRAMES, 0, PACallbackFunc, this);
		error = Pa_StartStream(outStream);
		//if (error)
		//	std::cout << "Error on stream start: " << Pa_GetErrorText(error) << std::endl;

		EManager->RegisterEventCallback<AudioEngine, PlayEvent, &AudioEngine::PlayCallback>(this);
		EManager->RegisterEventCallback<AudioEngine, PauseEvent, &AudioEngine::PauseCallback>(this);
		EManager->RegisterEventCallback<AudioEngine, StopEvent, &AudioEngine::StopCallback>(this);
		EManager->RegisterEventCallback<AudioEngine, LoadEvent, &AudioEngine::LoadCallback>(this);
		EManager->RegisterEventCallback<AudioEngine, IOTargetSwapEvent, &AudioEngine::SetIOTargetCallback>(this);

		EManager->StartParsing();
	}

	void AudioEngine::onUpdate()
	{
		EManager->ParseAllEvents();

		for (int i = bus_Default; i < bus_BusCount; ++i)
			m_AudioBuses[i]->update();
	}

	void AudioEngine::onShutdown()
	{
		// destroy internal data structures, call the shutdown on
		// the eventmanager.

		EManager->StopParsing();

		PaError l_error = Pa_StopStream(outStream);
		//if (l_error != paNoError)
		//	std::cout << Pa_GetErrorText(l_error) << std::endl;

		for (int i = bus_Default; i < bus_BusCount; ++i)
		{
			if(m_AudioBuses[i])
				delete m_AudioBuses[i];
		}

		delete[] m_AudioBuses;

		l_error = Pa_Terminate();
		//if (l_error != paNoError)
		//	std::cout << Pa_GetErrorText(l_error) << std::endl;

#ifdef AE_DEBUG
		RManager->writeFile(debugOutBuffer, SAMPLERATE * 100);
#endif
	}

	// Private Functions
	void AudioEngine::AudioCallback(float* pa_output, unsigned long& pa_frameCount)
	{
		size_t samples = pa_frameCount * CHANNELS;
		std::memset(pa_output, 0, samples * sizeof(float));

		MixBus** busTraversal = m_AudioBuses;
		for (int busCounter = 0; busCounter < bus_BusCount; ++busCounter)
		{
			MixBus* currentBus = *busTraversal++;
			bool emptyBus = false;
			currentBus->getProcessedBuffer(vBufferData, pa_frameCount, &emptyBus);

			if (!emptyBus)
			{
				for (int i = 0; i < samples; ++i)
					*pa_output++ += *vBufferData++;

				pa_output -= samples;
				vBufferData -= samples;
			}

			std::memset(vBufferData, 0, sizeof(float) * samples);
		}

		// This should be exclusively used with the audio editing tool
		// TODO: Use defines or something to remove this from the loop for normal usage
		if (secondaryOutput)
			secondaryOutput->write(pa_output);
	}

	// Bus Interface functions
	// These are called by the tool GUI to get information about private members of the audio engine
	float AudioEngine::getBusGain(AudioBusType busID_p)
	{
		return m_AudioBuses[busID_p]->getGain();
	}

	void AudioEngine::setBusGain(int busID_p, float gain_p)
	{
		m_AudioBuses[busID_p]->setGain(gain_p);
	}

	float AudioEngine::getBusSpeedupFactor(int busID_p)
	{
		return m_AudioBuses[busID_p]->getSpeedFactor();
	}

	void AudioEngine::setBusSpeedupFactor(int busID_p, float speedFactor_p)
	{
		m_AudioBuses[busID_p]->setSpeedFactor(speedFactor_p);
	}

	int AudioEngine::getBusPanning(int busID_p)
	{
		return m_AudioBuses[busID_p]->getPanning();
	}

	void AudioEngine::setBusPanning(int busID_p, int panningAmount_p)
	{
		m_AudioBuses[busID_p]->setPanning(panningAmount_p);
	}


	void AudioEngine::allocateData()
	{
		vBufferData = new float[SAMPLES];
		std::memset(vBufferData, 0, sizeof(float) * SAMPLES);

		m_AudioBuses = new MixBus*[bus_BusCount];

		m_AudioBuses[bus_Default]	   = new MixBus(16, 2, SAMPLES, "Default");
		m_AudioBuses[bus_Music]        = new MixBus(16, 2, SAMPLES, "Music");
		m_AudioBuses[bus_SoundEffects] = new MixBus(16, 2, SAMPLES, "SoundEffects");
		m_AudioBuses[bus_UI]           = new MixBus(16, 2, SAMPLES, "UI");
	}

	// Clean this up later
	void AudioEngine::PlayCallback(PlayEvent* p_event)
	{
		int objectBus = RManager->getBusFromObject(p_event->objectName);
		bool started = m_AudioBuses[objectBus]->startVoice(p_event->objectName, p_event->ObjectID);
		if (started)
			return;

		VoiceBase* newVoice = nullptr;
		if (p_event->audioObjType == Audio::type_audiodata)
		{
			if (p_event->parentName != "NoParent")
			{
				AudioNode* data = RManager->getAudioNode(p_event->parentName);
				if (!data)
					return;

				newVoice = data->createVoice(p_event->ObjectID, p_event->objectName);
			}
			else
			{
				AudioData* data = RManager->getAudioData(p_event->objectName);
				if (!data)
					return;

				newVoice = data->createVoice(p_event->ObjectID);
			}
		}
		else if (p_event->audioObjType == Audio::type_node)
		{
			AudioNode* data = RManager->getAudioNode(p_event->objectName);
			if (!data)
				return;

			newVoice = data->createVoice(p_event->ObjectID);
		}

		if (newVoice)
		{
			m_AudioBuses[objectBus]->addVoice(newVoice);
			newVoice->start();
		}
	}

	void AudioEngine::PauseCallback(PauseEvent* p_event)
	{
		int objectBus = RManager->getBusFromObject(p_event->fileName);
		m_AudioBuses[objectBus]->pauseVoice(p_event->fileName, p_event->ObjectID);
	}

	void AudioEngine::StopCallback(StopEvent* p_event)
	{
		int objectBus = RManager->getBusFromObject(p_event->fileName);
		m_AudioBuses[objectBus]->stopVoice(p_event->fileName, p_event->ObjectID);
	}

	void AudioEngine::LoadCallback(LoadEvent* p_event)
	{
		if(p_event->objType == Audio::type_audiodata)
		{
			RManager->loadAudioData(p_event->objName);
		}
		else if (p_event->objType == Audio::type_node)
		{
			AudioNode* node = RManager->loadAudioNode(p_event->objName);
			(void)(node);
		}
		else
		{
			AudioNode* node = RManager->loadAudioNode(p_event->objName);
			AudioData* data = RManager->loadAudioData(p_event->objName);

			(void)(node);
			(void)(data);
		}
	}

	void AudioEngine::SetIOTargetCallback(IOTargetSwapEvent* swapEvent_p)
	{
		//TODO:
		if (!secondaryOutput)
		{
			secondaryOutput = new DoubleBufferOutput(swapEvent_p->IOTarget, FRAMES * CHANNELS);
		}
	}

	// callback function passed to port audio for sending data
	int PACallbackFunc(const void* p_input, void* p_output, unsigned long p_frameCount,
		const PaStreamCallbackTimeInfo* p_timeInfo,
		PaStreamCallbackFlags p_statusFlags, void* p_userData)
	{
		(void)p_input;
		(void)p_timeInfo;
		(void)p_statusFlags;

		// dataType* type = reinterpret_cast<dataType*>(userData);
		AudioEngine* engine = reinterpret_cast<AudioEngine*>(p_userData);
		engine->AudioCallback(reinterpret_cast<float*>(p_output), p_frameCount);
		return paContinue;
	}

} // namespace Audio
