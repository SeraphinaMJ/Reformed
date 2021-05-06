/*!***************************************************************************************
\file       AudioSystem.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../SystemBase.hpp"
#include "AudioEngine.hpp"

class AudioSystem final : public systemBase
{
public:
	AudioSystem();
	~AudioSystem();


	void sendAudioEvent(Audio::EventBase* audioEvent);

	static const std::string& getName() { static const std::string n("AudioSystem"); return n; }
	const std::string& name() const override{ return getName(); }

protected:
	void onInitialize() override;
	void onUpdate() override;
	void onShutdown() override;

private:
	Audio::AudioEngine* engine;

};