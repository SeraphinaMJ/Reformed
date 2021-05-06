/*!***************************************************************************************
\file       AudioSystem.cpp
\author     Morgen Hyde
\date       9/20/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Definition of the Audio Engine's interface.
*****************************************************************************************/
#include "AudioSystem.h"
#include "AE_Events.hpp"
#include "SystemManager.h"

AudioSystem::AudioSystem() 
	: engine(new Audio::AudioEngine("../data/audio/")), systemName("AudioSystem")
{
}

AudioSystem::~AudioSystem()
{
	delete engine;
}

bool AudioSystem::onInitialize()
{
	engine->onInitialize();
	getSystemManager()->registerEventCallback<AudioSystem, Audio::PlayEvent, &AudioSystem::playHandler>(this);
	getSystemManager()->registerEventCallback<AudioSystem, Audio::StopEvent, &AudioSystem::stopHandler>(this);
	getSystemManager()->registerEventCallback<AudioSystem, Audio::PauseEvent, &AudioSystem::pauseHandler>(this);
	getSystemManager()->registerEventCallback<AudioSystem, Audio::IOTargetSwapEvent, &AudioSystem::IOSwapHandler>(this);

	return true;
}

void AudioSystem::onUpdate() 
{
	engine->onUpdate();
}

void AudioSystem::onShutdown()
{
	engine->onShutdown();
}

Audio::BusInterface* AudioSystem::getBusInterface() const 
{ 
	return engine; 
}

// adjust play events to contain source type information
void AudioSystem::playHandler(Audio::PlayEvent* pEvent)
{
	Audio::PlayEvent* pEventCopy = new Audio::PlayEvent(pEvent->ObjectID, pEvent->objectName, pEvent->parentName);
	engine->postEvent(pEventCopy);
}

void AudioSystem::stopHandler(Audio::StopEvent* sEvent)
{
	Audio::StopEvent* sEventCopy = new Audio::StopEvent(sEvent->ObjectID, sEvent->fileName);
	engine->postEvent(sEventCopy);
}

void AudioSystem::pauseHandler(Audio::PauseEvent* pEvent)
{
	Audio::PauseEvent* pEventCopy = new Audio::PauseEvent(pEvent->ObjectID, pEvent->fileName);
	engine->postEvent(pEventCopy);
}

void AudioSystem::IOSwapHandler(Audio::IOTargetSwapEvent* iEvent)
{
	engine->postEvent(new Audio::IOTargetSwapEvent(iEvent->IOTarget));
}