/*!***************************************************************************************
\file       AudioAssetManager.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "AudioAssetManager.h"
#include "FileDefinitions.h"
#include "AE_AudioMath.hpp"
#include "Envelopes.h"

#include "Serializer.h"

#include <fstream>
#include <cstring> // std::memcmp, std::memset
#include <stdexcept> // std::runtime_error

#include <unordered_map>

const float MAX_16 = float((1 << 15) - 1);

namespace Audio
{
	const std::string wavHeader = "RIFFxxxxWAVEfmt xxxxxxxxxxxxxxxxxxxxxxxxdataxxxxxxxx";
	const std::string nodeHeader = "RIFFxxxxNODEfmt NMIXAMIXADAT";

	const char RIFF_tag[] = { 'R', 'I', 'F', 'F' };
	const char WAVE_tag[] = { 'W', 'A', 'V', 'E' };
	const char FMT_tag[] = { 'f', 'm', 't', ' ' };
	const char DATA_tag[] = { 'd', 'a', 't', 'a' };

	const char NODE_tag[] = { 'N', 'O', 'D', 'E' };
	const char ADTA_tag[] = { 'A', 'D', 'T', 'A' };

	const char MIX_tag[] = { 'M', 'I', 'X', ' ' };
	const char FILT_tag[] = { 'F', 'I', 'L', 'T' };
	const char DSP_tag[] = { 'D', 'S', 'P', ' ' };

#define CHUNK_HEADER_SIZE 8

	std::unordered_map<std::string, AudioData*> ResourceManager::audioDataMap;
	std::unordered_map<std::string, AudioNode*> ResourceManager::nodeMap;

	ResourceManager::ResourceManager()
	{
		initialize();
	}

	ResourceManager::ResourceManager(const std::string& inputPath_p, const std::string& outputPath_p)
		: inputPath(inputPath_p), outputPath(outputPath_p)
	{
		if (outputPath_p.length() == 0)
			outputPath = inputPath;

		initialize();
	}

	void ResourceManager::initialize()
	{
		dataCacheValid = false;
		nodeCacheValid = false;

		processingInfo = new char[64];
	}

	void ResourceManager::setInputFilepath(const std::string& newFilepath)
	{
		inputPath = newFilepath;
	}

	void ResourceManager::setOutputFilepath(const std::string& newFilepath)
	{
		outputPath = newFilepath;
	}

	void ResourceManager::LoadFromHierarchy(const std::string& hierarchyFile)
	{
		std::ifstream file(hierarchyFile, std::ios_base::binary);
		if (!file.is_open())
			return;

		std::vector<std::string> Nodes;
		std::vector<std::string> LooseFiles;
		std::string header, line;

		getline(file, header);

		getNames(file, "</Node>", Nodes); // Read in the names of all nodes and the assets inside them
		getNames(file, "</AudioData>", LooseFiles);
		AudioNode* currentParent = nullptr;
		int numAssets = 0;

		for (int i = 0, size = static_cast<int>(Nodes.size()); i < size; ++i)
		{
			std::string& activeName = Nodes[i];
			if (activeName.find(".node") != std::string::npos)
			{
				currentParent = loadAudioNode(activeName);
				numAssets = 0;
			}
			else if (currentParent)
			{
				//currentParent->setAudioDataName(activeName, numAssets++);
			}
		}

		for (int i = 0, size = static_cast<int>(LooseFiles.size()); i < size; ++i)
			loadAudioData(LooseFiles[i]);
	}

	void ResourceManager::createEmptyNode(const std::string& nodeName, nodeType type)
	{
		AudioNode* blankNode = new AudioNode(nodeName, type);
		if (nodeMap.find(nodeName) == nodeMap.end())
		{
			nodeMap[nodeName] = blankNode;
			nodeCacheValid = false;
		}
	}

	AudioNode* ResourceManager::getAudioNode(const std::string& nodeName_p)
	{
		if (nodeMap.find(nodeName_p) != nodeMap.end())
			return nodeMap[nodeName_p];

		return nullptr;
	}

	AudioData* ResourceManager::getAudioData(const std::string& dataName_p)
	{
		if (audioDataMap.find(dataName_p) != audioDataMap.end())
		{
			dataCacheValid = false;
			return audioDataMap[dataName_p];
		}

		return nullptr;
	}

	AudioNode* ResourceManager::loadAudioNode(const std::string& nodeName_p)
	{
		if (nodeMap.find(nodeName_p) != nodeMap.end())
			return nodeMap[nodeName_p];

		// Perform loading code
		std::ifstream source(inputPath + nodeName_p, std::ios_base::binary); // opens node RIFF file

		if (!source.is_open())
			throw std::runtime_error(std::string("Failed to read: " + inputPath + nodeName_p));

		char readHeader[12];

		source.read(readHeader, 12);
		if (std::memcmp(readHeader + 0, RIFF_tag, 4))
			throw std::runtime_error(std::string("Cannot read file: " + nodeName_p + " missing RIFF tag"));

		if (std::memcmp(readHeader + 8, NODE_tag, 4))
			throw std::runtime_error(std::string("Cannot read file: " + nodeName_p + " missing NODE tag"));

		source.read(reinterpret_cast<char*>(&readHeader), CHUNK_HEADER_SIZE);
		if (std::memcmp(readHeader, FMT_tag, 4)) { /* Handle */ }
		int chunkDataSize = *reinterpret_cast<int*>(readHeader + 4);

		source.read(reinterpret_cast<char*>(&readHeader), chunkDataSize);
		int sampleRate = *reinterpret_cast<int*>(readHeader + 0);
		int channels   = *reinterpret_cast<int*>(readHeader + 4);
		int bitDepth   = *reinterpret_cast<int*>(readHeader + 8);

		source.read(reinterpret_cast<char*>(&readHeader), CHUNK_HEADER_SIZE);
		if (std::memcmp(readHeader, MIX_tag, 4))
			throw std::runtime_error(std::string("Cannot read file" + nodeName_p + " missing MIX tag"));

		int fileMixReadSize = *reinterpret_cast<int*>(readHeader + 4);
		// If the mix chunk of this file is larger or smaller than we expect, read the smaller amount to avoid overfilling
		fileMixReadSize = (fileMixReadSize < sizeof(MixSettings) ? fileMixReadSize : sizeof(MixSettings));

		MixSettings settingsRead;
		source.read(reinterpret_cast<char*>(&settingsRead), fileMixReadSize);

		AudioNode* newNode = new AudioNode(nodeName_p);
		newNode->setMixSettings(settingsRead);

		// Read the next header to trigger the eof bit being set
		source.read(readHeader, CHUNK_HEADER_SIZE);

		// TODO: Handle parsing of a filter (or not!) here
		if (!std::memcmp(readHeader, FILT_tag, 4))
		{
			int chunkSize = *reinterpret_cast<int*>(readHeader + 4);

			// Read the size of the string
			source.read(readHeader, 4);
			int stringLength = *reinterpret_cast<int*>(readHeader);

			std::string filterType(stringLength, 0);
			source.read(&(filterType[0]), stringLength);

			FilterBase* constructedFilter = FilterFactory::getInstance().createFilter(filterType);

			int remainingSize = chunkSize - stringLength - sizeof(int);

			source.read(processingInfo, remainingSize);

			constructedFilter->initFromArray(processingInfo);
			newNode->setFilter(constructedFilter);

			// Read the next header to trigger the eof bit being set
			source.read(readHeader, CHUNK_HEADER_SIZE);
		}

		// TODO: Handle parsing of N DSP objects (or none!) here

		while (!source.eof()) // loop through the rest of the file (assuming the rest of the file is AudioData objects)
		{
			if (std::memcmp(readHeader, ADTA_tag, 4))
				throw std::runtime_error(std::string("Unidentified tag in file: " + nodeName_p));

			int ADataSize  = *reinterpret_cast<int*>(readHeader + 4);
			source.read(readHeader, 4);

			int nameLength = *reinterpret_cast<int*>(readHeader);
			std::string objectName(nameLength, '0');
			source.read(&(objectName[0]), nameLength);

			int dataSize = ADataSize - nameLength - sizeof(int);

			AudioData* data = getAudioData(objectName);
			if (!data)
			{
				char* readBuffer = new char[dataSize];
				source.read(readBuffer, dataSize);

				float* audioDataBuffer = reinterpretAudioData<short>(readBuffer, dataSize / (bitDepth / 8));

				delete[] readBuffer;

				data = new AudioData(objectName, sampleRate, channels, bitDepth, dataSize, audioDataBuffer);
				audioDataMap[objectName] = data;
				dataCacheValid = false;
			}
			else
			{
				source.seekg((int)source.tellg() + dataSize);
			}

			newNode->addAudioData(data);
			source.read(readHeader, CHUNK_HEADER_SIZE);

			if (std::memcmp(readHeader, MIX_tag, 4))
				throw std::runtime_error(std::string("Missing MIX tag after an audioData object in" + nodeName_p));

			fileMixReadSize = *reinterpret_cast<int*>(readHeader + 4);
			// If the mix chunk of this file is larger or smaller than we expect, read the smaller amount to avoid overfilling
			fileMixReadSize = (fileMixReadSize < sizeof(MixSettings) ? fileMixReadSize : sizeof(MixSettings));

			source.read(reinterpret_cast<char*>(&settingsRead), fileMixReadSize);

			newNode->setMixSettingsOverride(settingsRead, objectName);

			// Read the next chunk header to trigger the eof bit if we're at the end of the file
			source.read(reinterpret_cast<char*>(readHeader), CHUNK_HEADER_SIZE);

			// TODO: Handle parsing of a filter (or not!) here
			// TODO: Handle parsing of a filter (or not!) here
			if (!std::memcmp(readHeader, FILT_tag, 4))
			{
				int chunkSize = *reinterpret_cast<int*>(readHeader + 4);

				// Read the size of the string
				source.read(readHeader, 4);
				int stringLength = *reinterpret_cast<int*>(readHeader);

				std::string filterType(stringLength, 0);

				source.read(&(filterType[0]), stringLength);
				FilterBase* constructedFilter = FilterFactory::getInstance().createFilter(filterType);

				int remainingSize = chunkSize - stringLength - sizeof(int);

				source.read(processingInfo, remainingSize);

				constructedFilter->initFromArray(processingInfo);
				newNode->setFilterBaseOverride(constructedFilter, objectName);

				// Read the next header to trigger the eof bit being set
				source.read(readHeader, CHUNK_HEADER_SIZE);
			}

			// TODO: Handle parsing of N DSP objects (or none!) here


		}

		nodeCacheValid = false;
		nodeMap[nodeName_p] = newNode;

		return newNode;
	}

	AudioData* ResourceManager::loadAudioData(const std::string& dataName_p)
	{
		if (audioDataMap.find(dataName_p) != audioDataMap.end())
			return audioDataMap[dataName_p];

		std::ifstream sourceFile(inputPath + dataName_p, std::ios_base::binary);
		if (!sourceFile.is_open())
			return nullptr;

		RIFFChunk expectedRiff = { {'R','I','F','F'}, 0, {'W','A','V','E'} };
		fmt_Chunk expectedFmt = { {'f','m','t',' '} };
		dataChunk expectedData = { {'d','a','t','a'} };

		RIFFChunk actualRiff;
		fmt_Chunk actualFmt;
		dataChunk actualData;

		// TODO: UNUSED
		// const char* expectedWavHeader = wavHeader.c_str();
		char* fileHeader = new char[44];

		sourceFile.read(reinterpret_cast<char*>(&actualRiff), sizeof(RIFFChunk));
		if (memcmp(actualRiff.ID, expectedRiff.ID, 4) != 0) { /* Handle */ }
		if (memcmp(actualRiff.type, expectedRiff.type, 4) != 0) { /* Handle */ }

		sourceFile.read(reinterpret_cast<char*>(&actualFmt), 8);
		while (memcmp(actualFmt.ID, expectedFmt.ID, 4) != 0)
		{
			int fakeChunkSize = actualFmt.size;
			sourceFile.ignore(fakeChunkSize);
			sourceFile.read(reinterpret_cast<char*>(&actualFmt), 8);
		}

		sourceFile.read(reinterpret_cast<char*>(&actualFmt) + 8, sizeof(fmt_Chunk) - 8);

		short channels = actualFmt.channels;
		int sampleRate = actualFmt.sampleRate;
		short bitDepth = actualFmt.bitsPerSample;

		sourceFile.read(reinterpret_cast<char*>(&actualData), sizeof(dataChunk));
		while (memcmp(actualData.ID, expectedData.ID, 4) != 0)
		{
			int fakeChunkSize = actualData.size;
			sourceFile.ignore(fakeChunkSize);
			sourceFile.read(reinterpret_cast<char*>(&actualData), sizeof(dataChunk));
		}

		int dataSize = actualData.size;
		int numSamples = dataSize / (bitDepth / 8);

		float* data = nullptr;
		MixSettings mix;

		if (bitDepth == 16)
		{
			short* shortData = new short[dataSize / sizeof(short)];
			sourceFile.read(reinterpret_cast<char*>(shortData), dataSize);
			data = new float[numSamples];
			for (int i = 0; i < numSamples; ++i)
			{
				data[i] = *shortData / MAX_16;
				shortData++;
			}

			shortData -= numSamples;
			delete[] shortData;
		}

		std::memset(&mix, 0, sizeof(MixSettings));

		mix.maxGain = 1.0f;
		mix.signalTime = static_cast<float>(numSamples) / (float)(channels * sampleRate);
		mix.panValue = 50;
		mix.bus = bus_Default;

		AudioData* newData = new AudioData(dataName_p, sampleRate, channels, bitDepth, dataSize, data);
		newData->setMixSettings(mix);

		dataCacheValid = false;
		audioDataMap[dataName_p] = newData;

		delete[] fileHeader;

		return newData;
	}

	// TODO: handle the memory leak here from deleting

	void ResourceManager::releaseAudioNode(const std::string& nodeName_p)
	{
		if (nodeMap.find(nodeName_p) != nodeMap.end())
		{
			nodeCacheValid = false;
			//delete nodeMap[nodeName_p];
			nodeMap.erase(nodeName_p);
		}
	}

	void ResourceManager::releaseAudioData(const std::string& dataName_p)
	{
		std::unordered_map<std::string, AudioData*>::iterator findData = audioDataMap.find(dataName_p);
		if (findData != audioDataMap.end())
		{
			dataCacheValid = false;
			//delete findData->second;
			audioDataMap.erase(findData);
		}
	}

	// This function retrieves the target output bus from a specified audio object.
	// TODO: Please clean this up. This isn't the best way to do this
	int ResourceManager::getBusFromObject(const std::string& objectName_p)
	{
		// Parse the string to determine if it's a node, they should all have this extension
		if (objectName_p.find(".node") != std::string::npos)
		{
			// Iterate over the list of AudioData objects
			std::unordered_map<std::string, AudioNode*>::iterator findNode = nodeMap.find(objectName_p);
			if (findNode != nodeMap.end())
				return findNode->second->getOutputBus();

			// return default if not found
			return bus_Default;
		}
		else
		{
			// Iterate over the list of AudioData objects
			std::unordered_map<std::string, AudioData*>::iterator findData = audioDataMap.find(objectName_p);
			if (findData != audioDataMap.end())
				return findData->second->getOutputBus();

			// Return default if not found
			return bus_Default;
		}
	}

	void ResourceManager::removeAudioDataFromNode(const std::string& audioDataName_p, const std::string& nodeName_p)
	{
		std::unordered_map<std::string, AudioNode*>::iterator findNode = nodeMap.find(nodeName_p);
		if (findNode != nodeMap.end())
			findNode->second->detachAudioData(audioDataName_p);
	}

	void ResourceManager::getNames(std::ifstream& file,
		const std::string& escapeTag,
		std::vector<std::string>& names)
	{
		std::string line;

		while (getline(file, line))
		{
			if (line.find("<Name>") != std::string::npos)
			{
				int nameTagEnd = (int)line.find_first_of('>') + 1;
				int nameEndTagStart = (int)line.find_last_of('<');
				std::string NodeName = line.substr(nameTagEnd, nameEndTagStart - nameTagEnd);
				names.push_back(NodeName);
			}
			else if (line.find(escapeTag) != std::string::npos)
			{
				return;
			}
			else
			{
				std::string escape = line;
				escape.erase(0, escape.find_first_not_of(" "));
				escape.insert(escape.begin() + 1, '/');

				getNames(file, escape, names);
			}
		}
	}

	const std::vector<std::string>& ResourceManager::getAllNodes()
	{
		if (nodeCacheValid)
			return nodeNamesCache;

		nodeNamesCache.clear();

		std::unordered_map<std::string, AudioNode*>::iterator nodesStart, nodesEnd;
		nodesStart = nodeMap.begin();
		nodesEnd = nodeMap.end();

		while (nodesStart != nodesEnd)
		{
			nodeNamesCache.push_back(nodesStart->first);
			++nodesStart;
		}

		nodeCacheValid = true;
		return nodeNamesCache;
	}

	const std::vector<std::string>& ResourceManager::getAllAudioData()
	{
		if (dataCacheValid)
			return dataNamesCache;

		dataNamesCache.clear();

		std::unordered_map<std::string, AudioData*>::iterator dataStart, dataEnd;
		dataStart = audioDataMap.begin();
		dataEnd = audioDataMap.end();

		while (dataStart != dataEnd)
		{
			dataNamesCache.push_back(dataStart->first);
			++dataStart;
		}

		dataCacheValid = true;
		return dataNamesCache;
	}

	// This was redesigned on 11/30/18 to assume that nodes cannot be placed inside other nodes.
	// the extra effort to support this tree-like behavior isn't currently in scope. Don't do it.
	void ResourceManager::move(AudioData* asset, AudioNode* destination, AudioNode* source = nullptr)
	{
		if (!destination && !asset) // Do null pointer check before manipulation
			return;

		if (source)
			source->detachAudioData(asset->getName());

		destination->addAudioData(asset);
	}

	void ResourceManager::exportLoadedNodes() const
	{
		std::unordered_map<std::string, AudioNode*>::iterator nodeIter, nodesEnd;
		nodeIter = nodeMap.begin();
		nodesEnd = nodeMap.end();

		//for (; nodeIter != nodesEnd; ++nodeIter)
		//	nodeIter->second->exportToFile(outputPath);

		Serializer output(outputPath);
		while (nodeIter != nodesEnd)
		{
			output.writeNode(nodeIter->second);
			++nodeIter;
		}
	}

	void ResourceManager::writeToWaveFile(float* data, int size)
	{
		RIFFChunk expectedRiff = { {'R','I','F','F'},
								   static_cast<int>(4 + sizeof(fmt_Chunk) + sizeof(dataChunk) + size * 16),
								   {'W','A','V','E'} };
		fmt_Chunk expectedFmt = { {'f','m','t',' '}, sizeof(fmt_Chunk) - 8,
								  1, 2, 48000, 48000 * 2 * 2, 2 * 2, 16 };
		dataChunk expectedData = { {'d','a','t','a'}, size * 16 };

		std::ofstream output(outputPath + "debug.wav", std::ios_base::binary);
		if (!output.is_open())
		{
		}

		output.write(reinterpret_cast<char*>(&expectedRiff), sizeof(RIFFChunk));
		output.write(reinterpret_cast<char*>(&expectedFmt), sizeof(fmt_Chunk));
		output.write(reinterpret_cast<char*>(&expectedData), sizeof(dataChunk));

		short* outData = reinterpretAudioData<short>(data, size);
		output.write(reinterpret_cast<char*>(outData), size * 16);
	}

}	// namespace Audio
