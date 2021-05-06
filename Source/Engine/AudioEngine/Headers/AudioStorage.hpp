/*!***************************************************************************************
\file       AudioStorage.hpp
\author     Morgen Hyde
\date       9/11/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Definition of the Audio Engine's interface.
*****************************************************************************************/
#pragma once

#include "AE_RingBuffer.hpp"
#include "MixerObjects.h"
#include "BusInterface.h"

#include "DSPBase.h"
#include "Filter.h"

//========3rd Party Includes============================================================//
#include <vector>
#include <atomic>

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////
namespace Audio
{
	////////==========================================================================////////
	//////// Struct                                                                   ////////
	////////==========================================================================////////

	//========Forward Declaration==========================================================//
	class VoiceBase;
	class Voice;
	class VoiceGroup;

	typedef unsigned int GameObjectID;
	typedef short AssetID;

	////////==========================================================================////////
	//////// Classes                                                                  ////////
	////////==========================================================================////////
	class AssetBase
	{
	public:
		static AssetID numAssetTypes;
		virtual const std::string& getName() const = 0;
		virtual AssetID getAssetType() const = 0;

		virtual void setOutputBus(char outputBus_p) = 0;
		virtual const char& getOutputBus() const = 0;

		virtual MixSettings& getMixSettings() = 0;
		virtual void setMixSettings(const MixSettings& settings) = 0;

		virtual void setFilter(FilterBase* newFilter) = 0;
		virtual void deleteFilter() = 0;

		virtual FilterBase* getFilter() const = 0;
		virtual const std::vector<DSPBase*>& getDSPList() const = 0;
		// EXPERIMENTAL:
	};

	template <typename T>
	class AssetType : public AssetBase
	{
	public:
		AssetType() : defaultName("AssetType Class") {}
		AssetID getAssetType() const { return type; }
		virtual const std::string& getName() const { return defaultName; }

		virtual void setOutputBus(char outputBus_p) = 0;
		virtual const char& getOutputBus() const = 0;

		virtual MixSettings& getMixSettings() = 0;
		virtual void setMixSettings(const MixSettings& settings) = 0;

		virtual void setFilter(FilterBase* newFilter) = 0;
		virtual void deleteFilter() = 0;

		virtual FilterBase* getFilter() const = 0;
		virtual const std::vector<DSPBase*>& getDSPList() const = 0;

	//private:
		const std::string defaultName;
		static const AssetID type;
	};
	
	template <typename T> const AssetID AssetType<T>::type = AssetBase::numAssetTypes++;

	/*!***************************************************************************************
	\par class: AudioData
	\brief      Responsible for loading the audio data from a file and
				creating a voice to hold that data when requested
				Functionality will be modified once streaming is further explored
	*****************************************************************************************/
	class AudioData : public AssetType<AudioData>
	{
	public:

		/*!***************************************************************************************
		\brief  Constructor for AudioData object, passes in a filename to load from a stream flag
		\param  p_file_name - file to load into memory or open for streaming
		\param  p_stream    - flag to stream or block load the file
		*****************************************************************************************/
		AudioData(const std::string& filename, int sampleRate, short channels, 
			      short bitDepth, int dataSize, float* data);

		/*!***************************************************************************************
		\brief  Destructor for the AudioData object, cleans up file handles
		*****************************************************************************************/
		~AudioData();

		/*!***************************************************************************************
		\brief  Gets the number of channels the loaded audio file contains
		\return number of audio channels the file contains
		*****************************************************************************************/
		inline unsigned numChannels() const { return channels; }
		inline size_t getSampleRate() const { return sampleRate; }

		const float* getAudioDataBuffer() const { return data; }
		unsigned getNumSamples() const { return numSamples; }

		const std::string& getName() const override { return fileName; }
		void setName(const std::string& newName) { 
			fileName = newName; 
		}
		

		/*!***************************************************************************************
		\brief  Creates a voice from the audio file, can mix multiple channels down to one
		\param  ID - ID of the audio component that requested this voice be played
		\return Pointer to the new voice object
		*****************************************************************************************/
		Voice* createVoice(GameObjectID ID, MixSettings* settingsOverride = nullptr, FilterBase* filterOverride_p = nullptr,
			               const std::string& nameOverride_p = "NONE");

		void setOutputBus(char outputBus_p) override;

		const char& getOutputBus() const override;

		MixSettings& getMixSettings() override;

		void setMixSettings(const MixSettings& newSettings) override;

		void setFilter(FilterBase* newFilter) override;
		void deleteFilter() override;

		FilterBase* getFilter() const override;
		const std::vector<DSPBase*>& getDSPList() const override;

		// This sucks dude, figure out a cleaner way to get this ref counting
		// functionality. maybe try storing this stuff in the assetManager or use smartPointers
		void incrementRefCount();
		void decrementRefCount();
		int getRefCount() const;

	private:

		std::string fileName;      //!< name of the file to open
		int sampleRate;       //!< sample rate of the file
		int dataSize;		   //!< size of the data in the file (number of samples)
		short channels;		   //!< number of channels in the file
		unsigned frames;
		unsigned numSamples;
		short bitDepth;			   //!< bits per sample (short vs int vs ...)
		float* data; // TODO: Replace with double buffer class

		MixSettings settings;
		FilterBase* activeFilter;
		std::vector<DSPBase*> DSPList;

		short nodeRefCount;
	};

	typedef enum nodeType
	{
		type_mix,
		type_random,

	} nodeType;

	class AudioNode : public AssetType<AudioNode>
	{
	public:
		AudioNode(std::string objectName, nodeType type = type_mix);
		~AudioNode();

		void addAudioData(AudioData* data);
		AudioData* detachAudioData(const std::string& name);

		VoiceBase* createVoice(GameObjectID ID);
		VoiceBase* createVoice(GameObjectID ID, const std::string& audioDataName_p);

		const std::string& getName() const override { return name; }
		nodeType getMixType() const { return typeInfo; }
		void setAudioDataName(const std::string& name, int index);
		bool containsAudioData(const std::string& name);

		__inline const char& getOutputBus() const override { return settings.bus; }
		__inline void setOutputBus(char outputBus_p) override {
			if (outputBus_p > bus_BusCount || outputBus_p < bus_Default)
				return;

			settings.bus = outputBus_p;
		}

		const MixSettings* getMixSettingsOverride(const std::string& audioDataName_p) const;
		void setMixSettingsOverride(const MixSettings& mixSettings_p, const std::string& audioDataName_p);

		FilterBase* getFilterBaseOverride(const std::string& audioDataName_p) const;
		void setFilterBaseOverride(FilterBase* filter_p, const std::string& audioDataName_p);

		std::vector<DSPBase*>& getDSPListOverride(const std::string& audioDataName_p);
		const std::vector<DSPBase*>& getDSPListOverride(const std::string& audioDataName_p) const;

		const MixSettings& getMixSettings() const;
		MixSettings& getMixSettings() override;
		void setMixSettings(const MixSettings& newSettings) override;

		void setFilter(FilterBase* newFilter) override;
		void deleteFilter() override;

		FilterBase* getFilter() const override;
		const std::vector<DSPBase*>& getDSPList() const override;

		const std::vector<AudioData*>& getAllData() const;

		void exportToFile(const std::string& filepath);

	private:

		void getProcessedBuffer(float** bufferAddress, int* bufferSize); // ???

		nodeType typeInfo;
		unsigned voiceBufferSize;

		std::string name;

		MixSettings settings;
		FilterBase* activeFilter;
		std::vector<DSPBase*> DSPList;

		std::vector<AudioData*> dataVec;

		std::vector<MixSettings*> settingsOverride;
		std::vector<FilterBase*> filterOverride;
		// std::vector<std::vector<DSPBase*>> DSPListOverride;
	};

} // namespace Audio