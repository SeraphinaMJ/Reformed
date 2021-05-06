/*!***************************************************************************************
\file       AudioScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "AudioScript.h"
#include "../Audio/AudioSystem.h"

audioScript::audioScript(typeRT p_data): scriptCPP(p_data)
{
}

void audioScript::onStart()
{
	//start logic
	getSystemManager()->getSystem<AudioSystem>()->sendAudioEvent(new Audio::LoadEvent("Music_Mariachi.node"));
	getSystemManager()->getSystem<AudioSystem>()->sendAudioEvent(new Audio::PlayEvent(1256, "Music_Mariachi.node"));
}

void audioScript::onUpdate()
{
		//update logic


		//An example of getting a system
		//getSystem<AudioSystem>()->sendAudioEvent();
}
