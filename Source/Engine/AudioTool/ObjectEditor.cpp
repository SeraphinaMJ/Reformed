/*!***************************************************************************************
\file       ObjectEditor.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "ObjectEditor.h"
#include "ImGui/imgui.h"
#include "MixerObjects.h"
#include "UIEventTypes.h"
#include "AudioAssetManager.h"

#include "DSPFactory.h"
#include "FilterFactory.h"

#include <cmath> // std::fmod

#include "AE_AudioMath.hpp"

const char* BusTargets[]  = { "No Bus", "Music", "Sound Effects", "UI" };

ObjectEditor::ObjectEditor(Audio::BusInterface* busInterface_p)
	: activeView(view_Closed), bInterface(busInterface_p), activeObject(nullptr)
{
	//activeEnvelope = new envelopeDisplay();
	//activePitch = new pitchDisplay();

	// handle (if at all) filter vector initialization
	// same for DSP vector initialization
}

ObjectEditor::~ObjectEditor()
{
	//delete activeEnvelope;
	//delete activePitch;
	
	// Properly deallocate vectors of Filters and DSP
}

void ObjectEditor::onUpdate()
{
	editGraphs();
	editProperties();
	editProcessing();

	editBusMixing();
}

void ObjectEditor::setActiveObject(UI::ActiveSwapEvent* swapEvent)
{
	activeObject = swapEvent->data;
	activeParent = dynamic_cast<Audio::AudioNode*>(swapEvent->parent);

	if (activeObject)
	{
		activeSettings = activeObject->getMixSettings();
		if (activeParent)
			activeSettings = *(activeParent->getMixSettingsOverride(activeObject->getName()));

		filterEditor.reset();
	}
}

struct TempNode {
	ImVec2 Position;
	ImVec2 NodeDim;
	float data;
	std::string DisplayText;
};

static inline ImVec2 operator+(const ImVec2& left, const ImVec2& right) { return ImVec2(left.x + right.x, left.y + right.y); }
static inline ImVec2 operator-(const ImVec2& left, const ImVec2& right) { return ImVec2(left.x - right.x, left.y - right.y); }

void ObjectEditor::editGraphs()
{
	static ImVec2 scrolling(0, 0);
	
	ImVec2 offset = ImGui::GetCursorScreenPos() + scrolling;

	static TempNode node1  = { ImVec2(0.0f, 0.0f),   ImVec2(100.0f, 100.0f), 0.0f, "node1"  };
	static TempNode node2  = { ImVec2(50.0f, 50.0f), ImVec2(100.0f, 100.0f), 0.0f, "node2" };

	static const ImVec2 NodePadding(8.0f, 8.0f);

	ImU32 testColor = ImColor(200, 50, 20);
	ImU32 nodeColor = ImColor(30, 30, 50);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Border, testColor);
	ImGui::Begin("Graph Editor");

	ImGui::Text("Offset coordinates %.2f, %.2f", offset.x, offset.y);
	ImGui::SameLine();
	if (ImGui::Button("Zero Scrolling"))
	{
		scrolling.x = 0.0f;
		scrolling.y = 0.0f;
	}

	ImGui::BeginChild("Graph Window", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	
	ImVec2 window = ImGui::GetWindowSize();
	ImVec2 gridDim(window.x / 20.0f, window.y / 20.0f);
	
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	for (float xLine = std::fmod(scrolling.x, gridDim.x); xLine < window.x; xLine += gridDim.x)
		drawList->AddLine(ImVec2(xLine + cursorPos.x, 0.0f + cursorPos.y), ImVec2(xLine + cursorPos.x, window.y + cursorPos.y), testColor);
	for (float yLine = std::fmod(scrolling.y, gridDim.y); yLine < window.y; yLine += gridDim.y)
		drawList->AddLine(ImVec2(0.0f + cursorPos.x, yLine + cursorPos.y), ImVec2(window.x + cursorPos.x, yLine + cursorPos.y), testColor);

	
	drawList->ChannelsSplit(2);
	
	drawList->ChannelsSetCurrent(0);
	// Draw the edges between nodes

	ImVec2 node1Out = offset + node1.Position;
	node1Out.y += node1.NodeDim.y / 2.0f;
	ImVec2 node2In = offset + node2.Position + node2.NodeDim;
	node2In.y -= node2.NodeDim.y / 2.0f;
	drawList->AddBezierCurve(node1Out, node1Out + ImVec2(50, 0), node2In + ImVec2(-50, 0), node2In, ImColor(200, 200, 0), 1.0f);

	// Draw the actual node objects
	{
		ImGui::PushID(node1.DisplayText.c_str());
		drawList->ChannelsSetCurrent(1);
		ImVec2 nodeMinCorner = node1.Position + offset;
		ImGui::SetCursorScreenPos(nodeMinCorner + NodePadding);
		ImGui::BeginGroup();
		ImGui::Text("%s", node1.DisplayText.c_str());
		ImGui::SliderFloat("Node Data", &(node1.data), 0.0f, 10.0f);
		ImGui::EndGroup();

		node1.NodeDim = ImGui::GetItemRectSize() + NodePadding + NodePadding;
		ImVec2 nodeMaxCorner = nodeMinCorner + node1.NodeDim;

		drawList->ChannelsSetCurrent(0);
		ImGui::SetCursorScreenPos(nodeMinCorner);
		ImGui::InvisibleButton("node", node1.NodeDim);
		if (ImGui::IsMouseDragging(0) && ImGui::IsItemActive())
			node1.Position = node1.Position + ImGui::GetIO().MouseDelta;

		drawList->AddRectFilled(nodeMinCorner, nodeMaxCorner, nodeColor, 4.0f);
		drawList->AddRect(nodeMinCorner, nodeMaxCorner, ImColor(255.0f, 255.0f, 255.0f), 4.0f);
		ImGui::PopID();
	}

	{
		ImGui::PushID((node2.DisplayText + "1").c_str());
		drawList->ChannelsSetCurrent(1);
		ImVec2 nodeMinCorner = node2.Position + offset;
		ImGui::SetCursorScreenPos(nodeMinCorner + NodePadding);
		ImGui::BeginGroup();
		ImGui::Text("%s", node2.DisplayText.c_str());
		ImGui::SliderFloat("Node Data", &(node2.data), 0.0f, 10.0f);
		ImGui::EndGroup();

		node2.NodeDim = ImGui::GetItemRectSize() + NodePadding + NodePadding;
		ImVec2 nodeMaxCorner = nodeMinCorner + node2.NodeDim;

		drawList->ChannelsSetCurrent(0);
		ImGui::SetCursorScreenPos(nodeMinCorner);
		ImGui::InvisibleButton("node", node2.NodeDim);
		if (ImGui::IsMouseDragging(0) && ImGui::IsItemActive())
			node2.Position = node2.Position + ImGui::GetIO().MouseDelta;

		drawList->AddRectFilled(nodeMinCorner, nodeMaxCorner, nodeColor, 4.0f);
		drawList->AddRect(nodeMinCorner, nodeMaxCorner, ImColor(255.0f, 255.0f, 255.0f), 4.0f);
		ImGui::PopID();
	}

	drawList->ChannelsMerge();

	if (ImGui::IsMouseDragging(1, 0.0f) && !ImGui::IsAnyItemActive() && ImGui::IsWindowHovered())
	{
		scrolling.x += ImGui::GetIO().MouseDelta.x;
		scrolling.y += ImGui::GetIO().MouseDelta.y;
	}

	ImGui::SetCursorScreenPos(cursorPos);
	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void ObjectEditor::editProperties()
{
	ImGui::Begin("Property Editor");

	if (!activeObject)
	{
		ImGui::Text("No Object Selected");
		ImGui::End();
		return;
	}

	
	std::string parentName = "";
	if (activeParent)
		parentName = activeParent->getName();

	ImGui::TextWrapped("Currently Editing: %s/%s", parentName.c_str(), activeObject->getName().c_str());

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

	bool dirtyFlag = false;

	if (editEnvelope())
		dirtyFlag = true;

	if (editPitch())
		dirtyFlag = true;

	float childWidth = ImGui::GetWindowContentRegionWidth();

	ImGui::BeginChild("Panning Tool", ImVec2(childWidth, 60), true);
	ImGui::PushItemWidth(childWidth * 0.955f);
	ImGui::TextWrapped("Panning Tool (L to R)");
	int panning = static_cast<int>(activeSettings.panValue);
	ImGui::DragInt("##P", &panning, 1.0f, 0, 100);
	activeSettings.panValue = static_cast<char>(panning);
	if (ImGui::IsItemDeactivatedAfterEdit())
		dirtyFlag = true;
		
	ImGui::PopItemWidth();
	ImGui::EndChild();

	// We don't want audioData objects that belong to a node being edited able to change this
	ImGui::BeginChild("Bus Selection", ImVec2(childWidth, 60), true);
	ImGui::PushItemWidth(childWidth * 0.955f);
	ImGui::TextWrapped("OutputBus");
	int targetBus = activeSettings.bus;
	if (ImGui::Combo("##bus", &targetBus, BusTargets, sizeof(BusTargets) / sizeof(char*)))
	{
		activeSettings.bus = static_cast<char>(targetBus);
		dirtyFlag = true;
	}
	ImGui::PopItemWidth();
	ImGui::EndChild();

	if (dirtyFlag)
	{
		if (activeParent)
			activeParent->setMixSettingsOverride(activeSettings, activeObject->getName());
		else
			activeObject->setMixSettings(activeSettings);

		dirtyFlag = false;
	}


	ImGui::PopStyleVar();
	ImGui::End();
}

void ObjectEditor::editProcessing()
{
	static bool createFilterFlag = false;
	static bool createDSPFlag = false;

	ImGui::Begin("Processing Window", 0, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			createFilterFlag = ImGui::MenuItem("Filter");
			createDSPFlag    = ImGui::MenuItem("DSP");

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (createFilterFlag && activeObject)
	{
		ImGui::OpenPopup("filterDialogue");
		createFilterFlag = false;
	}

	createFilterPrompt();

	if (createDSPFlag && activeObject)
	{
		ImGui::OpenPopup("DSPDialogue");
		createDSPFlag = false;
	}

	createDSPPrompt();

	if (!activeObject)
	{
		ImGui::Text("No Object Selected");
		ImGui::End();
		return;
	}

	float childWidth = ImGui::GetWindowContentRegionWidth();
	

	ImGui::TextWrapped("Currently Editing: %s", activeObject->getName().c_str());

	float contentHeight = ImGui::GetWindowHeight() - ImGui::GetCursorPosY();

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

	// Throw this into a separate function
	ImGui::BeginChild("Filter Editor", ImVec2(childWidth, contentHeight / 2.0f), true);
	ImGui::PushItemWidth(childWidth * 0.955f);

	Audio::FilterBase* activeFilter = activeObject->getFilter();
	if (activeParent)
		activeFilter = activeParent->getFilterBaseOverride(activeObject->getName());

	if (activeFilter)
		activeFilter->editFilter(&filterEditor);
	else
		ImGui::Text("No Active Filter!");

	ImGui::PopItemWidth();
	ImGui::EndChild();

	// Throw this into a seperate function also
	ImGui::BeginChild("DSP Editor", ImVec2(childWidth, contentHeight / 2.0f/*330.0f*/), true);
	ImGui::PushItemWidth(childWidth * 0.955f);

	// DSP stuff goes here
	const std::vector<Audio::DSPBase*>& DSPChain = activeObject->getDSPList();

	if (DSPChain.size())
	{
		static int activeDSP = 0;

		if (ImGui::BeginCombo("DSP List", DSPChain[activeDSP]->getDSPName().c_str()))
		{
			for (int dspElem = 0; dspElem < (int)DSPChain.size(); ++dspElem)
			{
				bool dspSelected = (activeDSP == dspElem);
				if (ImGui::Selectable(DSPChain[dspElem]->getDSPName().c_str(), &dspSelected))
					activeDSP = dspElem;
			}

			ImGui::EndCombo();
		}

		DSPChain[activeDSP]->editDSP(&DSPEditor); // function to actually render editing code
	}
	else
	{
		ImGui::TextWrapped("No DSPs attached!");
	}

	ImGui::PopItemWidth();
	ImGui::EndChild();

	
	ImGui::PopStyleVar();
	

	ImGui::End();
}

void ObjectEditor::editBusMixing()
{
	static float busVolumeDB    = 0.0f;
	static int   busPanning     = 50;
	static float busSpeedFactor = 1.0f;
	static Audio::AudioBusType currentBus = Audio::bus_Default;

	ImGui::Begin("Bus Mixer");
	ImGui::PushID("BusMixing");

	if (ImGui::Combo("##B", reinterpret_cast<int*>(&currentBus), BusTargets, sizeof(BusTargets) / sizeof(char*)))
	{
		busVolumeDB = bInterface->getBusGain(currentBus);
		busPanning     = bInterface->getBusPanning(currentBus);
		busSpeedFactor = bInterface->getBusSpeedupFactor(currentBus);
	}

	ImVec2 childDimensions(ImGui::GetWindowContentRegionWidth(), 0);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

	ImGui::BeginChild("Bus Mixing", childDimensions, true);

	ImGui::PushItemWidth(childDimensions.x * 0.955f);

	if (currentBus != Audio::bus_Default)
	{
		ImGui::TextWrapped("Volume (db)");
		ImGui::DragFloat("##dB", &busVolumeDB, 0.1f, -96.0f, 12.0f, "%.1f");
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			bInterface->setBusGain(currentBus, busVolumeDB);
		}

		ImGui::Separator();

		ImGui::TextWrapped("Pitch Shift (cents)");
		float cents = speedFactorToCents(busSpeedFactor);
		if (ImGui::DragFloat("##PS", &cents, 0.1f, -1200.0f, 1200.0f, "%.1f"))
		{
			busSpeedFactor = pitchToSpeedFactor(cents);
			bInterface->setBusSpeedupFactor(currentBus, busSpeedFactor);
		}

		ImGui::Separator();

		ImGui::TextWrapped("Panning Tool (L to R)");
		ImGui::DragInt("##P", &busPanning, 1.0f, 0, 100);
		if (ImGui::IsItemDeactivatedAfterEdit())
			bInterface->setBusPanning(currentBus, busPanning);
	}

	ImGui::PopItemWidth();

	ImGui::EndChild();

	ImGui::PopStyleVar();

	ImGui::PopID();
	ImGui::End();
}

enum FilterTypes
{
	lowpass,
	highpass,
	bandpass
};

void ObjectEditor::createFilterPrompt()
{
	if (ImGui::BeginPopupModal("filterDialogue"))
	{
		ImGui::Text("Create a new filter, this will overwrite any current filter applied to the object");
		
		static int currentSelection = 0;
		Audio::FilterFactory& factory = Audio::FilterFactory::getInstance();
		std::vector<std::string> filterList = factory.getFilterNameList();
		if (filterList.size())
		{
			if (ImGui::BeginCombo("filterList", filterList[currentSelection].c_str()))
			{
				for (int filterTypes = 0; filterTypes < (int)filterList.size(); ++filterTypes)
				{
					bool typeSelected = (currentSelection == filterTypes);
					if (ImGui::Selectable(filterList[filterTypes].c_str(), typeSelected))
						currentSelection = filterTypes;
				}
				ImGui::EndCombo();
			}
		}

		if (ImGui::Button("Create Filter"))
		{	
			Audio::FilterBase* filter = factory.createFilter(filterList[currentSelection]);
			if (activeParent)
				activeParent->setFilterBaseOverride(filter, activeObject->getName());
			else
				activeObject->setFilter(filter);

			ImGui::CloseCurrentPopup(); // close "filterDialogue"
		}

		ImGui::EndPopup();
	}
}

void ObjectEditor::createDSPPrompt()
{
	if (ImGui::BeginPopupModal("DSPDialogue"))
	{
		ImGui::Text("Create a new filter, this will overwrite any current filter applied to the object");

		Audio::DSPFactory& factory = Audio::DSPFactory::getInstance();
		const std::vector<std::string>& DSPList = factory.getDSPNameList();

		static int currentSelection = 0;

		if (DSPList.size())
		{
			if (ImGui::BeginCombo("DSPList", DSPList[currentSelection].c_str()))
			{
				for (int dspTypes = 0; dspTypes < (int)DSPList.size(); ++dspTypes)
				{
					bool typeSelected = (currentSelection == dspTypes);
					if (ImGui::Selectable(DSPList[dspTypes].c_str(), typeSelected))
						currentSelection = dspTypes;
				}
				ImGui::EndCombo();
			}
		}

		if (ImGui::Button("Create DSP"))
		{
			// Filter creation logic goes here
			factory.createDSP(DSPList[currentSelection]);

			ImGui::CloseCurrentPopup(); // close "DSPDialogue"
		}

		ImGui::EndPopup();
	}
}

bool ObjectEditor::editEnvelope()
{
	float windowWidth = ImGui::GetWindowContentRegionWidth();
	ImVec2 childDimensions(windowWidth, 300);

	ImGui::BeginChild("Trapezoid Envelope Editor", childDimensions, true);

	ImGui::PushItemWidth(childDimensions.x * 0.95f);
	ImGui::TextWrapped("Trapezoid Envelope Editor");
	ImGui::NewLine();

	bool dirtyFlag = false;

	bool looping = activeSettings.looping;
	if (ImGui::Checkbox("Loop sound", &looping))
	{
		activeSettings.looping = looping;
		dirtyFlag = true;
	}

	float value = activeSettings.maxGain;
	value = gainTodB(value);

	ImGui::TextWrapped("Length of track: %.3f", activeSettings.signalTime);
	ImGui::NewLine();

	// Move these hard coded values for min and max values into an object or macro or something
	ImGui::TextWrapped("Sustain Level (dB):");
	ImGui::DragFloat("##dB", &value, 0.1f, -96.0f, 12.0f, "%.1f");
	if (ImGui::IsItemDeactivatedAfterEdit())
		dirtyFlag = true;

	activeSettings.maxGain = dBToGain(value);

	ImGui::NewLine();

	ImGui::TextWrapped("Fade In Length (seconds):");
	ImGui::DragFloat("##In", &activeSettings.fadeInTime, 0.01f, 0.0f, activeSettings.signalTime, "%.2f");
	if (ImGui::IsItemDeactivatedAfterEdit())
		dirtyFlag = true;

	ImGui::NewLine();

	ImGui::TextWrapped("Fade Out Length (seconds):");
	ImGui::DragFloat("##Out", &activeSettings.fadeOutTime, 0.01f, 0.0f, activeSettings.signalTime, "%.2f");
	if (ImGui::IsItemDeactivatedAfterEdit())
		dirtyFlag = true;

	ImGui::NewLine();

	if (ImGui::Button((std::string("Randomize max gain").c_str())))
		ImGui::OpenPopup("Randomize");

	if (ImGui::BeginPopup("Randomize"))
	{
		//ImGui::Checkbox("Enable Random");
		ImGui::TextWrapped("Place holder");
		//ImGui::InputFloat("Gain Max", &gainRandMax, 0.1f, 1.0f, 1);
		//if (gainRandMax > 12.0f) gainRandMax = 12.0f; if (gainRandMax < -96.0f) gainRandMax = -96.0f;
		//ImGui::InputFloat("Gain Min", &gainRandMin, 0.1f, 1.0f, 1);
		//if (gainRandMin > 12.0f) gainRandMin = 12.0f; if (gainRandMin < -96.0f) gainRandMin = -96.0f;

		ImGui::EndPopup();
	}

	

	ImGui::PopItemWidth();
	ImGui::EndChild();

	return dirtyFlag;
}

#define MIN_PITCH -1200.0f
#define MAX_PITCH 1200.0f

bool ObjectEditor::editPitch()
{
	ImVec2 childDimensions(ImGui::GetWindowContentRegionWidth(), 110);

	ImGui::BeginChild(std::string("Pitch Slider").c_str(), childDimensions, true);
	ImGui::PushItemWidth(childDimensions.x * 0.955f);

	ImGui::TextWrapped("Pitch Change Editor");
	ImGui::NewLine();
	
	bool dirtyFlag = false;

	ImGui::TextWrapped("Pitch Shift (cents):");
	ImGui::DragFloat("##V", &activeSettings.pitchChangeCents, 0.1f, MIN_PITCH, MAX_PITCH, "%.1f");
	if (ImGui::IsItemDeactivatedAfterEdit())
		dirtyFlag = true;

	if (ImGui::Button((std::string("Randomize Pitch").c_str())))
		ImGui::OpenPopup("Randomize");

	if (ImGui::BeginPopup("Randomize"))
	{
		//ImGui::Checkbox("Enable Random");
		ImGui::TextWrapped("Place holder");
		//ImGui::InputFloat("Gain Max", &gainRandMax, 0.1f, 1.0f, 1);
		//if (gainRandMax > 12.0f) gainRandMax = 12.0f; if (gainRandMax < -96.0f) gainRandMax = -96.0f;
		//ImGui::InputFloat("Gain Min", &gainRandMin, 0.1f, 1.0f, 1);
		//if (gainRandMin > 12.0f) gainRandMin = 12.0f; if (gainRandMin < -96.0f) gainRandMin = -96.0f;

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();
	ImGui::EndChild();

	return dirtyFlag;
}
