/*!***************************************************************************************
\file       ObjectHierarchy.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "ObjectHierarchy.h"
#include "FileDefinitions.h"
#include "ImGui/imgui.h"
#include "AudioAssetManager.h"
#include "UIEventTypes.h"

#include <iostream>
#include <fstream>

#define ID_SIZE 4

std::string defaultFilenameDisp("File to load");
std::string defaultContainerDisp("New Container Name");

const std::string outputPath = "../data/audio/";
const std::string projFileName = "AudioToolSession.atproj";

ObjectHierarchy::ObjectHierarchy(const bool startShown)
{
	filenameBuffer.insert(0, defaultFilenameDisp);
	filenameBuffer.resize(128);

	containerNameBuffer.insert(0, defaultContainerDisp);
	containerNameBuffer.resize(128);

	isVisible = startShown;

	RManager = new Audio::ResourceManager(outputPath);
	RManager->LoadFromHierarchy(outputPath + projFileName);

	dragObject = new dragPair(nullptr);
}

ObjectHierarchy::~ObjectHierarchy()
{
	//delete RManager;
	delete dragObject;
}

void ObjectHierarchy::onUpdate()
{
	static bool createNodeFlag = false;
	static bool loadFileFlag = false;
	static bool saveAllFlag = false;
	static bool exportNodesFlag = false;

	ImGui::Begin("File View", &isVisible, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("New"))
		{
			createNodeFlag = ImGui::MenuItem("Create Node");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Load"))
		{
			loadFileFlag = ImGui::MenuItem("Load File");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Save"))
		{
			saveAllFlag = ImGui::MenuItem("Save All");
			exportNodesFlag = ImGui::MenuItem("Export Nodes");
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	// Load a file
	if (loadFileFlag)
	{
		ImGui::OpenPopup("LoadFile");
		loadFileFlag = false;
	}

	loadFilePrompt(); 

	if (createNodeFlag)
	{
		ImGui::OpenPopup("CreateNode");
		createNodeFlag = false;
	}

	createNodePrompt();

	if (saveAllFlag)
	{
		generateProjectFile();
		exportData();
		saveAllFlag = false;
	}

	if (exportNodesFlag)
	{
		exportData();
		exportNodesFlag = false;
	}

	ImGui::BeginChild("Node View");

	ImGui::Text("Loaded Audio Assets");

	ImGui::Separator();

	//const std::vector<Audio::AssetBase*>* AudioDataVec = RManager->getAllOf<Audio::AudioData>();
	const std::vector<std::string>& AudioDataVec = RManager->getAllAudioData();
	displayAudioData(AudioDataVec);

	ImGui::Separator();

	const std::vector<std::string>& AudioNodesVec = RManager->getAllNodes();
	displayAudioNode(AudioNodesVec);

	if (selectedObject != lastSelected || selectedParent != lastParent)
	{
		lastSelected = selectedObject;
		lastParent = selectedParent;
		sendLocalEvent(new UI::ActiveSwapEvent(selectedObject, selectedParent));
	}

	ImGui::EndChild();
	ImGui::End();
}

void ObjectHierarchy::loadFilePrompt()
{
	if (ImGui::BeginPopupModal("LoadFile"))
	{
		ImGui::Text("Type the full name of the file to load.");
		ImGui::Text("Hit enter to load.");

		if (ImGui::InputText("##L", &filenameBuffer.front(), 128, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::string toLoad = filenameBuffer.substr(0, filenameBuffer.find('\0'));
			if (filenameBuffer.find('.') != std::string::npos)
			{
				std::string extension = filenameBuffer.substr(filenameBuffer.find('.'), filenameBuffer.find('\0'));

				if (extension.find(".wav") != std::string::npos)
				{
					RManager->loadAudioData(toLoad);
					ImGui::CloseCurrentPopup();
				}
				else if (extension.find(".node") != std::string::npos)
				{
					RManager->loadAudioNode(toLoad);
					ImGui::CloseCurrentPopup();
				}
				else if (extension.find(".atproj") != std::string::npos)
				{
					RManager->LoadFromHierarchy(toLoad);
					ImGui::CloseCurrentPopup();
				}
			}

			filenameBuffer.erase();
			filenameBuffer.insert(0, defaultFilenameDisp);
			filenameBuffer.resize(128);
		}

		if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

		ImGui::EndPopup();
	}
}

void ObjectHierarchy::createNodePrompt()
{
	if (ImGui::BeginPopupModal("CreateNode"))
	{
		ImGui::Text("Type the name of the node to create");

		// Create a new node
		if (ImGui::InputText("##C", &containerNameBuffer.front(), 128, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::string containerName = containerNameBuffer.substr(0, containerNameBuffer.find('\0'));
			if (containerName.find(".node") == std::string::npos)
				containerName.append(".node");

			RManager->createEmptyNode(containerName);
			ImGui::CloseCurrentPopup();

			containerNameBuffer.erase();
			containerNameBuffer.insert(0, defaultContainerDisp);
			containerNameBuffer.resize(128);
		}

		if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
			
		ImGui::EndPopup();
	}
}

void ObjectHierarchy::displayAudioData(const std::vector<std::string>& AudioDataVec)
{
	// Display Loose Files
	if (ImGui::TreeNode("Loose Files"))
	{

		std::vector<std::string>::const_iterator adatStart, adatEnd;
		adatStart = AudioDataVec.begin();
		adatEnd   = AudioDataVec.end();
		for (int ID = 0; adatStart != adatEnd; ++adatStart, ++ID)
		{
			Audio::AudioData* currentElem = RManager->getAudioData(*adatStart);
			ImGui::PushID(ID);

			if (ImGui::Selectable(currentElem->getName().c_str(), (selectedObject == currentElem) && !selectedParent))
			{
				// These need to be changed together so the object 
				// has the correct parent (or no parent)
				selectedObject = currentElem;
				selectedParent = nullptr;
			}
				
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				dragObject->newDragTarget(currentElem);
				ImGui::SetDragDropPayload("MOVE_ASSET", &dragObject, sizeof(Audio::AssetBase*));
				ImGui::Text("Copying %s", currentElem->getName().c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginPopupContextItem("Options"))
			{
				ImGui::Text("I'm a context menu!");
				ImGui::EndPopup();
			}

			ImGui::PopID();
		}
		
		ImGui::TreePop();
	}
}

void ObjectHierarchy::displayAudioNode(const std::vector<std::string>& AudioNodesVec)
{
	// Display Nodes
	for (int i = 0; i < (int)AudioNodesVec.size(); ++i)
	{
		Audio::AudioNode* currentElem = RManager->getAudioNode(AudioNodesVec[i]);
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
		nodeFlags |= ((selectedObject == currentElem) ? ImGuiTreeNodeFlags_Selected : 0);

		ImGui::PushID(i);
		bool shown = ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, "%s", currentElem->getName().c_str());

		if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered())
		{
			selectedObject = currentElem;
			selectedParent = nullptr;
		}

		bool eraseNode = false;

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MOVE_ASSET");
			if (payload)
			{
				Audio::AssetBase* target = dragObject->getTarget();
				//if (target == selectedObject)
				//	selectedObject = nullptr;

				Audio::AudioNode* newParent = reinterpret_cast<Audio::AudioNode*>(currentElem);
				Audio::AudioData* asset = reinterpret_cast<Audio::AudioData*>(target);
				Audio::AudioNode* assetParent = reinterpret_cast<Audio::AudioNode*>(dragObject->getParent());

				RManager->move(asset, newParent, assetParent);

				ImGui::EndDragDropTarget();
			}
		}

		if (ImGui::BeginPopupContextItem("containerMods"))
		{
			if (ImGui::Selectable("Delete"))
			{
				eraseNode = true;
				--i;
				RManager->releaseAudioNode(currentElem->getName());
				
				if (selectedObject == currentElem)
				{
					selectedObject = nullptr;
					selectedParent = nullptr;
				}
			}

			ImGui::EndPopup();
		}

		if (shown)
		{
			Audio::AudioNode* node = reinterpret_cast<Audio::AudioNode*>(currentElem); // ???
			const std::vector<Audio::AudioData*>& filesInNode = node->getAllData();
			for (size_t j = 0; j < filesInNode.size(); ++j)
			{
				ImGui::PushID((void*)(intptr_t)j);
				if (ImGui::Selectable(filesInNode[j]->getName().c_str(), (selectedObject == filesInNode[j]) && selectedParent == currentElem))
				{
					selectedObject = filesInNode[j];
					selectedParent = node;
				}
					
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					dragObject->newDragTarget(filesInNode[j], currentElem);
					ImGui::SetDragDropPayload("MOVE_ASSET", &dragObject, sizeof(Audio::AssetBase*));
					ImGui::Text("Copying %s", currentElem->getName().c_str());
					ImGui::EndDragDropSource();
				}

				bool removeAsset = false;
				if (ImGui::BeginPopupContextItem("context"))
				{
					if(ImGui::Selectable("Delete", &removeAsset))
						node->detachAudioData(filesInNode[j]->getName());

					ImGui::EndPopup();
				}
				ImGui::PopID();
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}

void ObjectHierarchy::exportData()
{
	RManager->exportLoadedNodes();
}

void ObjectHierarchy::generateProjectFile()
{
	std::ofstream projectXML(outputPath + projFileName, std::ios_base::binary);
	if (!projectXML.is_open())
	{
		// Handle file not being created
		return;
	}

	projectXML << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
	
	// Graph implementation goes here

	int indent = 0;
	projectXML << std::string(indent, ' ') << "<Node>" << std::endl;
	indent += 2;

	const std::vector<std::string>& outputNodes = RManager->getAllNodes();
	std::vector<std::string>::const_iterator outnStart, outnEnd;
	outnStart = outputNodes.begin();
	outnEnd = outputNodes.end();

	for (; outnStart != outnEnd; ++outnStart)
		writeXMLNode(projectXML, RManager->getAudioNode(*outnStart), indent);

	indent -= 2;
	projectXML << std::string(indent, ' ') << "</Node>" << std::endl;

	projectXML << std::string(indent, ' ') << "<AudioData>" << std::endl;
	indent += 2;

	const std::vector<std::string>& outputLoose = RManager->getAllAudioData();	
	for (size_t i = 0, size = outputLoose.size(); i < size; ++i)
		writeXMLAudioData(projectXML, outputLoose[i], indent);

	indent -= 2;
	projectXML << std::string(indent, ' ') << "</AudioData>" << std::endl;
}

void ObjectHierarchy::writeXMLNode(std::ofstream& file, const Audio::AudioNode* nodeOut, int indent)
{
	std::string outputName = nodeOut->getName();
	if (outputName.find(".node") == std::string::npos)
		outputName += ".node";

	file << std::string(indent, ' ') << "<Name>" << outputName << "</Name>" << std::endl;
	file << std::string(indent, ' ') << "<AudioData>" << std::endl;
	indent += 2;

	const std::vector<Audio::AudioData*> files = nodeOut->getAllData();
	for (size_t i = 0, size = files.size(); i < size; ++i)
		writeXMLAudioData(file, files[i]->getName(), indent);
	
	indent -= 2;
	file << std::string(indent, ' ') << "</AudioData>" << std::endl;
}

void ObjectHierarchy::writeXMLAudioData(std::ofstream& file, const std::string& dataName_p, int indent)
{
	std::string outFileName = dataName_p;
	if (outFileName.find(".wav") == std::string::npos)
		outFileName += ".wav";
	file << std::string(indent, ' ') << "<Name>" << outFileName << "</Name>" << std::endl;
}
