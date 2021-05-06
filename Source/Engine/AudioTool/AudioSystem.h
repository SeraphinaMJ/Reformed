/*!***************************************************************************************
\file       AudioSystem.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "SystemBase.h"
#include "AudioEngine.hpp"
#include "UIEventTypes.h"

class AudioSystem : public SystemBase
{
public:
	AudioSystem();
	~AudioSystem();

	bool onInitialize() override;
	void onUpdate() override;
	void onShutdown() override;

	void sendAudioEvent(Audio::EventBase* audioEvent) { engine->postEvent(audioEvent); }

	Audio::BusInterface* getBusInterface() const;

	const std::string& getSystemName() const override { return systemName; }

	void playHandler(Audio::PlayEvent* pEvent);
	void stopHandler(Audio::StopEvent* sEvent);
	void pauseHandler(Audio::PauseEvent* pEvent);
	void IOSwapHandler(Audio::IOTargetSwapEvent* iEvent);

private:
	Audio::AudioEngine* engine;
	std::string systemName;

};
