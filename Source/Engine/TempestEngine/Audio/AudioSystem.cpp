/*!***************************************************************************************
\file       AudioSystem.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "AudioSystem.h"
#include "AE_Events.hpp"
#include "AudioEngine.hpp"
#include <Configuration.hpp>

AudioSystem::AudioSystem() : engine{nullptr}
{ }

AudioSystem::~AudioSystem()
{
}

void AudioSystem::onInitialize()
{
	engine = new Audio::AudioEngine(getSystemManager()->getConfigurationFunction()().data_path / "audio/");
	engine->onInitialize();
}

void AudioSystem::onUpdate()
{
	engine->onUpdate();
}

void AudioSystem::onShutdown()
{
	engine->onShutdown();
	delete engine;
	engine = nullptr;
}

void AudioSystem::sendAudioEvent(Audio::EventBase* audioEvent)
{
	engine->postEvent(audioEvent);
}
