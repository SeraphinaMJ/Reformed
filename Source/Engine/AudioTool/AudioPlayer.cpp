/*!***************************************************************************************
\file       AudioPlayer.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "AudioPlayer.h"
#include "ImGui/imgui.h"
#include "AE_Events.hpp"
#include "AudioStorage.hpp"

Audio::GameObjectID audioPlayerID = 0xFFFF;

AudioPlayer::AudioPlayer() : activeObject(nullptr)
{
}

AudioPlayer::~AudioPlayer()
{
}

void AudioPlayer::onUpdate()
{
	if (activeObject && activeObject->getName() != "noObj")
	{
		bool shown = true;
		ImGui::Begin("AudioPlayer", &shown, ImVec2(100.0f, 100.0f));
		ImGui::Text("%s", activeObject->getName().c_str());

		if (ImGui::Button("Play", ImVec2(50.0f, 20.0f)))
		{
			Audio::PlayEvent* playEvent = new Audio::PlayEvent(audioPlayerID, activeObject->getName());
			if (activeParent)
				playEvent->parentName = activeParent->getName();

			sendGlobalEvent(playEvent);
		}

		ImGui::SameLine();

		if (ImGui::Button("Pause", ImVec2(50.0f, 20.0f)))
		{
			sendGlobalEvent(new Audio::PauseEvent(audioPlayerID, activeObject->getName()));
		}

		ImGui::SameLine();

		if (ImGui::Button("Stop", ImVec2(50.0f, 20.0f)))
		{
			sendGlobalEvent(new Audio::StopEvent(audioPlayerID, activeObject->getName()));
		}

		ImGui::End();
	}
}

void AudioPlayer::changeActiveCallback(UI::ActiveSwapEvent* eventPtr)
{
	activeObject = eventPtr->data;
	activeParent = eventPtr->parent;
}
