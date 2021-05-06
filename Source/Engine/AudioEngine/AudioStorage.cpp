/*!***************************************************************************************
\file       AudioStorage.cpp
\author     Morgen Hyde
\date       9/11/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Definition of the Audio Engine's interface.
*****************************************************************************************/
#include "AudioStorage.hpp"
#include "AE_AudioMath.hpp"
#include "Voice.h"

#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstring>

#define AE_MONO 1
#define AE_STEREO 2

const float MAX_16 = float((1 << 15) - 1);

namespace Audio
{

	AssetID AssetBase::numAssetTypes = 0;

	// VERIFIED FOR 16 BIT MONO & STEREO AUDIO  //
	std::string path = "data/audio/";

	AudioData::AudioData(const std::string& filename, int sampleRate, short channels,
						 short bitDepth, int dataSize, float* data)
		: fileName(filename), sampleRate(sampleRate), dataSize(dataSize),
		  channels(channels), numSamples(dataSize / (bitDepth / 8)),
		  bitDepth(bitDepth), data(data), activeFilter(nullptr), nodeRefCount(0)
	{
	}

	AudioData::~AudioData()
	{
		delete[] data;
	}

	Voice* AudioData::createVoice(GameObjectID ID, MixSettings* settingsOverride_p, FilterBase* filterOverride_p,
								  const std::string& nameOverride_p)
	{
		// load data from file into a short or int array depending on bit depth
		Voice* newVoice = nullptr;

		std::string voiceName = fileName;
		if (nameOverride_p != "NONE")
			voiceName = nameOverride_p;

		MixSettings* outputSettings = &settings;
		if (settingsOverride_p)
			outputSettings = settingsOverride_p;

		FilterBase* outputFilter = activeFilter;
		if (filterOverride_p)
			outputFilter = filterOverride_p;

		newVoice = new Voice(voiceName, ID, data, numSamples, channels, *outputSettings, outputFilter);

		return newVoice;
	}

	void AudioData::setOutputBus(char outputBus_p)
	{
		if (outputBus_p > bus_BusCount || outputBus_p < bus_Default)
			return;

		settings.bus = outputBus_p;
	}

	const char& AudioData::getOutputBus() const
	{
		return settings.bus;
	}

	MixSettings& AudioData::getMixSettings()
	{
		return settings;
	}

	void AudioData::setMixSettings(const MixSettings& newSettings)
	{
		settings = newSettings;
	}

	void AudioData::setFilter(FilterBase* newFilter)
	{
		if (!newFilter)
			return;

		if (activeFilter)
			delete activeFilter;

		activeFilter = newFilter;
	}

	void AudioData::deleteFilter()
	{
		if (activeFilter)
		{
			delete activeFilter;
			activeFilter = nullptr;
		}
	}

	FilterBase* AudioData::getFilter() const
	{
		return activeFilter;
	}

	const std::vector<DSPBase*>& AudioData::getDSPList() const
	{
		return DSPList;
	}

	void AudioData::incrementRefCount()
	{
		++nodeRefCount;
	}

	void AudioData::decrementRefCount()
	{
		--nodeRefCount;
	}

	int AudioData::getRefCount() const
	{
		return nodeRefCount;
	}


	// AudioNode //
	AudioNode::AudioNode(std::string objectName, nodeType n_type)
		: typeInfo(n_type), name(objectName), activeFilter(nullptr)
	{
	}

	AudioNode::~AudioNode()
	{
		for (size_t i = 0; i < settingsOverride.size(); ++i)
		{
			dataVec[i]->decrementRefCount(); // Jank code - reduce the ref count manually for each attached asset
			delete settingsOverride[i];
		}

	}

	void AudioNode::addAudioData(AudioData* audioData_p)
	{
		audioData_p->incrementRefCount();
		dataVec.push_back(audioData_p);

		settingsOverride.push_back(new MixSettings((audioData_p->getMixSettings())));

		FilterBase* filterPush = audioData_p->getFilter();
		if (filterPush)
		{
			filterPush = FilterFactory::getInstance().createFilter(audioData_p->getFilter()->getFilterName());
		}

		filterOverride.push_back(filterPush);

		// Push back new DSP's for the object into the override

		// check if this audio object would need a bigger
		// buffer than the previously added audio objects
		if (audioData_p->getNumSamples() > voiceBufferSize)
		{
			voiceBufferSize = audioData_p->getNumSamples();
			//TrapezoidEnvelope& envelope = settings.getEnvelope();
			//envelope.setSignalInformation((unsigned int)SAMPLE_RATE, (float)voiceBufferSize / ((float)data->numChannels() * SAMPLE_RATE));
			settings.signalTime = (float)voiceBufferSize / ((float)audioData_p->numChannels() * SAMPLE_RATE);
		}
	}

	AudioData* AudioNode::detachAudioData(const std::string& name_p)
	{
		std::vector<AudioData*>::iterator audioDataIter, audioDataEnd;
		std::vector<MixSettings*>::iterator mixIter = settingsOverride.begin();
		audioDataIter = dataVec.begin();
		audioDataEnd   = dataVec.end();
		for (; audioDataIter != audioDataEnd; ++audioDataIter, ++mixIter)
		{
			if ((*audioDataIter)->getName() == name_p)
			{
				AudioData* toDetach = *audioDataIter;
				dataVec.erase(audioDataIter);
				settingsOverride.erase(mixIter);
				toDetach->decrementRefCount();
				return toDetach;
			}
		}

		return nullptr;
	}

	VoiceBase* AudioNode::createVoice(GameObjectID ID)
	{
		VoiceBase* output = nullptr;

		if (typeInfo == type_mix)
		{
			VoiceGroup* newGroup = new VoiceGroup(name, static_cast<int>(dataVec.size()), 2, ID, settings);

			for (int i = 0; i < dataVec.size(); ++i)
				newGroup->addVoice(dataVec[i]->createVoice(ID, settingsOverride[i], filterOverride[i]));

			output = newGroup;
		}
		else if (typeInfo == type_random)
		{

		}

		return output;
	}

	VoiceBase* AudioNode::createVoice(GameObjectID ID_p, const std::string& audioDataName_p)
	{
		VoiceBase* output = nullptr;

		for (size_t i = 0; i < dataVec.size(); ++i)
		{
			// Look for the specified asset we want to play back
			if (dataVec[i]->getName() == audioDataName_p)
			{
				// Keep the audioData name rather than overriding it with the nodes
				// However we do want to override the mix information
				output = dataVec[i]->createVoice(ID_p, settingsOverride[i], filterOverride[i]);
				break;
			}
		}

		return output;
	}

	void AudioNode::setAudioDataName(const std::string& newName, int index)
	{
		if (index >= static_cast<int>(dataVec.size()))
			return; // Invalid index for naming

		dataVec[index]->setName(newName);
	}

	void AudioNode::getProcessedBuffer(float** bufferAddress, int* bufferSize)
	{
		(void)bufferAddress;
		(void)bufferSize;
	}

	const MixSettings* AudioNode::getMixSettingsOverride(const std::string& audioDataName_p) const
	{
		for (size_t i = 0; i < dataVec.size(); ++i)
		{
			if (dataVec[i]->getName() == audioDataName_p)
			{
				return settingsOverride[i];
			}
		}

		return nullptr;
	}

	void AudioNode::setMixSettingsOverride(const MixSettings& mixSettings_p, const std::string& audioDataName_p)
	{
		for (size_t i = 0; i < dataVec.size(); ++i)
		{
			if (dataVec[i]->getName() == audioDataName_p)
			{
				*settingsOverride[i] = mixSettings_p;
				return;
			}
		}
	}

	FilterBase* AudioNode::getFilterBaseOverride(const std::string& audioDataName_p) const
	{
		for (size_t i = 0; i < dataVec.size(); ++i)
		{
			if (dataVec[i]->getName() == audioDataName_p)
			{
				return filterOverride[i];
			}
		}

		return nullptr;
	}

	void AudioNode::setFilterBaseOverride(FilterBase* filter_p, const std::string& audioDataName_p)
	{
		for (size_t i = 0; i < dataVec.size(); ++i)
		{
			if (dataVec[i]->getName() == audioDataName_p)
			{
				if (filterOverride[i])
					delete filterOverride[i];

				filterOverride[i] = filter_p;
				return;
			}
		}
	}

	const MixSettings& AudioNode::getMixSettings() const
	{
		return settings;
	}

	MixSettings& AudioNode::getMixSettings()
	{
		return settings;
	}

	void AudioNode::setMixSettings(const MixSettings& newSettings)
	{
		settings = newSettings;
	}

	// Filtering and DSP management functions //

	void AudioNode::setFilter(FilterBase* newFilter_p)
	{
		if (!newFilter_p)
			return;

		if (activeFilter)
			delete activeFilter;

		activeFilter = newFilter_p;
	}

	void AudioNode::deleteFilter()
	{
		if (activeFilter)
		{
			delete activeFilter;
			activeFilter = nullptr;
		}
	}

	FilterBase* AudioNode::getFilter() const
	{
		return activeFilter;
	}

	const std::vector<DSPBase*>& AudioNode::getDSPList() const
	{
		return DSPList;
	}

	// Internal data management / IO

	const std::vector<AudioData*>& AudioNode::getAllData() const
	{
		return dataVec;
	}

	// EXPERIMENTAL:
//#ifdef _DEBUG
#include "FileDefinitions.h"
#define ID_SIZE 4

	void AudioNode::exportToFile(const std::string& filepath)
	{
		std::string outputName = name;
		if (outputName.find(".node") == std::string::npos)
			outputName.append(".node");

		std::ofstream outputFile(filepath + outputName, std::ios_base::binary);
		if (!outputFile.is_open())
		{
		}

		NodeFormatChunk nodeInfo;
		std::memcpy(nodeInfo.ID, NodeFileIDs[format].c_str(), 4);
		nodeInfo.chunkSize = sizeof(NodeFormatChunk) - 2 * ID_SIZE;
		nodeInfo.nodeType = typeInfo;
		nodeInfo.channels = 2;
		nodeInfo.sampleRate = 48000;
		nodeInfo.bitDepth = 16;
		nodeInfo.numAssets = static_cast<int>(dataVec.size());
		nodeInfo.garbage = 0;

		MixDataChunk nodeMixInfo;
		std::memcpy(nodeMixInfo.ID, NodeFileIDs[nodeMix].c_str(), 4);
		nodeMixInfo.chunkSize = sizeof(MixDataChunk) - 2 * ID_SIZE;
		float gainVal = settings.maxGain;
		nodeMixInfo.volumeDB = static_cast<int>(gainTodB(gainVal));
		nodeMixInfo.pitchChange = static_cast<int>(settings.pitchChangeCents);
		nodeMixInfo.fadeInTime = settings.fadeInTime;
		nodeMixInfo.fadeOutTime = settings.fadeOutTime;
		nodeMixInfo.looping = settings.looping;
		nodeMixInfo.panning = settings.panValue;
		nodeMixInfo.outputBus = settings.bus;

		int bytesPerSample = nodeInfo.bitDepth / 8;

		int fileSize = sizeof(NodeFormatChunk) + sizeof(MixDataChunk)
			         + static_cast<int>(dataVec.size()) * sizeof(MixDataChunk);
		std::vector<const float*> audioFileData;
		std::vector<int> fileSamples;
		std::vector<std::string> fileNames;
		for (size_t fileIndex = 0, numFiles = dataVec.size(); fileIndex < numFiles; ++fileIndex)
		{
			Audio::AudioData* nextAsset = dataVec[fileIndex];
			audioFileData.push_back(nextAsset->getAudioDataBuffer());
			fileSamples.push_back(nextAsset->getNumSamples());
			fileSize += 4 + fileSamples[fileIndex] * bytesPerSample;
			// TESTING NEW //
			fileSize += static_cast<int>(nextAsset->getName().size());
			fileNames.push_back(nextAsset->getName());
		}

		outputFile.write(NodeFileIDs[fileTitle].c_str(), ID_SIZE); // RIFF file header

		char tempBuffer[ID_SIZE];
		*reinterpret_cast<int*>(tempBuffer) = fileSize;
		outputFile.write(tempBuffer, ID_SIZE);

		// file type specifier
		outputFile.write(NodeFileIDs[fileType].c_str(), ID_SIZE);
		// write node format chunk
		outputFile.write(reinterpret_cast<char*>(&nodeInfo), sizeof(NodeFormatChunk));
		// write node mix settings chunk
		outputFile.write(reinterpret_cast<char*>(&nodeMixInfo), sizeof(MixDataChunk));

		for (size_t fileIndex = 0, numFiles = dataVec.size(); fileIndex < numFiles; ++fileIndex)
		{
			MixDataChunk audioMix;
			std::memcpy(audioMix.ID, NodeFileIDs[audiofileMix].c_str(), ID_SIZE);
			audioMix.chunkSize = sizeof(MixDataChunk) - 2 * ID_SIZE;

			MixSettings* dataSettings = settingsOverride[fileIndex];

			float adGainVal = dataSettings->maxGain;
			audioMix.volumeDB = static_cast<int>(gainTodB(adGainVal));
			audioMix.pitchChange = static_cast<int>(dataSettings->pitchChangeCents);
			audioMix.fadeInTime = dataSettings->fadeInTime;
			audioMix.fadeOutTime = dataSettings->fadeOutTime;
			audioMix.panning = dataSettings->panValue;

			// audio file mix settings
			outputFile.write(reinterpret_cast<char*>(&audioMix), sizeof(MixDataChunk));

			// Write audio data name to the file directly
			outputFile.write(NodeFileIDs[audiofileName].c_str(), ID_SIZE);
			int tempValue = static_cast<int>(fileNames[fileIndex].size());
			outputFile.write(reinterpret_cast<char*>(&tempValue), sizeof(int));
			outputFile.write(fileNames[fileIndex].c_str(), fileNames[fileIndex].size());

			// audio data chunk label
			outputFile.write(NodeFileIDs[audiofileData].c_str(), ID_SIZE);
			*reinterpret_cast<int*>(tempBuffer) = fileSamples[fileIndex] * bytesPerSample;

			// audio data chunk size
			outputFile.write(reinterpret_cast<char*>(tempBuffer), ID_SIZE);
			// convert from floating point used by the engine to integer data used in output file
			const short* shortData = reinterpretAudioData<short>(audioFileData[fileIndex], fileSamples[fileIndex]);
			// write audio data to output file
			outputFile.write(reinterpret_cast<const char*>(shortData), fileSamples[fileIndex] * bytesPerSample);

			delete[] shortData; // Clean up your memory!
		}
	}
//#endif

} // namespace Audio
