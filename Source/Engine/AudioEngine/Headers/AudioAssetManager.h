/*!***************************************************************************************
\file       AudioAssetManager.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		This file contains the AudioAssetManager which faciliates the IO, storage, 
			and retrival of the various audio assets loaded by the game. 
*****************************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "AudioStorage.hpp"

namespace Audio
{ 

/*!***************************************************************************************
\par class: ResourceManager
\brief   This class acts as interface to access, manipulate, and perform IO operations on
         various types of audio assets. Multiple ResourceManagers can be created but they
		 will share the underlying data. 
*****************************************************************************************/
class ResourceManager
{

public:
	ResourceManager();
	ResourceManager(const std::string& inputPath, const std::string& outputPath = "");
	
	void setInputFilepath(const std::string& newFilepath);
	void setOutputFilepath(const std::string& newFilepath);

	void createEmptyNode(const std::string& nodeName, nodeType type = type_mix);

	AudioNode* getAudioNode(const std::string& nodeName_p);
	AudioData* getAudioData(const std::string& dataName_p);

	void LoadFromHierarchy(const std::string& xmlFile);
	AudioNode* loadAudioNode(const std::string& nodeName_p);
	AudioData* loadAudioData(const std::string& dataName_p);

	void releaseAudioNode(const std::string& nodeName_p);
	void releaseAudioData(const std::string& dataName_p);

	void removeAudioDataFromNode(const std::string& audioDataName, const std::string& nodeName);

	/*!***************************************************************************************
	\brief  This function retrieves the target output bus from a specified audio object.
			If it cannot find the object it returns bus_noBus.
	\param  audioObjectName_p - The name of the audio object to find the output bus of.
	\return ID of the mixing bus that this object will be in when played, according to the
			AudioBusType enum in AudioStorage.hpp
	*****************************************************************************************/
	int getBusFromObject(const std::string& audioObjectName_p);

	// If the audio data belongs to a parent, the parent pointer must be passed in.
	// Otherwise it's assumed the source isn't connected to a node
	void move(AudioData* asset, AudioNode* destination, AudioNode* source);

	const std::vector<std::string>& getAllNodes();
	const std::vector<std::string>& getAllAudioData();

	void exportLoadedNodes() const;
	//void generateXML(const std::string* filename) const;

	// Move this somewhere else
	void writeToWaveFile(float* data, int size);
	void writeToWaveFile(const std::string& AudioDataName);

private:
	void initialize();
	void getNames(std::ifstream& file, const std::string& escapeTag, std::vector<std::string>& names);

	std::string inputPath;
	std::string outputPath;

	char* processingInfo;

	static std::unordered_map<std::string, AudioData*> audioDataMap;
	std::vector<std::string> dataNamesCache;
	bool dataCacheValid;

	static std::unordered_map<std::string, AudioNode*> nodeMap;
	std::vector<std::string> nodeNamesCache;
	bool nodeCacheValid;
};

}	// namespace Audio

