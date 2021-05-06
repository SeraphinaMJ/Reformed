/*!***************************************************************************************
\file       ObjectHierarchy.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "UI_System.h"
#include "AudioStorage.hpp"

//#include "AudioGraph.h"
#include <string>
namespace Audio {
	class ResourceManager;
};

class dragPair
{
public:
	dragPair(Audio::AssetBase* dragTarget_p, Audio::AssetBase* dragParent_p = nullptr)
		: dragTarget(dragTarget_p), dragParent(dragParent_p) {}

	void newDragTarget(Audio::AssetBase* dragTarget_p, Audio::AssetBase* dragParent_p = nullptr)
	{
		dragTarget = dragTarget_p;
		dragParent = dragParent_p;
	}

	Audio::AssetBase* getTarget() { return dragTarget; }
	Audio::AssetBase* getParent() { return dragParent; }

private:
	Audio::AssetBase* dragTarget;
	Audio::AssetBase* dragParent; // This should always be null or an audioNode or a graphPointer
};

class ObjectHierarchy : public UI_ComponentsBase
{
public:
	ObjectHierarchy(const bool startShown = true);
	~ObjectHierarchy() override;

	void onUpdate() override;
	void exportData();

private:

	// Menu Bar functions
	void loadFilePrompt();
	void createNodePrompt();

	// display functions for all audio assets
	void displayAudioData(const std::vector<std::string>& AudioDataVec);
	void displayAudioNode(const std::vector<std::string>& AudioNodesVec);

	// I/O functions 
	void generateProjectFile();
	//void writeXMLGraph(const Graph& graphOut);

	void writeXMLNode(std::ofstream& file, const Audio::AudioNode* nodeOut, int indent);
	void writeXMLAudioData(std::ofstream& file, const std::string& dataName_p, int indent);

	// Audio Asset Manipulation (move, change parameters)
	dragPair* dragObject;

	// This fixes the voiceGroup mix overriding as long as nodes can't hold nodes
	Audio::AssetBase* selectedParent; 
	Audio::AssetBase* selectedObject;

	Audio::AssetBase* lastSelected;
	Audio::AssetBase* lastParent;

	bool isVisible;

	Audio::ResourceManager* RManager;
	std::string filenameBuffer;
	std::string containerNameBuffer;
};