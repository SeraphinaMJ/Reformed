/*!***************************************************************************************
\file       Voice.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include "AudioStorage.hpp"
#include "Parameters.h"
#include "Envelopes.h"

namespace Audio
{
	enum playbackState
	{
		enm_starting,
		enm_playing,
		enm_paused,
		enm_stopping,
		enm_stopped,
	};

	class VoiceBase
	{
	public:
		virtual ~VoiceBase() = default;

		virtual bool updateVoice() = 0;
		virtual void getProcessedBuffer(float* buffer_p, const int& reqFrames_p, bool* finishedFlag) = 0;

		virtual const std::string& getObjectName() const = 0;
		virtual GameObjectID getGameObjectID() const = 0;

		virtual void start() = 0;
		virtual void pause() = 0;
		virtual void stop()  = 0;
	};

	/*!***************************************************************************************
	\par class: Voice
	\brief
	*****************************************************************************************/
	class Voice : public VoiceBase
	{
	public:
		//========Constructors / Deconstructors=====================================================//
		Voice(const std::string& sourceName, GameObjectID ID, const float* buffer,
			  size_t samples_p, int channels, const MixSettings& settings, 
			  FilterBase* filter_p = nullptr/*, DSPBase** dspChain_p */);

		~Voice();

		//========Update Functions==================================================================//
		bool updateVoice() override; // Used by external game thread only

		//========Signal Processing Functions=======================================================//
		void getProcessedBuffer(float* buffer, const int& reqFrames_p, bool* voiceFinished_p) override;

		//========Control Functions=================================================================//
		GameObjectID getGameObjectID() const override { return ownerID; }
		const std::string& getObjectName() const override { return sourceName; }

		void setLooping(bool p_isLooping) { envelope.setLooping(p_isLooping); }

		void start() override {
			voiceState.store(enm_starting);
		}
		void stop() override {
			voiceState.store(enm_stopping, std::memory_order_release);
		}
		void pause() override {
			voiceState.store(enm_paused, std::memory_order_release);
		}

	private:

		const AudioData* source;  //!< pointer to the AudioData source

		// Playback data
		RingBuffer ringData;
		//DoubleBuffer sourceData;

		// information about the audio data
		size_t numChannels;
		size_t numSamples;
		const std::string sourceName;

		// Mixing parameters
		pitchParameter pitchChange;
		int panningAmount;
		TrapezoidEnvelope envelope;

		FilterBase* filter;

		// control parameters
		GameObjectID ownerID;

		// state information
		std::atomic<int> voiceState;
	};

	class VoiceGroup : public VoiceBase
	{
	public:

		VoiceGroup(const std::string groupName_p, int numVoices_p, int channels_p,
			       GameObjectID ID_p, const MixSettings& settings_p, 
				   FilterBase* filter_p = nullptr/*, DSPBase** dspChain_p */);
		
		~VoiceGroup();

		// Adds new voices to the internal buffer, if a pointer is passed in the group will
		// handle that voice's memory
		bool addVoice(Voice* newVoice);
		bool addVoice(const std::string& sourceName_p, const float* buffer_p, size_t samples_p, 
			          int channels_p, const MixSettings& settings_p);

		Voice* removeVoice(const std::string& voiceName_p);

		const std::string& getObjectName() const override { return groupName; }
		GameObjectID getGameObjectID() const override { return parentID; }

		bool updateVoice() override;
		void getProcessedBuffer(float* outputBuffer_p, const int& reqFrames_p, bool* voiceFinished_p) override;

		void start() override;
		void pause() override;
		void stop() override;

		bool startVoice(const std::string& voiceName);
		bool pauseVoice(const std::string& voiceName);
		bool stopVoice(const std::string& voiceName);

	private:
		// Lookup Information
		const std::string groupName;
		GameObjectID parentID;
		
		// Variables used in mixing final buffer
		pitchParameter pitchChange;
		int panning;
		TrapezoidEnvelope envelope;
		FilterBase* filter;

		int channelCount;

		// Voice storage information
		const int maxVoices;
		int activeVoices;
		Voice** vArray;
		// VoiceGroup** groupArray; // Add this in later to handle nested node hierarchies

		// State Information
		std::atomic<int> voiceGroupState;
	};

	class MixBus
	{
	public:
		MixBus(int maxVoices_p, int channels_p, int bufferSize_p, const std::string& BusName_p);
		~MixBus();

		bool addVoice(VoiceBase* newVoice_p);
		bool addVoice(const std::string& sourceName_p, const float* buffer_p, size_t numSamples_p,
					  int channels_p, GameObjectID parentID, const MixSettings& settings_p, 
			          FilterBase* filter_p = nullptr/*, DSPBase** dspChain_p = nullptr */);

		VoiceBase* detatchVoice(const std::string& voiceName_p);

		void update();
		void getProcessedBuffer(float* outputBuffer_p, int requestedFrames_p, bool* emptyBusFlag_p);

		/*!***************************************************************************************
		\brief  This function retrieves the target output bus from a specified audio object.
		\return None
		*****************************************************************************************/
		void startAll();
		void pauseAll();
		void stopAll();

		/*!***************************************************************************************
		\brief  This function starts a specific voice inside the bus
		\param  voiceName_p - The name of the voice to start playing
		\param  objectID_p  - ID of the game object that owns the voice for lookups
		\return a boolean indicating if the voice was successfully started
		*****************************************************************************************/
		bool startVoice(const std::string& voiceName_p, GameObjectID objectID_p);

		/*!***************************************************************************************
		\brief  This function starts a specific voice inside the bus
		\param  voiceName_p - The name of the voice to start playing
		\param  objectID_p  - ID of the game object that owns the voice for lookups
		\return a boolean indicating if the voice was successfully started
		*****************************************************************************************/
		bool pauseVoice(const std::string& voiceName_p, GameObjectID objectID_p);

		/*!***************************************************************************************
		\brief  This function starts a specific voice inside the bus
		\param  voiceName_p - The name of the voice to start playing
		\param  objectID_p  - ID of the game object that owns the voice for lookups
		\return a boolean indicating if the voice was successfully started
		*****************************************************************************************/
		bool stopVoice(const std::string& voiceName_p, GameObjectID objectID_p);


		// Mixing information functions

		/*!***************************************************************************************
		\brief  This function sets the pitch mixing information that the bus will use on it's final output
		\param  newPitch_p - A pitch parameter that sets the mixing information
		*****************************************************************************************/
		void setPitchMixing(const pitchParameter& newPitch_p);

		/*!***************************************************************************************
		\brief  Gets the current pitch parameter attached to the bus
		\return A reference to the pitch paramter attached to the bus for editing
		*****************************************************************************************/
		pitchParameter& getPitchMixing();

		/*!***************************************************************************************
		\brief  Gets the current pitch parameter attached to the bus
		\return A constant reference to the pitch paramter attached to the bus
		*****************************************************************************************/
		const pitchParameter& getPitchMixing() const;

		/*!***************************************************************************************
		\brief  Sets the speedup factor to be used by the pitch parameter
		\param  speedupFactor_p - The speedup factor to apply to the outgoing signal
		*****************************************************************************************/
		void setSpeedFactor(float speedupFactor_p);

		/*!***************************************************************************************
		\brief  Gets the current speedup factor of the pitch parameter;
		\return A constant reference to the pitch paramter attached to the bus
		*****************************************************************************************/
		float getSpeedFactor() const;

		/*!***************************************************************************************
		\brief Set the volume of the bus
		\param maxGain_p - The volume of the bus as a linear gain factor
		*****************************************************************************************/
		void setGain(float maxGain_p);

		/*!***************************************************************************************
		\brief Get the current volume of the bus
		\return The volume of the bus as a linear gain factor
		*****************************************************************************************/
		float getGain() const;

		void setPanning(int panValue_p);
		int getPanning() const;

		// Filtering and DSP chain functions
		FilterBase* getFilter() const;
		void setFilter(FilterBase* filter_p);

		/// DSP chain stuff...

		// Utility / Basic information related functions
		const std::string& getBusName();
		int getObjectCount();

	private:
		VoiceBase** vArray;
		std::atomic<int> numActiveVoices;
		const int maxVoices;

		int channelCount;
		float* busBuffer;
		const std::string busName;

		// Mixing and DSP
		pitchParameter pitchChange;
		int panning;
		float outputGain;
		FilterBase* filter;
	};
}