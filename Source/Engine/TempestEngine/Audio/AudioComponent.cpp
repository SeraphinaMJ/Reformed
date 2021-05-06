/*!***************************************************************************************
\file       AudioComponent.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "AudioComponent.hpp"
#include "AudioEngine.hpp"

#include <winnt.h>

namespace Audio
{
	unsigned int AudioComponent::NumAudioComponents = 0;

	AudioComponent::AudioComponent()
	{
		ObjectID = NumAudioComponents;
		NumAudioComponents++;
	}

	void AudioComponent::Load(const std::string& AudioFile)
	{
		LoadEvent* lEvent = new LoadEvent(AudioFile);
		SendAudioEvent(lEvent);
	}

	void AudioComponent::Unload(const std::string& AudioFile)
	{
		if (AudioFile.c_str())
		{

		}
	}

	void AudioComponent::Start(const std::string& AudioFile)
	{
		PlayEvent* pEvent = new PlayEvent(ObjectID, AudioFile);
		SendAudioEvent(pEvent);
	}

	void AudioComponent::Stop(const std::string& AudioFile)
	{
		if (AudioFile.c_str())
		{

		}
	}

	void AudioComponent::Pause(const std::string& AudioFile)
	{
		if (AudioFile.c_str())
		{

		}
	}

	bool AudioComponent::IsPaused(const std::string& AudioFile)
	{
		if (AudioFile.c_str())
		{

		}
		return false;
	}

	typeRT AudioComponent::toTypeRT() const
	{
		typeRT audioComp;
		audioComp.setTypeName("AudioComponent");
		audioComp.setVariableName("AudioComponent");

		typeRT pause("paused", false);

		audioComp.insertMember(pause);

		return audioComp;
	}

	void AudioComponent::updateFromTypeRT(typeRT& p_type)
	{
		UNREFERENCED_PARAMETER(p_type);
	}



} // namespace Audio
