/*!***************************************************************************************
\file       ObjectEditor.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "UI_System.h"
#include "UIEventTypes.h"
#include "MixerObjects.h"
#include "AudioStorage.hpp"
#include "FilterManipulator.h"
#include "DSPManipulator.h"

//  TODO: Move these to a dispatcher like with filters and DSPs
/*
class envelopeDisplay : public Audio::TrapezoidEnvelope
{
public:
	envelopeDisplay() = default;
	envelopeDisplay& operator=(const Audio::TrapezoidEnvelope& source);
	bool displayAndEdit();
};

class pitchDisplay : public Audio::pitchParameter
{
public:
	pitchDisplay() = default;
	pitchDisplay& operator=(const Audio::pitchParameter& source);
	bool displayAndEdit();

};
*/

class ObjectEditor : public UI_ComponentsBase
{
public:
	ObjectEditor(Audio::BusInterface* busInterface_p);
	~ObjectEditor() override;

	void onUpdate() override;
	void setActiveObject(UI::ActiveSwapEvent* swapEvent);

private:

	enum viewWindow
	{
		view_Closed,
		view_GraphEdit,
		view_NodeEdit,
	};

	void editGraphs();
	void editProperties();
	void editProcessing();

	bool editEnvelope();
	bool editPitch();

	void editBusMixing();

	void createFilterPrompt();
	void createDSPPrompt();

	viewWindow activeView;

	Audio::BusInterface* bInterface;

	Audio::AssetBase* activeObject;
	Audio::AudioNode* activeParent;
	Audio::MixSettings activeSettings;

	FilterManipulator filterEditor;
	DSPManipulator DSPEditor;

	//int outputBusTarget;

	//bool parametersDirty;
	//envelopeDisplay* activeEnvelope;
	//pitchDisplay* activePitch;
	//int panValue;
	//std::vector<FilterDisplay*> activeFilters;
	//std::vector<DSPDisplay*> activeEffects;


	//std::vector<displayBase*> displayParameters;
};
